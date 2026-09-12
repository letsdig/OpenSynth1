#pragma once
#include <algorithm>
#include <cmath>
#include <juce_audio_basics/juce_audio_basics.h>

/**
 * OpenSynth1 Tribute Filter Engine
 *
 * Modes:
 *  0: LP12 (12dB State Variable Lowpass, 2x oversampled)
 *  1: LP24 (24dB 4-pole cascaded filter with negative feedback)
 *  2: HP12 (12dB State Variable Highpass, 2x oversampled)
 *  3: BP12 (12dB State Variable Bandpass, 2x oversampled)
 *  4: LPDL (VA Diode Ladder with DC blocker, Padé diode saturation, dynamic
 * oversampling)
 */
class OpenSynthFilter {
public:
  OpenSynthFilter() {
    initCutoffTable();
    reset();
  }

  void reset() {
    svfS1 = 0.0;
    svfS2 = 0.0;

    for (int i = 0; i < 4; ++i) {
      lp24S[i] = 0.0;
      lpdlV[i] = 0.0;
    }

    for (int i = 0; i < 3; ++i) {
      lp24SPrev[i] = 0.0;
    }

    lp24InPrev = 0.0;
    lpdlVHp = 0.0;
    limiterEnv = 0.0f;
  }

  /**
   * Compute modulated filter cutoff frequency in Hz.
   * Pivot note is MIDI note 48 (C3).
   * Bounded to table index [0, 1023].
   */
  static float calculateCutoffHz(float rawCutoff127, int midiNote,
                                 float rawTrack127, float rawAmt127,
                                 float envVal, float lfoMod) {
    float baseIdx =
        juce::jlimit(0.0f, 1023.0f, rawCutoff127 * (1023.0f / 127.0f));

    float trackDelta = 0.0f;
    if (rawTrack127 > 0.5f) {
      float trackRatio = rawTrack127 / 64.0f;
      float semiOffset = (float)(midiNote - 48); // Pivot su C3 (MIDI note 48)
      trackDelta = semiOffset * trackRatio * 8.483383f;
    }

    float amtNormalized = (rawAmt127 - 64.0f) / 63.0f;
    float envDelta = envVal * amtNormalized * 1023.0f;

    float combinedIdx =
        juce::jlimit(0.0f, 1023.0f, baseIdx + trackDelta + envDelta);

    // LFO cutoff modulation in musical pitch units (160 indices = ~1.57 octaves max at 100% depth).
    // Symmetrical and stable across all cutoff settings, avoiding wild exaggerated swings on low-cutoff pads.
    float targetIdx = juce::jlimit(0.0f, 1023.0f, combinedIdx + lfoMod * 160.0f);

    int i0 = (int)targetIdx;
    int i1 = std::min(1023, i0 + 1);
    float frac = targetIdx - (float)i0;
    return cutoffTable[i0] + frac * (cutoffTable[i1] - cutoffTable[i0]);
  }

  float processSample(float inSample, float cutoffHz, float fRes, int fType,
                      float sampleRate) {
    double filtered = 0.0;

    switch (fType) {
    case 0: // LP12
    case 2: // HP12
    case 3: // BP12
      filtered = processSVF12((double)inSample, (double)cutoffHz, (double)fRes,
                              fType, (double)sampleRate);
      break;
    case 1: // LP24
      filtered = processLP24((double)inSample, (double)cutoffHz, (double)fRes,
                             (double)sampleRate);
      break;
    case 4: // LPDL
      filtered = (double)processLPDL(inSample, cutoffHz, fRes, sampleRate);
      break;
    }

    // Peak Limiter Feed-forward
    float outF = (float)filtered;
    float excess = std::max(0.0f, std::abs(outF) - 1.1f);
    limiterEnv = limiterEnv * 0.995f + excess * 0.005f;
    float limiterGain = 1.1f / (1.1f + limiterEnv);
    return outF * limiterGain;
  }

private:
  // =========================================================================
  // Mode 0, 2, 3: 12dB Chamberlin SVF
  // =========================================================================
  double processSVF12(double inSample, double cutoffHz, double fRes, int fType,
                      double sampleRate) {
    cutoffHz = std::max(15.0, cutoffHz); 

    double F = 2.0 * std::sin(juce::MathConstants<double>::pi * cutoffHz /
                              (2.0 * sampleRate));
    F = juce::jlimit(0.0001, 1.414, F);

    double fMul = 1.0 - 0.35 * fRes;
    double R = fRes - 1.0;

    double xIn = inSample * fMul;

    // Sub-step 1
    double satS2 = diodePade(svfS2);
    svfS1 += F * svfS2;
    double hp1 = xIn - svfS1 + R * satS2;
    double bp1 = svfS2 + F * hp1;

    // Sub-step 2
    double satBp1 = diodePade(bp1);
    svfS1 += F * bp1;
    double hp2 = xIn - svfS1 + R * satBp1;
    svfS2 = bp1 + F * hp2;

    if (fRes > 0.9 || !std::isfinite(svfS1) || !std::isfinite(svfS2)) {
      if (!std::isfinite(svfS1))
        svfS1 = 0.0;
      if (!std::isfinite(svfS2))
        svfS2 = 0.0;
      svfS1 = juce::jlimit(-20.0, 20.0, svfS1);
      svfS2 = juce::jlimit(-20.0, 20.0, svfS2);
    }

    if (fType == 2)
      return hp2; // HP12
    else if (fType == 3)
      return svfS2; // BP12
    return svfS1;   // LP12
  }

  // =========================================================================
  // Mode 1: LP24 4-pole cascaded
  // =========================================================================
  double processLP24(double inSample, double cutoffHz, double fRes,
                     double sampleRate) {
    cutoffHz = std::max(15.0, cutoffHz); 
    double w = (2.0 * cutoffHz) / sampleRate;
    w = juce::jlimit(0.0001, 0.95, w);

    // Filter tuning constants
    const double C_3_6 = 3.6000000000; 
    const double C_1_6 = 1.6000000000; 
    const double C_4_3 = 4.3000000000; 
    const double C_LN4 = 1.3862490000; 

    double c1 = C_3_6 * w - C_1_6 * w * w - 1.0;
    double ca = (c1 + 1.0) * 0.5;

    double wRes = (C_4_3 * w - 2.5 * w * w) * 0.5;
    double expVal = (1.0 - wRes) * C_LN4;
    double resScale = fRes * std::exp(juce::jlimit(-10.0, 10.0, expVal));

    // Non-linear diode saturation in the resonance feedback path
    double satFb = diodePade(lp24S[3]);
    double x = inSample - resScale * satFb;

    double s0 = (x + lp24InPrev) * ca - c1 * lp24S[0];
    lp24InPrev = x;
    lp24S[0] = s0;

    double s1 = (s0 + lp24SPrev[0]) * ca - c1 * lp24S[1];
    lp24SPrev[0] = s0;
    lp24S[1] = s1;

    double s2 = (s1 + lp24SPrev[1]) * ca - c1 * lp24S[2];
    lp24SPrev[1] = s1;
    lp24S[2] = s2;

    double s3 = (s2 + lp24SPrev[2]) * ca - c1 * lp24S[3];
    lp24SPrev[2] = s2;
    lp24S[3] = s3;

    if (fRes > 0.9 || !std::isfinite(lp24S[3])) {
      for (int i = 0; i < 4; ++i) {
        if (!std::isfinite(lp24S[i]))
          lp24S[i] = 0.0;
        lp24S[i] = juce::jlimit(-20.0, 20.0, lp24S[i]);
      }
    }

    // Passband gain compensation: 4-pole cascaded filters naturally drop in volume
    // as resonance increases. Compensate so resonant presets retain punch and body.
    double resComp = 1.0 + 0.5 * fRes;
    return lp24S[3] * resComp;
  }

  // =========================================================================
  // Mode 4: Diode Ladder VA Model
  // =========================================================================
  static inline double diodePade(double u) {
    if (u < -3.0)
      return -9.0 / 7.0;
    if (u > 3.0)
      return 9.0 / 7.0;
    return (u * (u * u + 27.0)) / (7.0 * (u * u + 3.0));
  }

  float processLPDL(float inSample, float cutoffHz, float fRes,
                    float sampleRate) {
    int os = (sampleRate <= 48000.0f) ? 2 : 1;
    double fsEff = (double)sampleRate * (double)os;

    double fc = std::min((double)cutoffHz * 1.5, (double)sampleRate * 0.45);
    fc = std::max(15.0, fc);

    double g = std::tan(juce::MathConstants<double>::pi * fc / fsEff);
    g = juce::jlimit(0.0001, 10.0, g);

    double kRes = (double)fRes * 15.0;
    double makeupGain = ((double)os * 0.65) * (1.0 + 3.0 * (double)fRes);

    double fHp = 30.0;
    if (cutoffHz >= 300.0f) {
      fHp = 120.0;
    } else if (cutoffHz > 100.0f) {
      double t = (std::log((double)cutoffHz) - std::log(100.0)) /
                 (std::log(300.0) - std::log(100.0));
      fHp = std::exp(std::log(30.0) + t * (std::log(120.0) - std::log(30.0)));
    }
    double gHp = std::tan(juce::MathConstants<double>::pi * fHp / fsEff);
    double hpCoeff = 2.0 * gHp / (1.0 + gHp);
    double hpDenom = 1.0 / (1.0 + gHp);

    double G = g / (1.0 + g);
    double gHalf = G * 0.5;

    double d1 = 1.0 / (1.0 - gHalf * gHalf);
    double e1 = d1 * gHalf;
    double d2 = 1.0 / (1.0 - e1 * gHalf);
    double e2 = d2 * gHalf;
    double d3 = 1.0 / (1.0 - e2 * gHalf);

    double outSample = 0.0;

    for (int step = 0; step < os; ++step) {
      double xIn = (step == 0) ? (double)inSample : 0.0;

      double fbRaw = lpdlV[3] * kRes;
      double hpOut = fbRaw - lpdlVHp;
      lpdlVHp += hpCoeff * hpOut;
      double fb = hpOut * hpDenom;

      double p1 = lpdlV[0] + gHalf * lpdlV[1];
      double p2 = (lpdlV[1] + gHalf * lpdlV[2]) * d1 + e1 * p1;
      double p3 = (lpdlV[2] + gHalf * lpdlV[3]) * d2 + e2 * p2;
      double p4 = lpdlV[3] * d3 + (d3 * gHalf) * p3;

      double uIn = (xIn - fb - G * p4);
      double satOut = diodePade(uIn);

      double du0 = (satOut - lpdlV[0]);
      double du1 = (lpdlV[0] - lpdlV[1]);
      double du2 = (lpdlV[1] - lpdlV[2]);
      double du3 = (lpdlV[2] - lpdlV[3]);

      lpdlV[0] += G * (du0 - gHalf * du1);
      lpdlV[1] += G * (du1 - gHalf * du2);
      lpdlV[2] += G * (du2 - gHalf * du3);
      lpdlV[3] += G * du3;

      for (int i = 0; i < 4; ++i) {
        if (!std::isfinite(lpdlV[i]))
          lpdlV[i] = 0.0;
        lpdlV[i] = juce::jlimit(-15.0, 15.0, lpdlV[i]);
      }
      if (!std::isfinite(lpdlVHp))
        lpdlVHp = 0.0;
      lpdlVHp = juce::jlimit(-15.0, 15.0, lpdlVHp);

      outSample = lpdlV[3];
    }

    return (float)(outSample * makeupGain);
  }

  // =========================================================================
  // Cutoff Table a 1024 punti (15.0 Hz a 16000.0 Hz, ratio 1066.66667)
  // =========================================================================
  static inline float cutoffTable[1025];
  static inline bool tableInitialized = false;

  static void initCutoffTable() {
    if (tableInitialized)
      return;
    double ratio = 1066.6666666666667; 
    for (int i = 0; i <= 1024; ++i) {
      cutoffTable[i] = (float)(15.0 * std::pow(ratio, (double)i / 1024.0));
    }
    tableInitialized = true;
  }

  double svfS1 = 0.0;
  double svfS2 = 0.0;

  double lp24S[4] = {0.0};
  double lp24InPrev = 0.0;
  double lp24SPrev[3] = {0.0};

  double lpdlV[4] = {0.0};
  double lpdlVHp = 0.0;

  float limiterEnv = 0.0f;
};
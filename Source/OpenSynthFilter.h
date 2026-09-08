#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <cmath>
#include <algorithm>

/**
 * OpenSynth1 Virtual Analog Filter Engine
 *
 * Mode indices:
 *  0: LP12 (12dB State Variable Lowpass, 2x oversampled)
 *  1: LP24 (24dB 4-pole cascaded filter with stage-4 negative feedback)
 *  2: HP12 (12dB State Variable Highpass, 2x oversampled)
 *  3: BP12 (12dB State Variable Bandpass, 2x oversampled)
 *  4: LPDL (VA Diode Ladder with DC blocker, Padé diode saturation, dynamic oversampling)
 */
class OpenSynthFilter
{
public:
    OpenSynthFilter()
    {
        initCutoffTable();
        reset();
    }

    void reset()
    {
        // SVF 12dB states
        svfS1 = 0.0f;
        svfS2 = 0.0f;

        // LP24 4-pole cascaded states
        lp24S[0] = 0.0f;
        lp24S[1] = 0.0f;
        lp24S[2] = 0.0f;
        lp24S[3] = 0.0f;
        lp24InPrev = 0.0f;
        lp24SPrev[0] = 0.0f;
        lp24SPrev[1] = 0.0f;
        lp24SPrev[2] = 0.0f;

        // LPDL 5 double states (4 capacitors + DC blocker)
        lpdlV[0] = 0.0;
        lpdlV[1] = 0.0;
        lpdlV[2] = 0.0;
        lpdlV[3] = 0.0;
        lpdlVHp = 0.0;

        limiterEnv = 0.0f;
    }

    /**
     * Compute modulated filter cutoff frequency in Hz.
     * Pivot note is MIDI note 48 (C3).
     * Asymmetric modulation bounds the 10-bit integer table index in [0, 1023].
     */
    static float calculateCutoffHz(float rawCutoff127,
                                  int midiNote,
                                  float rawTrack127,
                                  float rawAmt127,
                                  float envVal,
                                  float lfoMod)
    {
        // 1. Base cutoff index 0..1023
        float baseIdx = juce::jlimit(0.0f, 1023.0f, rawCutoff127 * (1023.0f / 127.0f));

        // 2. Key tracking: pivot = note 48 (C3)
        float trackDelta = 0.0f;
        if (rawTrack127 > 0.5f)
        {
            float trackRatio = rawTrack127 / 64.0f;
            float semiOffset = (float)(midiNote - 48);
            // Steps per semitone in 1024-step table spanning 15Hz to 16000Hz:
            // log2(16000/15) = 10.05889 octaves = 120.7067 semitones
            // 1024.0 / 120.7067 ≈ 8.48338 steps/semitone
            trackDelta = semiOffset * trackRatio * 8.483383f;
        }

        // 3. Envelope modulation: rawAmt: 64 = 0 (bipolar: 0..127)
        float amtNormalized = (rawAmt127 - 64.0f) / 63.0f; // -1.0 .. +1.0
        float envDelta = envVal * amtNormalized * 1023.0f;

        // 4. Combined pre-LFO index
        float combinedIdx = juce::jlimit(0.0f, 1023.0f, baseIdx + trackDelta + envDelta);

        // 5. Asymmetric LFO modulation
        float targetIdx = combinedIdx;
        if (lfoMod > 0.0f)
        {
            targetIdx += (1023.0f - combinedIdx) * lfoMod;
        }
        else if (lfoMod < 0.0f)
        {
            targetIdx += combinedIdx * lfoMod;
        }
        targetIdx = juce::jlimit(0.0f, 1023.0f, targetIdx);

        // 6. Table lookup with linear interpolation
        int i0 = (int)targetIdx;
        int i1 = std::min(1023, i0 + 1);
        float frac = targetIdx - (float)i0;
        return cutoffTable[i0] + frac * (cutoffTable[i1] - cutoffTable[i0]);
    }

    /**
     * Process one audio sample through the selected filter mode with resonance limiting.
     *
     * @param inSample   Input audio sample
     * @param cutoffHz   Cutoff frequency in Hz (from calculateCutoffHz)
     * @param fRes       Normalized resonance [0.0, 1.0]
     * @param fType      Filter mode index (0: LP12, 1: LP24, 2: HP12, 3: BP12, 4: LPDL)
     * @param sampleRate Current project sample rate
     */
    float processSample(float inSample, float cutoffHz, float fRes, int fType, float sampleRate)
    {
        float filtered = 0.0f;

        switch (fType)
        {
            case 0: // LP12
            case 2: // HP12
            case 3: // BP12
            {
                filtered = processSVF12(inSample, cutoffHz, fRes, fType, sampleRate);
                break;
            }
            case 1: // LP24
            {
                filtered = processLP24(inSample, cutoffHz, fRes, sampleRate);
                break;
            }
            case 4: // LPDL
            {
                filtered = processLPDL(inSample, cutoffHz, fRes, sampleRate);
                break;
            }
        }

        // Feed-Forward Peak Limiter
        float excess = std::max(0.0f, std::abs(filtered) - 1.1f);
        limiterEnv = limiterEnv * 0.995f + excess * 0.005f;
        float limiterGain = 1.1f / (1.1f + limiterEnv);
        return filtered * limiterGain;
    }

private:
    // =========================================================================
    // Mode 0, 2, 3: 12dB Chamberlin SVF (2x oversampled)
    // =========================================================================
    float processSVF12(float inSample, float cutoffHz, float fRes, int fType, float sampleRate)
    {
        // Tuning parameter F with 2x oversampling: F = 2.0 * sin(pi * fc / (2 * fs))
        float F = 2.0f * std::sin(juce::MathConstants<float>::pi * cutoffHz / (2.0f * sampleRate));
        F = juce::jlimit(0.0001f, 1.414f, F);

        // Passband compensation: fMul = 1.0 - 0.7 * res
        float fMul = 1.0f - 0.7f * fRes;
        // Resonance feedback: R = res - 1.0
        float R = fRes - 1.0f;

        float xIn = inSample * fMul;

        // Sub-step 1:
        svfS1 += F * svfS2;
        float hp1 = xIn - svfS1 + R * svfS2;
        float bp1 = svfS2 + F * hp1;

        // Sub-step 2:
        svfS1 += F * bp1;
        float hp2 = xIn - svfS1 + R * bp1;
        svfS2 = bp1 + F * hp2;

        // Denormal & resonant blowup protection (clamp to [-20, 20] when res > 0.9)
        if (fRes > 0.9f || !std::isfinite(svfS1) || !std::isfinite(svfS2))
        {
            if (!std::isfinite(svfS1)) svfS1 = 0.0f;
            if (!std::isfinite(svfS2)) svfS2 = 0.0f;
            svfS1 = juce::jlimit(-20.0f, 20.0f, svfS1);
            svfS2 = juce::jlimit(-20.0f, 20.0f, svfS2);
        }

        if (fType == 2)      return hp2;   // HP12
        else if (fType == 3) return svfS2; // BP12
        return svfS1;                      // LP12 (default)
    }

    // =========================================================================
    // Mode 1: LP24 4-pole cascaded filter with negative feedback
    // =========================================================================
    float processLP24(float inSample, float cutoffHz, float fRes, float sampleRate)
    {
        float w = (2.0f * cutoffHz) / sampleRate;
        w = juce::jlimit(0.0001f, 0.95f, w);

        float c1 = 3.6f * w - 1.6f * w * w - 1.0f;
        float ca = (c1 + 1.0f) * 0.5f;

        // Resonance scaling:
        float wRes = (4.3f * w - 2.5f * w * w) * 0.5f;
        float expVal = (1.0f - wRes) * 1.386249f; // ln(4)
        float resScale = fRes * std::exp(juce::jlimit(-10.0f, 10.0f, expVal));

        // Negative feedback from stage 4:
        float x = inSample - resScale * lp24S[3];

        // 4 cascaded 1-pole stages:
        float s0 = (x + lp24InPrev) * ca - c1 * lp24S[0];
        lp24InPrev = x;
        lp24S[0] = s0;

        float s1 = (s0 + lp24SPrev[0]) * ca - c1 * lp24S[1];
        lp24SPrev[0] = s0;
        lp24S[1] = s1;

        float s2 = (s1 + lp24SPrev[1]) * ca - c1 * lp24S[2];
        lp24SPrev[1] = s1;
        lp24S[2] = s2;

        float s3 = (s2 + lp24SPrev[2]) * ca - c1 * lp24S[3];
        lp24SPrev[2] = s2;
        lp24S[3] = s3;

        if (fRes > 0.9f || !std::isfinite(lp24S[3]))
        {
            for (int i = 0; i < 4; ++i)
            {
                if (!std::isfinite(lp24S[i])) lp24S[i] = 0.0f;
                lp24S[i] = juce::jlimit(-20.0f, 20.0f, lp24S[i]);
            }
        }

        return lp24S[3];
    }

    // =========================================================================
    // Mode 4: LPDL Diode Ladder VA Model
    // =========================================================================
    static inline double diodePade(double u)
    {
        if (u < -3.0) return -9.0 / 7.0;
        if (u > 3.0)  return  9.0 / 7.0;
        return (u * (u * u + 27.0)) / (7.0 * (u * u + 3.0));
    }

    float processLPDL(float inSample, float cutoffHz, float fRes, float sampleRate)
    {
        // 1. Dynamic oversampling (sampleRate <= 48000 -> 2x, else 1x)
        int os = (sampleRate <= 48000.0f) ? 2 : 1;
        double fsEff = (double)sampleRate * (double)os;

        // Cutoff multiplied by 1.5x up to 0.45 * sampleRate
        double fc = std::min((double)cutoffHz * 1.5, (double)sampleRate * 0.45);
        fc = std::max(10.0, fc);

        // Pre-warping: g = tan(pi * fc / fsEff)
        double g = std::tan(juce::MathConstants<double>::pi * fc / fsEff);
        g = juce::jlimit(0.0001, 10.0, g);

        // Resonance parameter
        double kRes = (double)fRes * 15.0;

        // Passband makeup gain
        double makeupGain = ((double)os * 0.65) * (1.0 + 3.0 * (double)fRes);

        // Tracking highpass DC blocker cutoff 30Hz..120Hz
        double fHp = 30.0;
        if (cutoffHz >= 300.0f)
        {
            fHp = 120.0;
        }
        else if (cutoffHz > 100.0f)
        {
            double t = (std::log((double)cutoffHz) - std::log(100.0)) / (std::log(300.0) - std::log(100.0));
            fHp = std::exp(std::log(30.0) + t * (std::log(120.0) - std::log(30.0)));
        }
        double gHp = std::tan(juce::MathConstants<double>::pi * fHp / fsEff);
        double hpCoeff = 2.0 * gHp / (1.0 + gHp);
        double hpDenom = 1.0 / (1.0 + gHp);

        // Tridiagonal half-pole matrix solver coefficients
        double G = g / (1.0 + g);
        double gHalf = G * 0.5;

        double d1 = 1.0 / (1.0 - gHalf * gHalf);
        double e1 = d1 * gHalf;
        double d2 = 1.0 / (1.0 - e1 * gHalf);
        double e2 = d2 * gHalf;
        double d3 = 1.0 / (1.0 - e2 * gHalf);

        double outSample = 0.0;

        // Oversampling sub-steps
        for (int step = 0; step < os; ++step)
        {
            double xIn = (step == 0) ? (double)inSample : 0.0;

            // DC blocker on stage 4 feedback
            double fbRaw = lpdlV[3] * kRes;
            double hpOut = fbRaw - lpdlVHp;
            lpdlVHp += hpCoeff * hpOut;

            // Feedback with highpass DC blocker
            double fb = hpOut * hpDenom;

            // Half-pole diode ladder stage prediction
            double p1 = lpdlV[0] + gHalf * lpdlV[1];
            double p2 = (lpdlV[1] + gHalf * lpdlV[2]) * d1 + e1 * p1;
            double p3 = (lpdlV[2] + gHalf * lpdlV[3]) * d2 + e2 * p2;
            double p4 = lpdlV[3] * d3 + (d3 * gHalf) * p3;

            // Input stage non-linearity
            double uIn = (xIn - fb - G * p4);
            double satOut = diodePade(uIn);

            // Capacitor state updates with exact matrix loading
            double du0 = (satOut - lpdlV[0]);
            double du1 = (lpdlV[0] - lpdlV[1]);
            double du2 = (lpdlV[1] - lpdlV[2]);
            double du3 = (lpdlV[2] - lpdlV[3]);

            lpdlV[0] += G * (du0 - gHalf * du1);
            lpdlV[1] += G * (du1 - gHalf * du2);
            lpdlV[2] += G * (du2 - gHalf * du3);
            lpdlV[3] += G * du3;

            // Denormal & stability protection
            for (int i = 0; i < 4; ++i)
            {
                if (!std::isfinite(lpdlV[i])) lpdlV[i] = 0.0;
                lpdlV[i] = juce::jlimit(-15.0, 15.0, lpdlV[i]);
            }
            if (!std::isfinite(lpdlVHp)) lpdlVHp = 0.0;
            lpdlVHp = juce::jlimit(-15.0, 15.0, lpdlVHp);

            outSample = lpdlV[3];
        }

        return (float)(outSample * makeupGain);
    }

    // =========================================================================
    // Exponential Cutoff Table (1024 points, from 15Hz to 16000Hz)
    // =========================================================================
    static inline float cutoffTable[1025];
    static inline bool tableInitialized = false;

    static void initCutoffTable()
    {
        if (tableInitialized) return;
        // f = 15.0 * (1066.6666666666667)^(i / 1024.0)
        double ratio = 16000.0 / 15.0; // 1066.6666666666667
        for (int i = 0; i <= 1024; ++i)
        {
            cutoffTable[i] = (float)(15.0 * std::pow(ratio, (double)i / 1024.0));
        }
        tableInitialized = true;
    }

    // Filter States
    float svfS1 = 0.0f;
    float svfS2 = 0.0f;

    float lp24S[4] = { 0.0f };
    float lp24InPrev = 0.0f;
    float lp24SPrev[3] = { 0.0f };

    double lpdlV[4] = { 0.0 };
    double lpdlVHp = 0.0;

    float limiterEnv = 0.0f;
};

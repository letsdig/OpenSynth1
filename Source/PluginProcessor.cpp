#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>
#include <climits>

namespace {

juce::File getOpenSynth1PreferencesFolder() {
  auto appData = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
  auto prefFolder = appData.getChildFile("OpenSynth1");
  prefFolder.createDirectories();
  return prefFolder;
}

juce::File getOpenSynth1PreferencesFile() {
  return getOpenSynth1PreferencesFolder().getChildFile("openSynth1_preferences.txt");
}

juce::String readSavedBankDirectory() {
  auto prefFile = getOpenSynth1PreferencesFile();
  if (!prefFile.existsAsFile()) {
    return {};
  }

  juce::StringArray lines;
  prefFile.readLines(lines);
  for (const auto &line : lines) {
    auto trimmed = line.trim();
    if (trimmed.startsWith("soundbankDirectory=")) {
      return trimmed.fromFirstOccurrenceOf("soundbankDirectory=", false, false).trim();
    }
  }

  return {};
}

void writeSavedBankDirectory(const juce::File &dir) {
  auto prefFile = getOpenSynth1PreferencesFile();
  auto prefFolder = prefFile.getParentDirectory();
  prefFolder.createDirectories();

  juce::String text = "soundbankDirectory=" + dir.getFullPathName() + "\n";
  prefFile.replaceWithText(text);
}

} // namespace

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout layout;

  // OSCILLATORS
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "0", "Osc1 Shape", juce::StringArray{"Sine", "Saw", "Pulse", "Triangle"},
      1));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("45", "Osc1 FM", 0, 127, 0));
  layout.add(std::make_unique<juce::AudioParameterInt>("76", "Osc1 Detune", 0,
                                                       127, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "1", "Osc2 Shape",
      juce::StringArray{"Off", "Saw", "Pulse", "Triangle", "Noise"}, 1));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("2", "Osc2 Pitch", 0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "108", "Osc2 Pitch On/Off", juce::StringArray{"Off", "On"}, 1));
  layout.add(std::make_unique<juce::AudioParameterInt>("3", "Osc2 Fine Tune", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "109", "Osc2 Fine On/Off", juce::StringArray{"Off", "On"}, 1));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "4", "Osc2 Kbd Track", juce::StringArray{"Off", "On"}, 1));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("5", "Osc Mix", 0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "6", "Osc2 Sync", juce::StringArray{"Off", "On"}, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "7", "Osc2 Ring Mod", juce::StringArray{"Off", "On"}, 0));
  layout.add(std::make_unique<juce::AudioParameterInt>("8", "Osc Pulse Width",
                                                       0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("9", "Osc Key Shift",
                                                       -24, 24, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "10", "Osc Mod Env On/Off", juce::StringArray{"Off", "On"}, 0));
  layout.add(std::make_unique<juce::AudioParameterInt>("11", "Osc Mod Env Amt",
                                                       0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>(
      "12", "Osc Mod Env Attack", 0, 127, 0));
  layout.add(std::make_unique<juce::AudioParameterInt>(
      "13", "Osc Mod Env Decay", 0, 127, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "71", "Osc Mod Dest", juce::StringArray{"osc2", "FM", "p/w"}, 0));
  layout.add(std::make_unique<juce::AudioParameterInt>("72", "Osc1,2 Fine Tune",
                                                       0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("91", "Osc Phase Shift",
                                                       0, 127, 0));
  layout.add(std::make_unique<juce::AudioParameterInt>("95", "Osc1 Sub Gain", 0,
                                                       127, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "96", "Osc1 Sub Shape",
      juce::StringArray{"Sine", "Triangle", "Saw", "Pulse"}, 1));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "97", "Osc1 Sub Octave", juce::StringArray{"0oct", "-1oct"}, 1));

  // FILTER
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "14", "Filter Type",
      juce::StringArray{"LP12", "LP24", "HP12", "BP12", "LPDL"}, 1));
  layout.add(std::make_unique<juce::AudioParameterInt>("15", "Filter Attack", 0,
                                                       127, 0));
  layout.add(std::make_unique<juce::AudioParameterInt>("16", "Filter Decay", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("17", "Filter Sustain",
                                                       0, 127, 32));
  layout.add(std::make_unique<juce::AudioParameterInt>("18", "Filter Release",
                                                       0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("19", "Filter Freq", 0,
                                                       127, 81));
  layout.add(std::make_unique<juce::AudioParameterInt>("20", "Filter Resonance",
                                                       0, 127, 14));
  layout.add(std::make_unique<juce::AudioParameterInt>("21", "Filter Amount", 0,
                                                       127, 127));
  layout.add(std::make_unique<juce::AudioParameterInt>(
      "22", "Filter Kbd Track", 0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>(
      "23", "Filter Saturation", 0, 127, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "24", "Filter Vel Switch", juce::StringArray{"Off", "On"}, 1));

  // AMPLIFIER
  layout.add(std::make_unique<juce::AudioParameterInt>("25", "Amp Attack", 0,
                                                       127, 64));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("26", "Amp Decay", 0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("27", "Amp Sustain", 0,
                                                       127, 107));
  layout.add(std::make_unique<juce::AudioParameterInt>("28", "Amp Release", 0,
                                                       127, 64));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("29", "Amp Gain", 0, 127, 107));
  layout.add(std::make_unique<juce::AudioParameterInt>("30", "Amp Vel Sens", 0,
                                                       127, 64));

  // ARPEGGIATOR
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "59", "Arp On/Off", juce::StringArray{"Off", "On"}, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "31", "Arp Type", juce::StringArray{"Up/Down", "Up", "Down", "Random"},
      0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "32", "Arp Range", juce::StringArray{"1oct", "2oct", "3oct", "4oct"}, 0));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("33", "Arp Beat", 0, 18, 11));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("34", "Arp Gate", 0, 127, 64));

  // TEMPO DELAY
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "65", "Delay On/Off", juce::StringArray{"Off", "On"}, 1));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "82", "Delay Type", juce::StringArray{"ST", "X", "PP"}, 0));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("35", "Delay Time", 0, 19, 8));
  layout.add(std::make_unique<juce::AudioParameterInt>("83", "Delay Spread", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("36", "Delay Feedback",
                                                       0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("98", "Delay Tone", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("37", "Delay Dry/Wet", 0,
                                                       127, 64));

  // CHORUS / FLANGER
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "66", "Chorus On/Off", juce::StringArray{"Off", "On"}, 1));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "64", "Chorus Type", juce::StringArray{"x1", "x2", "flg", "x4"}, 1));
  layout.add(std::make_unique<juce::AudioParameterInt>("52", "Chorus Time", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("53", "Chorus Depth", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("54", "Chorus Rate", 0,
                                                       127, 50));
  layout.add(std::make_unique<juce::AudioParameterInt>("55", "Chorus Feedback",
                                                       0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("56", "Chorus Level", 0,
                                                       127, 40));

  // EQUALIZER / PAN
  layout.add(
      std::make_unique<juce::AudioParameterInt>("60", "EQ Tone", 0, 127, 64));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("61", "EQ Freq", 0, 127, 64));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("62", "EQ Level", 0, 127, 64));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("63", "EQ Q", 0, 127, 64));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("90", "Pan", 0, 127, 64));

  // EFFECT
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "77", "Effect On/Off", juce::StringArray{"Off", "On"}, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "78", "Effect Type",
      juce::StringArray{"a.d.1", "a.d.2", "d.d.", "deci.", "r.m.", "comp.",
                        "ph.1", "ph.2", "ph.3", "ph.4"},
      0));
  layout.add(std::make_unique<juce::AudioParameterInt>("79", "Effect Ctrl1", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("80", "Effect Ctrl2", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("81", "Effect Level", 0,
                                                       127, 64));

  // VOICE & PLAY MODE
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "38", "Play Mode", juce::StringArray{"Poly", "Mono", "Legato"}, 0));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("94", "Polyphony", 1, 32, 16));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "73", "Unison Mode", juce::StringArray{"Off", "On"}, 0));
  layout.add(std::make_unique<juce::AudioParameterInt>("93", "Unison Voices", 2,
                                                       32, 2));
  layout.add(std::make_unique<juce::AudioParameterInt>("75", "Unison Detune", 0,
                                                       127, 22));
  layout.add(std::make_unique<juce::AudioParameterInt>(
      "84", "Unison Pan Spread", 0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("85", "Unison Pitch", 0,
                                                       48, 24));
  layout.add(std::make_unique<juce::AudioParameterInt>(
      "92", "Unison Phase Shift", 0, 127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("39", "Portamento Time",
                                                       0, 127, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "74", "Portamento Auto", juce::StringArray{"Off", "On"}, 0));

  // WHEEL / MIDI
  layout.add(std::make_unique<juce::AudioParameterInt>("40", "PitchBend Range",
                                                       0, 24, 12));
  layout.add(std::make_unique<juce::AudioParameterInt>("86", "MIDI Ctrl Src 1",
                                                       0, 127, 1));
  layout.add(std::make_unique<juce::AudioParameterInt>("50", "MIDI Ctrl Sens 1",
                                                       0, 127, 20));
  layout.add(std::make_unique<juce::AudioParameterInt>(
      "87", "MIDI Ctrl Assign 1", -1, 98, 44));
  layout.add(std::make_unique<juce::AudioParameterInt>("88", "MIDI Ctrl Src 2",
                                                       0, 127, 2));
  layout.add(std::make_unique<juce::AudioParameterInt>("51", "MIDI Ctrl Sens 2",
                                                       0, 127, 20));
  layout.add(std::make_unique<juce::AudioParameterInt>(
      "89", "MIDI Ctrl Assign 2", -1, 98, 43));

  // LFO 1 & 2
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "57", "LFO1 On/Off", juce::StringArray{"Off", "On"}, 1));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "41", "LFO1 Dest",
      juce::StringArray{"osc2", "osc1,2", "filter", "amp", "p/w", "FM", "pan"},
      2));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "42", "LFO1 Type",
      juce::StringArray{"Saw", "Triangle", "Square", "Random", "Smoothed",
                        "Sine"},
      1));
  layout.add(std::make_unique<juce::AudioParameterInt>("43", "LFO1 Speed", 0,
                                                       127, 64));
  layout.add(
      std::make_unique<juce::AudioParameterInt>("44", "LFO1 Depth", 0, 127, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "67", "LFO1 Tempo Sync", juce::StringArray{"Off", "On"}, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "68", "LFO1 Key Sync", juce::StringArray{"Off", "On"}, 0));

  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "58", "LFO2 On/Off", juce::StringArray{"Off", "On"}, 1));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "46", "LFO2 Dest",
      juce::StringArray{"osc2", "osc1,2", "filter", "amp", "p/w", "FM", "pan"},
      5));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "47", "LFO2 Type",
      juce::StringArray{"Saw", "Triangle", "Square", "Random", "Smoothed",
                        "Sine"},
      1));
  layout.add(std::make_unique<juce::AudioParameterInt>("48", "LFO2 Speed", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterInt>("49", "LFO2 Depth", 0,
                                                       127, 64));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "69", "LFO2 Tempo Sync", juce::StringArray{"Off", "On"}, 0));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "70", "LFO2 Key Sync", juce::StringArray{"Off", "On"}, 0));

  // MASTER VOLUME
  layout.add(std::make_unique<juce::AudioParameterInt>("107", "Master Vol", 0,
                                                       127, 100));

  return layout;
}

static const int s_synth1Osc2PitchSemitones[128] = {
    -60, -60, -59, -58, -57, -56, -55, -54, -53, -52, -51, -50, -49, -48, -47,
    -46, -45, -44, -43, -43, -42, -41, -40, -39, -38, -37, -36, -35, -34, -33,
    -32, -31, -30, -29, -28, -27, -26, -26, -25, -24, -23, -22, -21, -20, -19,
    -18, -17, -16, -15, -14, -13, -12, -11, -10, -9,  -9,  -8,  -7,  -6,  -5,
    -4,  -3,  -2,  -1,  0,   1,   2,   3,   4,   5,   6,   7,   8,   8,   9,
    10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
    25,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
    39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,
    53,  54,  55,  56,  57,  58,  59,  60};

// PolyBLEP antialiasing correction function for pristine, alias-free waveforms
inline float polyBlep(float t, float dt) {
  if (dt <= 0.0f)
    return 0.0f;
  if (t < dt) {
    t /= dt;
    return t + t - t * t - 1.0f;
  } else if (t > 1.0f - dt) {
    t = (t - 1.0f) / dt;
    return t * t + t + t + 1.0f;
  }
  return 0.0f;
}

// -------------------------------------------------------------
// Synth1Voice Implementation
// -------------------------------------------------------------
Synth1Voice::Synth1Voice(juce::AudioProcessorValueTreeState &apvts,
                         int voiceIdx, std::atomic<uint32_t> &voiceMask,
                         std::atomic<float> &hostBpmRef)
    : voiceIndex(voiceIdx), activeVoicesMask(voiceMask), hostBpm(hostBpmRef) {
  osc1Shape = apvts.getRawParameterValue("0");
  osc1FM = apvts.getRawParameterValue("45");
  osc1Detune = apvts.getRawParameterValue("76");
  osc2Shape = apvts.getRawParameterValue("1");
  osc2Pitch = apvts.getRawParameterValue("2");
  osc2PitchOn = apvts.getRawParameterValue("108");
  osc2Fine = apvts.getRawParameterValue("3");
  osc2FineOn = apvts.getRawParameterValue("109");
  osc2KbdTrack = apvts.getRawParameterValue("4");
  oscMix = apvts.getRawParameterValue("5");
  osc2Sync = apvts.getRawParameterValue("6");
  osc2Ring = apvts.getRawParameterValue("7");
  oscPulseWidth = apvts.getRawParameterValue("8");
  oscKeyShift = apvts.getRawParameterValue("9");
  oscModEnvOn = apvts.getRawParameterValue("10");
  oscModEnvAmt = apvts.getRawParameterValue("11");
  oscModEnvAttack = apvts.getRawParameterValue("12");
  oscModEnvDecay = apvts.getRawParameterValue("13");
  oscModDest = apvts.getRawParameterValue("71");
  masterFineTune = apvts.getRawParameterValue("72");
  oscPhaseShift = apvts.getRawParameterValue("91");
  oscSubGain = apvts.getRawParameterValue("95");
  oscSubShape = apvts.getRawParameterValue("96");
  oscSubOctave = apvts.getRawParameterValue("97");

  filterType = apvts.getRawParameterValue("14");
  filterAttack = apvts.getRawParameterValue("15");
  filterDecay = apvts.getRawParameterValue("16");
  filterSustain = apvts.getRawParameterValue("17");
  filterRelease = apvts.getRawParameterValue("18");

  osc1PhaseA = 0.0f;
  osc1PhaseB = 0.0f;
  osc2Phase = 0.0f;
  subPhase = 0.0f;
  lfo1Phase = 0.0f;
  lfo2Phase = 0.0f;
  for (int v = 0; v < 32; ++v) {
    uOsc1Phase[v] = 0.0f;
    uOsc2Phase[v] = 0.0f;
  }
  for (int d = 0; d < 8; ++d) {
    osc1DetunePhases[d] = 0.0f;
    subDetunePhases[d] = 0.0f;
  }
  filterCutoff = apvts.getRawParameterValue("19");
  filterRes = apvts.getRawParameterValue("20");
  filterAmt = apvts.getRawParameterValue("21");
  filterTrack = apvts.getRawParameterValue("22");
  filterSat = apvts.getRawParameterValue("23");
  filterVel = apvts.getRawParameterValue("24");

  ampAttack = apvts.getRawParameterValue("25");
  ampDecay = apvts.getRawParameterValue("26");
  ampSustain = apvts.getRawParameterValue("27");
  ampRelease = apvts.getRawParameterValue("28");
  ampGain = apvts.getRawParameterValue("29");
  ampVel = apvts.getRawParameterValue("30");
  lfo1On = apvts.getRawParameterValue("57");
  lfo1Dest = apvts.getRawParameterValue("41");
  lfo1Type = apvts.getRawParameterValue("42");
  lfo1Speed = apvts.getRawParameterValue("43");
  lfo1Depth = apvts.getRawParameterValue("44");
  lfo1Tempo = apvts.getRawParameterValue("67");
  lfo1Key = apvts.getRawParameterValue("68");

  lfo2On = apvts.getRawParameterValue("58");
  lfo2Dest = apvts.getRawParameterValue("46");
  lfo2Type = apvts.getRawParameterValue("47");
  lfo2Speed = apvts.getRawParameterValue("48");
  lfo2Depth = apvts.getRawParameterValue("49");
  lfo2Tempo = apvts.getRawParameterValue("69");
  lfo2Key = apvts.getRawParameterValue("70");

  playMode = apvts.getRawParameterValue("38");
  polyphonyParam = apvts.getRawParameterValue("94");
  portamentoTime = apvts.getRawParameterValue("39");
  portamentoAuto = apvts.getRawParameterValue("74");
  pbRange = apvts.getRawParameterValue("40");

  unisonOn = apvts.getRawParameterValue("73");
  unisonVoices = apvts.getRawParameterValue("93");
  unisonDetune = apvts.getRawParameterValue("75");
  unisonSpread = apvts.getRawParameterValue("84");
  unisonPitch = apvts.getRawParameterValue("85");
  unisonPhaseShift = apvts.getRawParameterValue("92");

  midiCtrlSrc1 = apvts.getRawParameterValue("86");
  midiCtrlSens1 = apvts.getRawParameterValue("50");
  midiCtrlAssign1 = apvts.getRawParameterValue("87");
  midiCtrlSrc2 = apvts.getRawParameterValue("88");
  midiCtrlSens2 = apvts.getRawParameterValue("51");
  midiCtrlAssign2 = apvts.getRawParameterValue("89");
}

bool Synth1Voice::canPlaySound(juce::SynthesiserSound *sound) {
  if (dynamic_cast<Synth1Sound *>(sound) == nullptr)
    return false;
  int mode = (playMode != nullptr) ? (int)playMode->load() : 0;
  if (mode > 0 && voiceIndex > 0)
    return false;
  int poly = (polyphonyParam != nullptr) ? (int)polyphonyParam->load() : 16;
  if (poly < 1)
    poly = 1;
  return voiceIndex < poly;
}

void Synth1Voice::resetVoiceState() {
  filterL.reset();
  filterR.reset();
  ampAdsr.reset();
  filterAdsr.reset();
  modEnvActive = false;
  modEnvPhase = 0.0f;
  currentGlideFreq = targetNoteFreq;
  for (int d = 0; d < 8; ++d) {
    osc1DetunePhases[d] = 0.0f;
    subDetunePhases[d] = 0.0f;
  }
  activeVoicesMask.fetch_and(~(1u << (voiceIndex % 32)));
  clearCurrentNote();
}

void Synth1Voice::updateLegatoNote(int newMidiNoteNumber, float newVelocity) {
  int keyShift = (oscKeyShift != nullptr) ? (int)oscKeyShift->load() : 0;
  currentMidiNote = juce::jlimit(0, 127, newMidiNoteNumber + keyShift);
  targetNoteFreq =
      (float)juce::MidiMessage::getMidiNoteInHertz(currentMidiNote);
  currentNoteVelocity = newVelocity;

  float velSens = (ampVel != nullptr) ? (ampVel->load() / 127.0f) : 0.5f;
  level = (1.0f - velSens) + newVelocity * velSens;

  float porta =
      (portamentoTime != nullptr) ? (portamentoTime->load() / 127.0f) : 0.0f;
  if (porta <= 0.01f) {
    currentGlideFreq = targetNoteFreq;
  }
}

void Synth1Voice::retriggerMonoNote(int newMidiNoteNumber, float newVelocity) {
  float sRate = (float)getSampleRate();
  if (sRate <= 0.0f)
    sRate = 44100.0f;

  ampAdsr.setSampleRate(sRate);
  filterAdsr.setSampleRate(sRate);
  updateEnvelopes();

  int keyShift = (oscKeyShift != nullptr) ? (int)oscKeyShift->load() : 0;
  currentMidiNote = juce::jlimit(0, 127, newMidiNoteNumber + keyShift);
  targetNoteFreq =
      (float)juce::MidiMessage::getMidiNoteInHertz(currentMidiNote);
  currentNoteVelocity = newVelocity;

  float velSens = (ampVel != nullptr) ? (ampVel->load() / 127.0f) : 0.5f;
  level = (1.0f - velSens) + newVelocity * velSens;

  ampAdsr.noteOn();
  filterAdsr.noteOn();
  modEnvPhase = 0.0f;
  modEnvActive = true;

  float porta =
      (portamentoTime != nullptr) ? (portamentoTime->load() / 127.0f) : 0.0f;
  if (porta <= 0.01f) {
    currentGlideFreq = targetNoteFreq;
  }
}

void Synth1Voice::startNote(int midiNoteNumber, float velocity,
                            juce::SynthesiserSound *,
                            int currentPitchWheelPosition) {
  activeVoicesMask.fetch_or(1u << (voiceIndex % 32));

  float sRate = (float)getSampleRate();
  if (sRate <= 0.0f)
    sRate = 44100.0f;

  ampAdsr.setSampleRate(sRate);
  filterAdsr.setSampleRate(sRate);
  updateEnvelopes();

  int mode = (playMode != nullptr) ? (int)playMode->load() : 0;
  bool isLegato = (mode == 2);
  bool wasPlaying = isVoiceActive() && ampAdsr.isActive();

  if (!isLegato || !wasPlaying) {
    ampAdsr.noteOn();
    filterAdsr.noteOn();
    modEnvPhase = 0.0f;
    modEnvActive = true;

    float pShiftVal = (oscPhaseShift != nullptr) ? oscPhaseShift->load() : 0.0f;
    float initSpread = pShiftVal / 127.0f;
    osc1PhaseA = 0.0f;
    osc1PhaseB = 0.0f;
    osc2Phase = 0.0f;
    subPhase = 0.0f;
    for (int d = 0; d < 8; ++d) {
      osc1DetunePhases[d] = (float)d / 8.0f * initSpread;
      subDetunePhases[d] = (float)d / 8.0f * initSpread;
    }
    for (int v = 0; v < 32; ++v) {
      uOsc1Phase[v] = (float)v / 32.0f * initSpread;
      uOsc2Phase[v] = std::fmod(uOsc1Phase[v] + 0.125f * initSpread, 1.0f);
    }
  }

  currentNoteVelocity = velocity;
  float velSens = (ampVel != nullptr) ? (ampVel->load() / 127.0f) : 0.5f;
  level = (1.0f - velSens) + velocity * velSens;

  int keyShift = (oscKeyShift != nullptr) ? (int)oscKeyShift->load() : 0;
  currentMidiNote = juce::jlimit(0, 127, midiNoteNumber + keyShift);
  targetNoteFreq =
      (float)juce::MidiMessage::getMidiNoteInHertz(currentMidiNote);

  float porta =
      (portamentoTime != nullptr) ? (portamentoTime->load() / 127.0f) : 0.0f;
  bool autoPorta = (portamentoAuto != nullptr && portamentoAuto->load() > 0.5f);

  if (porta <= 0.01f || currentGlideFreq <= 0.0f ||
      (autoPorta && !wasPlaying)) {
    currentGlideFreq = targetNoteFreq;
  }

  pitchWheelMoved(currentPitchWheelPosition);

  if (lfo1Key != nullptr && lfo1Key->load() > 0.5f) {
    lfo1Phase = 0.0f;
    vLfo1RandCurr = random.nextFloat() * 2.0f - 1.0f;
    vLfo1RandNext = random.nextFloat() * 2.0f - 1.0f;
  }
  if (lfo2Key != nullptr && lfo2Key->load() > 0.5f) {
    lfo2Phase = 0.0f;
    vLfo2RandCurr = random.nextFloat() * 2.0f - 1.0f;
    vLfo2RandNext = random.nextFloat() * 2.0f - 1.0f;
  }

  if (!wasPlaying) {
    filterL.reset();
    filterR.reset();
  }
}

void Synth1Voice::stopNote(float /*velocity*/, bool allowTailOff) {
  ampAdsr.noteOff();
  filterAdsr.noteOff();

  if (!allowTailOff) {
    activeVoicesMask.fetch_and(~(1u << (voiceIndex % 32)));
    filterL.reset();
    filterR.reset();
    clearCurrentNote();
  }
}

void Synth1Voice::pitchWheelMoved(int newPitchWheelValue) {
  float range = (pbRange != nullptr) ? pbRange->load() : 12.0f;
  pitchWheelBend = ((float)(newPitchWheelValue - 8192) / 8192.0f) * range;
}

void Synth1Voice::controllerMoved(int controllerNumber,
                                  int newControllerValue) {
  int src1 = (midiCtrlSrc1 != nullptr) ? (int)midiCtrlSrc1->load() : 1;
  int src2 = (midiCtrlSrc2 != nullptr) ? (int)midiCtrlSrc2->load() : 2;

  if (controllerNumber == src1)
    midiCtrl1Value = (float)newControllerValue / 127.0f;
  if (controllerNumber == src2)
    midiCtrl2Value = (float)newControllerValue / 127.0f;
}

float Synth1Voice::getOsc1Sample(float phase, float dt, int shape, float pw, bool antiAlias) {
  switch (shape) {
  case 0:
    return std::sin(phase * juce::MathConstants<float>::twoPi);
  case 1: {
    // Sawtooth with Phase Distortion (Synth1 Color parameter)
    // In Synth1, a Color of 64 (pw=0.5) is a standard sawtooth.
    // Values above 0.5 squash the sawtooth (sync effect).
    float fraction = 1.0f;
    if (pw > 0.5f) {
       fraction = 1.0f - (pw - 0.5f) * 1.8f; // Squashes down to 0.1
    }
    float safePw = juce::jlimit(0.05f, 1.0f, fraction);
    float val = -1.0f;
    if (phase < safePw) {
      val = 2.0f * (phase / safePw) - 1.0f;
    }
    if (antiAlias) {
      val -= polyBlep(phase, dt);
      if (safePw < 1.0f) {
          val += polyBlep(std::fmod(phase + 1.0f - safePw, 1.0f), dt);
      }
    }
    return val;
  }
  case 2: {
    float val = (phase < pw) ? 1.0f : -1.0f;
    if (antiAlias) {
      val += polyBlep(phase, dt);
      val -= polyBlep(std::fmod(phase + 1.0f - pw, 1.0f), dt);
    }
    return val;
  }
  case 3: {
    // Triangle with Phase Distortion (Spike morphing)
    float safePw = juce::jlimit(0.01f, 0.99f, pw);
    if (phase < safePw) {
       return 2.0f * (phase / safePw) - 1.0f;
    } else {
       return 1.0f - 2.0f * ((phase - safePw) / (1.0f - safePw));
    }
  }
  default:
    return 0.0f;
  }
}

float Synth1Voice::getOsc2Sample(float phase, float dt, int shape, float pw, bool antiAlias) {
  switch (shape) {
  case 0:
    return 0.0f;
  case 1: {
    float fraction = 1.0f;
    if (pw > 0.5f) {
       fraction = 1.0f - (pw - 0.5f) * 1.8f;
    }
    float safePw = juce::jlimit(0.05f, 1.0f, fraction);
    float val = -1.0f;
    if (phase < safePw) {
      val = 2.0f * (phase / safePw) - 1.0f;
    }
    if (antiAlias) {
      val -= polyBlep(phase, dt);
      if (safePw < 1.0f) {
          val += polyBlep(std::fmod(phase + 1.0f - safePw, 1.0f), dt);
      }
    }
    return val;
  }
  case 2: {
    float val = (phase < pw) ? 1.0f : -1.0f;
    if (antiAlias) {
      val += polyBlep(phase, dt);
      val -= polyBlep(std::fmod(phase + 1.0f - pw, 1.0f), dt);
    }
    return val;
  }
  case 3: {
    float safePw = juce::jlimit(0.01f, 0.99f, pw);
    if (phase < safePw) {
       return 2.0f * (phase / safePw) - 1.0f;
    } else {
       return 1.0f - 2.0f * ((phase - safePw) / (1.0f - safePw));
    }
  }
  case 4:
    return (random.nextFloat() * 2.0f) - 1.0f;
  default:
    return 0.0f;
  }
}

float Synth1Voice::getSubOscSample(float phase, int shape) {
  switch (shape) {
  case 0:
    return std::sin(phase * juce::MathConstants<float>::twoPi);
  case 1:
    return 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
  case 2:
    return 1.0f - 2.0f * phase;
  case 3:
    return (phase < 0.5f) ? 1.0f : -1.0f;
  default:
    return 0.0f;
  }
}

float Synth1Voice::getLfoSample(float phase, int shape, float rCurr, float rNext) {
  switch (shape) {
  case 0:
    return 1.0f - 2.0f * phase;
  case 1:
    return 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
  case 2:
    return (phase < 0.5f) ? 1.0f : -1.0f;
  case 3:
    return rCurr;
  case 4: {
    // Random (Smoothed): smooth cosine interpolation between consecutive random targets
    float s = 0.5f - 0.5f * std::cos(phase * juce::MathConstants<float>::pi);
    return rCurr + (rNext - rCurr) * s;
  }
  case 5:
    return std::sin(phase * juce::MathConstants<float>::twoPi);
  default:
    return std::sin(phase * juce::MathConstants<float>::twoPi);
  }
}

void Synth1Voice::updateEnvelopes() {
  // Curva ADSR identica al Synth1 originale:
  //   0   -> ~1ms (istantaneo)
  //   64  -> ~200ms (valore medio)
  //   127 -> ~6s (massimo, come NL2)
  // Split: sotto 85/127 usa curva rapida (0-1s), sopra usa curva lenta (1-6s)
  auto mapTime = [](float p) -> float {
    float norm = juce::jlimit(0.0f, 127.0f, p) / 127.0f;
    if (norm < 0.001f)
      return 0.0001f; // ~0.1ms minimo
    if (norm <= 0.5f) {
      float t = norm / 0.5f;
      return 0.0001f * std::pow(10000.0f, t); // 0.1ms -> 1s
    } else {
      float t = (norm - 0.5f) / 0.5f;
      return 1.0f * std::pow(15.0f, t); // 1s -> 15s
    }
  };

  auto mapSustainLevel = [](float p) -> float {
    if (p <= 0.0f)
      return 0.0f;
    float norm = juce::jlimit(0.0f, 127.0f, p) / 127.0f;
    return std::pow(norm, 1.5f);
  };

  if (ampAttack && ampDecay && ampSustain && ampRelease) {
    float att = mapTime(ampAttack->load());
    float dec = mapTime(ampDecay->load());
    float sus = mapSustainLevel(ampSustain->load());
    float rel = mapTime(ampRelease->load());
    ampAdsr.setParameters(att, dec, sus, rel);
  }

  if (filterAttack && filterDecay && filterSustain && filterRelease) {
    float att = mapTime(filterAttack->load());
    float dec = mapTime(filterDecay->load());
    float sus = mapSustainLevel(filterSustain->load());
    float rel = mapTime(filterRelease->load());
    filterAdsr.setParameters(att, dec, sus, rel);
  }
}

void Synth1Voice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer,
                                  int startSample, int numSamples) {
  if (!isVoiceActive()) {
    activeVoicesMask.fetch_and(~(1u << (voiceIndex % 32)));
    return;
  }

  updateEnvelopes();

  float sampleRate = (float)getSampleRate();
  if (sampleRate <= 0.0f)
    sampleRate = 44100.0f;

  float portaSpeed =
      (portamentoTime != nullptr) ? (portamentoTime->load() / 127.0f) : 0.0f;
  float glideCoeff =
      (portaSpeed > 0.01f)
          ? (1.0f - std::exp(-1.0f / (portaSpeed * 0.5f * sampleRate)))
          : 1.0f;

  float basePw =
      (oscPulseWidth != nullptr) ? (oscPulseWidth->load() / 127.0f) : 0.5f;
  basePw = juce::jlimit(0.05f, 0.95f, basePw);

  bool pOn = (osc2PitchOn == nullptr || osc2PitchOn->load() > 0.5f);
  int pVal = (osc2Pitch != nullptr)
                 ? juce::jlimit(0, 127, (int)std::round(osc2Pitch->load()))
                 : 64;
  float pitchSemi = pOn ? (float)s_synth1Osc2PitchSemitones[pVal] : 0.0f;

  bool fOn = (osc2FineOn == nullptr || osc2FineOn->load() > 0.5f);
  float pitchFine = (fOn && osc2Fine != nullptr)
                        ? ((osc2Fine->load() - 64.0f) * 0.96875f / 100.0f)
                        : 0.0f;
  bool kbdTrack2 =
      (osc2KbdTrack != nullptr) ? (osc2KbdTrack->load() > 0.5f) : true;

  float mix = (oscMix != nullptr) ? (oscMix->load() / 127.0f) : 0.5f;
  bool syncOn = (osc2Sync != nullptr) ? (osc2Sync->load() > 0.5f) : false;
  bool ringOn = (osc2Ring != nullptr) ? (osc2Ring->load() > 0.5f) : false;
  float fmAmount = (osc1FM != nullptr) ? (osc1FM->load() / 127.0f) : 0.0f;

  bool modEnvOn =
      (oscModEnvOn != nullptr) ? (oscModEnvOn->load() > 0.5f) : false;
  float modEnvAmtVal = (oscModEnvAmt != nullptr)
                           ? ((oscModEnvAmt->load() - 64.0f) / 64.0f)
                           : 0.0f;
  int modDest = (oscModDest != nullptr) ? (int)oscModDest->load() : 0;

  float subGainVal =
      (oscSubGain != nullptr) ? (oscSubGain->load() / 127.0f) : 0.0f;
  int subShapeVal = (oscSubShape != nullptr) ? (int)oscSubShape->load() : 1;
  int subOctVal = (oscSubOctave != nullptr) ? (int)oscSubOctave->load() : 1;

  int fType = (filterType != nullptr) ? (int)filterType->load() : 1;
  float resVal = (filterRes != nullptr) ? (filterRes->load() / 127.0f) : 0.1f;
  float rawFAmt = (filterAmt != nullptr) ? filterAmt->load() : 127.0f;
  float fAmtVal = (rawFAmt >= 63.0f) ? ((rawFAmt - 63.0f) / 64.0f)
                                     : ((rawFAmt - 63.0f) / 63.0f);

  int shape1 = (osc1Shape != nullptr) ? (int)osc1Shape->load() : 1;
  int shape2 = (osc2Shape != nullptr) ? (int)osc2Shape->load() : 0;

  bool lfo1Active = (lfo1On != nullptr && lfo1On->load() > 0.5f);
  int lfo1Shape = (lfo1Type != nullptr) ? (int)lfo1Type->load() : 1;
  float lfo1SpdNorm =
      (lfo1Speed != nullptr) ? (lfo1Speed->load() / 127.0f) : 0.5f;
  bool lfo1Sync = (lfo1Tempo != nullptr && lfo1Tempo->load() > 0.5f);
  float lfo1Hz = 0.1f * std::pow(300.0f, lfo1SpdNorm);
  if (lfo1Sync) {
    static const float lfoSyncBeats[] = {
        64.0f, 32.0f,   16.0f, 8.0f, 4.0f,    3.0f,   2.0f,  1.3333f, 1.5f,
        1.0f,  0.6667f, 0.75f, 0.5f, 0.3333f, 0.375f, 0.25f, 0.125f};
    int divIdx = juce::jlimit(0, 16, (int)std::round(lfo1SpdNorm * 16.0f));
    float beatSec = 60.0f / hostBpm.load();
    float cycleSec = beatSec * lfoSyncBeats[divIdx];
    if (cycleSec > 0.001f)
      lfo1Hz = 1.0f / cycleSec;
  }
  float lfo1DepthNorm =
      (lfo1Depth != nullptr) ? (lfo1Depth->load() / 127.0f) : 0.0f;
  int lfo1Dst = (lfo1Dest != nullptr) ? (int)lfo1Dest->load() : 2;
  float lfo1Delta = lfo1Hz / sampleRate;

  bool lfo2Active = (lfo2On != nullptr && lfo2On->load() > 0.5f);
  int lfo2Shape = (lfo2Type != nullptr) ? (int)lfo2Type->load() : 1;
  float lfo2SpdNorm =
      (lfo2Speed != nullptr) ? (lfo2Speed->load() / 127.0f) : 0.5f;
  bool lfo2Sync = (lfo2Tempo != nullptr && lfo2Tempo->load() > 0.5f);
  float lfo2Hz = 0.1f * std::pow(300.0f, lfo2SpdNorm);
  if (lfo2Sync) {
    static const float lfoSyncBeats[] = {
        64.0f, 32.0f,   16.0f, 8.0f, 4.0f,    3.0f,   2.0f,  1.3333f, 1.5f,
        1.0f,  0.6667f, 0.75f, 0.5f, 0.3333f, 0.375f, 0.25f, 0.125f};
    int divIdx = juce::jlimit(0, 16, (int)std::round(lfo2SpdNorm * 16.0f));
    float beatSec = 60.0f / hostBpm.load();
    float cycleSec = beatSec * lfoSyncBeats[divIdx];
    if (cycleSec > 0.001f)
      lfo2Hz = 1.0f / cycleSec;
  }
  float lfo2DepthNorm =
      (lfo2Depth != nullptr) ? (lfo2Depth->load() / 127.0f) : 0.0f;
  int lfo2Dst = (lfo2Dest != nullptr) ? (int)lfo2Dest->load() : 5;
  float lfo2Delta = lfo2Hz / sampleRate;

  bool isUnison = (unisonOn != nullptr && unisonOn->load() > 0.5f);
  int uVoices = (unisonVoices != nullptr)
                    ? juce::jlimit(2, 32, (int)unisonVoices->load())
                    : 2;
  float uDetuneVal = (unisonDetune != nullptr)
                         ? (unisonDetune->load() / 127.0f * 85.0f)
                         : 15.0f;
  float uSpreadVal =
      (unisonSpread != nullptr) ? (unisonSpread->load() / 127.0f) : 0.8f;
  float uPitchVal = (unisonPitch != nullptr)
                        ? (unisonPitch->load() - 24.0f)
                        : 0.0f;

  float uDetFactors[32];
  float uPanCos[32];
  float uPanSin[32];
  if (isUnison) {
    for (int v = 0; v < uVoices; ++v) {
      float vSpread = (uVoices > 1)
                          ? ((float)v / (float)(uVoices - 1) - 0.5f) * 2.0f
                          : 0.0f;
      uDetFactors[v] = std::pow(2.0f, (vSpread * uDetuneVal) / 1200.0f +
                                          (vSpread * uPitchVal) / 12.0f);
      float vPan = juce::jlimit(0.0f, 1.0f, 0.5f + vSpread * uSpreadVal * 0.5f);
      uPanCos[v] = std::cos(vPan * juce::MathConstants<float>::halfPi);
      uPanSin[v] = std::sin(vPan * juce::MathConstants<float>::halfPi);
    }
  }

  float masterFineCents =
      (masterFineTune != nullptr)
          ? ((masterFineTune->load() - 64.0f) * (100.0f / 64.0f))
          : masterTuneCents;
  float subDetFactor = (subOctVal == 1 ? 0.5f : 1.0f);

  float baseDetVal = (osc1Detune != nullptr) ? osc1Detune->load() : 0.0f;
  float osc1DetFactors[7];
  static const float kOddMultipliers[7] = {-3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f};
  if (baseDetVal > 0.5f && (shape1 == 1 || shape1 == 2)) {
    float detStepCents = (baseDetVal / 127.0f) * (80.0f / 3.0f);
    for (int d = 0; d < 7; ++d) {
      osc1DetFactors[d] =
          std::pow(2.0f, (kOddMultipliers[d] * detStepCents) / 1200.0f);
    }
  }

  bool lfo1KeySync = (lfo1Key != nullptr && lfo1Key->load() > 0.5f);
  bool lfo2KeySync = (lfo2Key != nullptr && lfo2Key->load() > 0.5f);
  float runLfo1Phase = lfo1Phase;
  float runLfo2Phase = lfo2Phase;

  for (int i = startSample; i < startSample + numSamples; ++i) {
    currentGlideFreq += (targetNoteFreq - currentGlideFreq) * glideCoeff;

    float lfo1Val = 0.0f;
    if (lfo1Active) {
      if (lfo1KeySync) {
        lfo1Val = getLfoSample(runLfo1Phase, lfo1Shape, vLfo1RandCurr, vLfo1RandNext) * lfo1DepthNorm;
        runLfo1Phase += lfo1Delta;
        if (runLfo1Phase >= 1.0f) {
          runLfo1Phase -= 1.0f;
          vLfo1RandCurr = vLfo1RandNext;
          vLfo1RandNext = random.nextFloat() * 2.0f - 1.0f;
        }
      } else {
        float curGlobalPhase1 = std::fmod(globalLfo1Phase + (float)i * lfo1Delta, 1.0f);
        if (curGlobalPhase1 < 0.0f) curGlobalPhase1 += 1.0f;
        lfo1Val = getLfoSample(curGlobalPhase1, lfo1Shape, globalLfo1RandCurr, globalLfo1RandNext) * lfo1DepthNorm;
      }
    }

    float lfo2Val = 0.0f;
    if (lfo2Active) {
      if (lfo2KeySync) {
        lfo2Val = getLfoSample(runLfo2Phase, lfo2Shape, vLfo2RandCurr, vLfo2RandNext) * lfo2DepthNorm;
        runLfo2Phase += lfo2Delta;
        if (runLfo2Phase >= 1.0f) {
          runLfo2Phase -= 1.0f;
          vLfo2RandCurr = vLfo2RandNext;
          vLfo2RandNext = random.nextFloat() * 2.0f - 1.0f;
        }
      } else {
        float curGlobalPhase2 = std::fmod(globalLfo2Phase + (float)i * lfo2Delta, 1.0f);
        if (curGlobalPhase2 < 0.0f) curGlobalPhase2 += 1.0f;
        lfo2Val = getLfoSample(curGlobalPhase2, lfo2Shape, globalLfo2RandCurr, globalLfo2RandNext) * lfo2DepthNorm;
      }
    }

    float lfoPitchMod2 = 0.0f;
    float lfoPitchMod12 = 0.0f;
    float lfoFilterMod = 0.0f;
    float lfoAmpMod = 1.0f;
    float lfoPwMod = 0.0f;
    float lfoFmMod = 0.0f;
    float lfoPanMod = 0.0f;

    auto applyLfo = [&](int dst, float val) {
      switch (dst) {
      case 0:
        lfoPitchMod2 += val * 12.0f;
        break;
      case 1:
        lfoPitchMod12 += val * 2.0f;
        break;
      case 2:
        lfoFilterMod += val;
        break;
      case 3:
        lfoAmpMod *= std::max(0.0f, 1.0f + val * 0.45f);
        break;
      case 4:
        lfoPwMod += val * 0.4f;
        break;
      case 5:
        lfoFmMod += val * 0.5f;
        break;
      case 6:
        lfoPanMod += val * 0.5f;
        break;
      }
    };
    if (lfo1Active)
      applyLfo(lfo1Dst, lfo1Val);
    if (lfo2Active)
      applyLfo(lfo2Dst, lfo2Val);

    float detVal = baseDetVal;

    auto applyMidiCtrl = [&](int assignParamId, float amount) {
      switch (assignParamId) {
      case 44:
        lfo1DepthNorm = juce::jlimit(0.0f, 1.0f, lfo1DepthNorm + amount);
        break;
      case 43:
        lfo1Delta *= std::pow(2.0f, amount * 2.0f);
        break;
      case 49:
        lfo2DepthNorm = juce::jlimit(0.0f, 1.0f, lfo2DepthNorm + amount);
        break;
      case 48:
        lfo2Delta *= std::pow(2.0f, amount * 2.0f);
        break;
      case 19:
        lfoFilterMod += amount * 0.5f;
        break;
      case 20:
        resVal = juce::jlimit(0.0f, 1.0f, resVal + amount);
        break;
      case 21:
        fAmtVal = juce::jlimit(-1.0f, 1.0f, fAmtVal + amount);
        break;
      case 29:
        lfoAmpMod *= std::max(0.0f, 1.0f + amount);
        break;
      case 2:
        lfoPitchMod2 += amount * 12.0f;
        break;
      case 45:
        lfoFmMod += amount * 0.5f;
        break;
      case 8:
        lfoPwMod += amount * 0.4f;
        break;
      case 90:
        lfoPanMod += amount * 0.5f;
        break;
      case 76:
        detVal = juce::jlimit(0.0f, 127.0f, detVal + amount * 64.0f);
        break;
      default:
        break;
      }
    };

    if (midiCtrlSrc1 != nullptr && midiCtrlAssign1 != nullptr &&
        midiCtrlSens1 != nullptr) {
      float sens1 = (midiCtrlSens1->load() - 64.0f) / 64.0f;
      int a1 = (int)midiCtrlAssign1->load();
      applyMidiCtrl(a1, midiCtrl1Value * sens1);
    }
    if (midiCtrlSrc2 != nullptr && midiCtrlAssign2 != nullptr &&
        midiCtrlSens2 != nullptr) {
      float sens2 = (midiCtrlSens2->load() - 64.0f) / 64.0f;
      int a2 = (int)midiCtrlAssign2->load();
      applyMidiCtrl(a2, midiCtrl2Value * sens2);
    }

    float baseFreq = currentGlideFreq *
                     std::pow(2.0f, (pitchWheelBend + lfoPitchMod12 +
                                     (masterFineCents / 100.0f)) /
                                        12.0f);

    float modEnvOut = 0.0f;
    if (modEnvOn) {
      float rawAtt =
          (oscModEnvAttack != nullptr) ? oscModEnvAttack->load() : 0.0f;
      float rawDec =
          (oscModEnvDecay != nullptr) ? oscModEnvDecay->load() : 0.0f;

      float attTime =
          0.001f *
          std::pow(12000.0f, juce::jlimit(0.0f, 127.0f, rawAtt) / 127.0f);
      float decTime =
          0.001f *
          std::pow(12000.0f, juce::jlimit(0.0f, 127.0f, rawDec) / 127.0f);

      if (modEnvPhase < attTime) {
        modEnvOut = modEnvPhase / attTime;
      } else {
        float tDecay = modEnvPhase - attTime;
        modEnvOut = std::exp(-4.605f * tDecay / decTime);
      }
      modEnvPhase += 1.0f / sampleRate;
    }

    float osc2PitchMod =
        (modDest == 0 && modEnvOn) ? (modEnvOut * modEnvAmtVal * 24.0f) : 0.0f;
    float fmMod =
        (modDest == 1 && modEnvOn) ? (modEnvOut * modEnvAmtVal) : 0.0f;
    float pwMod =
        (modDest == 2 && modEnvOn) ? (modEnvOut * modEnvAmtVal * 0.4f) : 0.0f;

    float totalPwMod = pwMod + lfoPwMod;
    float effectivePw = basePw;
    if (totalPwMod > 0.0f)
      effectivePw = basePw + totalPwMod * (0.95f - basePw);
    else
      effectivePw = basePw + totalPwMod * (basePw - 0.05f);
    effectivePw = juce::jlimit(0.05f, 0.95f, effectivePw);

    float effectiveFm = std::max(0.0f, fmAmount + fmMod + lfoFmMod);

    float freq1 = baseFreq;
    float fixedBaseFreq =
        261.6255f * std::pow(2.0f, (pitchWheelBend + lfoPitchMod12 +
                                    (masterFineCents / 100.0f)) /
                                       12.0f);
    float freq2 =
        kbdTrack2
            ? (baseFreq * std::pow(2.0f, (pitchSemi + pitchFine + osc2PitchMod +
                                          lfoPitchMod2) /
                                             12.0f))
            : (fixedBaseFreq * std::pow(2.0f, (pitchSemi + pitchFine +
                                               osc2PitchMod + lfoPitchMod2) /
                                                  12.0f));

    float dt2 = freq2 / sampleRate;
    float osc2Sample = getOsc2Sample(osc2Phase, dt2, shape2, effectivePw);

    // Phase Modulation (Stile DX7/Synth1)
    float fmModAmt = std::pow(effectiveFm, 1.2f) * 9.5f;

    if (syncOn && (osc1PhaseA + (freq1 / sampleRate) >= 1.0f)) {
      osc2Phase = 0.0f;
    }

    float osc1Sample = 0.0f;
    float subSample = 0.0f;

    if (detVal > 0.5f && (shape1 == 1 || shape1 == 2)) {
      float s1Sum = 0.0f;
      for (int d = 0; d < 7; ++d) {
        float dtD = (freq1 * osc1DetFactors[d]) / sampleRate;
        float readPhase = std::fmod(
            std::fmod(osc1DetunePhases[d] + fmModAmt * osc2Sample, 1.0f) + 1.0f,
            1.0f);
        float weight = (d == 3) ? 1.0f : 0.65f;
        s1Sum += getOsc1Sample(readPhase, dtD, shape1, effectivePw) * weight;
        osc1DetunePhases[d] += dtD;
        if (osc1DetunePhases[d] >= 1.0f)
          osc1DetunePhases[d] -= std::floor(osc1DetunePhases[d]);
        if (osc1DetunePhases[d] < 0.0f)
          osc1DetunePhases[d] += 1.0f;
      }
      osc1Sample = s1Sum * 0.40f;
    } else {
      float dt1 = freq1 / sampleRate;
      float readPhase = std::fmod(
          std::fmod(osc1PhaseA + fmModAmt * osc2Sample, 1.0f) + 1.0f, 1.0f);
      osc1Sample = getOsc1Sample(readPhase, dt1, shape1, effectivePw);

      osc1PhaseA += dt1;
      if (osc1PhaseA >= 1.0f)
        osc1PhaseA -= std::floor(osc1PhaseA);
      if (osc1PhaseA < 0.0f)
        osc1PhaseA += 1.0f;
    }

    if (subGainVal > 0.001f) {
      float subDelta = (freq1 * subDetFactor) / sampleRate;
      float subReadPhase = std::fmod(
          std::fmod(subPhase + fmModAmt * osc2Sample, 1.0f) + 1.0f, 1.0f);
      subSample = getSubOscSample(subReadPhase, subShapeVal);
      subPhase += subDelta;
      if (subPhase >= 1.0f)
        subPhase -= 1.0f;
      if (subPhase < 0.0f)
        subPhase += 1.0f;
    }

    osc2Phase += dt2;
    if (osc2Phase >= 1.0f)
      osc2Phase -= std::floor(osc2Phase);

    float osc1Gain = 1.0f / (1.0f + subGainVal);
    float osc1WithSub =
        osc1Sample * osc1Gain + subSample * subGainVal * osc1Gain;
    float osc2Out = ringOn ? (osc1Sample * osc2Sample) : osc2Sample;
    float oscOut = osc1WithSub * (1.0f - mix) + osc2Out * mix;

    float oscOutL = oscOut;
    float oscOutR = oscOut;
    if (isUnison) {
      float uSumL = 0.0f, uSumR = 0.0f;
      for (int v = 0; v < uVoices; ++v) {
        float uFreq1 = baseFreq * uDetFactors[v];
        float uFixedBaseFreq = fixedBaseFreq * uDetFactors[v];
        float uFreq2 =
            kbdTrack2 ? (uFreq1 * std::pow(2.0f, (pitchSemi + pitchFine +
                                                  osc2PitchMod + lfoPitchMod2) /
                                                     12.0f))
                      : (uFixedBaseFreq *
                         std::pow(2.0f, (pitchSemi + pitchFine + osc2PitchMod +
                                         lfoPitchMod2) /
                                            12.0f));

        float uD2 = uFreq2 / sampleRate;
        bool doAntiAlias = (uVoices < 4) || (v != 0 && v != uVoices - 1);
        float uS2 = getOsc2Sample(uOsc2Phase[v], uD2, shape2, effectivePw, doAntiAlias);
        float uFmPhaseOffset = fmModAmt * uS2;
        float uD1 = uFreq1 / sampleRate;

        if (syncOn && (uOsc1Phase[v] + uD1 >= 1.0f))
          uOsc2Phase[v] = 0.0f;
        float uReadPhase = std::fmod(std::fmod(uOsc1Phase[v] + uFmPhaseOffset, 1.0f) + 1.0f, 1.0f);
        float uS1 = getOsc1Sample(uReadPhase, uD1, shape1, effectivePw, doAntiAlias);

        uOsc1Phase[v] += uD1;
        if (uOsc1Phase[v] >= 1.0f)
          uOsc1Phase[v] -= std::floor(uOsc1Phase[v]);
        if (uOsc1Phase[v] < 0.0f)
          uOsc1Phase[v] += 1.0f;

        uOsc2Phase[v] += uD2;
        if (uOsc2Phase[v] >= 1.0f)
          uOsc2Phase[v] -= std::floor(uOsc2Phase[v]);

        float uSample = ringOn ? (uS1 * (1.0f - mix) + (uS1 * uS2) * mix)
                               : (uS1 * (1.0f - mix) + uS2 * mix);
        uSumL += uSample * uPanCos[v];
        uSumR += uSample * uPanSin[v];
      }
      float inv = 1.0f / std::sqrt((float)uVoices);
      oscOutL = uSumL * inv;
      oscOutR = uSumR * inv;
    }

    float inSampleL = oscOutL;
    float inSampleR = oscOutR;

    float rawSat = (filterSat != nullptr) ? filterSat->load() : 0.0f;
    if (rawSat > 0.01f) {
      float k = (rawSat / 127.0f) * 6.0f;
      inSampleL = (1.0f + k) * inSampleL / (1.0f + k * std::abs(inSampleL));
      inSampleR = (1.0f + k) * inSampleR / (1.0f + k * std::abs(inSampleR));
    }

    float fEnv = filterAdsr.getNextSample();
    bool fVelOn = (filterVel != nullptr && filterVel->load() > 0.5f);
    float effectiveFAmt = fVelOn ? (fAmtVal * currentNoteVelocity) : fAmtVal;

    float rawCutoffVal =
        (filterCutoff != nullptr) ? filterCutoff->load() : 81.0f;
    float rawTrackVal = (filterTrack != nullptr) ? filterTrack->load() : 64.0f;
    float currentPitchSemiFromC3 =
        12.0f * std::log2(std::max(10.0f, currentGlideFreq) / 130.81278f) +
        pitchWheelBend;
    int virtualNote = 48 + (int)std::round(currentPitchSemiFromC3);

    float envScale = (fAmtVal != 0.0f) ? (effectiveFAmt / fAmtVal) : 1.0f;
    float cutoffHz = OpenSynthFilter::calculateCutoffHz(
        rawCutoffVal, virtualNote, rawTrackVal, rawFAmt, fEnv * envScale,
        lfoFilterMod);
    cutoffHz = juce::jlimit(15.0f, sampleRate * 0.48f, cutoffHz);

    float filteredL =
        filterL.processSample(inSampleL, cutoffHz, resVal, fType, sampleRate);
    float filteredR = isUnison
        ? filterR.processSample(inSampleR, cutoffHz, resVal, fType, sampleRate)
        : filteredL;

    float ampEnvVal = ampAdsr.getNextSample();

    // --- GAIN TAPER QUADRATICO CORRETTO (+6dB max) ---
    float rawAmpGain = (ampGain != nullptr) ? ampGain->load() : 107.0f;
    float gainVal = 0.0f;
    if (rawAmpGain > 0.001f) {
      float normGain = rawAmpGain / 127.0f;
      gainVal = std::pow(normGain, 2.0f) * 2.0f;
    }

    float voicePan = juce::jlimit(0.0f, 1.0f, 0.5f + lfoPanMod);
    float pL = std::cos(voicePan * juce::MathConstants<float>::halfPi);
    float pR = std::sin(voicePan * juce::MathConstants<float>::halfPi);

    float commonGain = level * ampEnvVal * lfoAmpMod * gainVal * 0.45f;
    float finalSampleL = filteredL * commonGain * pL;
    float finalSampleR = filteredR * commonGain * pR;

    if (outputBuffer.getNumChannels() > 1) {
      outputBuffer.addSample(0, i, finalSampleL);
      outputBuffer.addSample(1, i, finalSampleR);
    } else {
      outputBuffer.addSample(0, i, (finalSampleL + finalSampleR) * 0.5f);
    }
  }

  if (lfo1KeySync)
    lfo1Phase = runLfo1Phase;
  if (lfo2KeySync)
    lfo2Phase = runLfo2Phase;

  if (!ampAdsr.isActive()) {
    activeVoicesMask.fetch_and(~(1u << (voiceIndex % 32)));
    filterL.reset();
    filterR.reset();
    clearCurrentNote();
  }
}

OpenSynth1AudioProcessor::OpenSynth1AudioProcessor()
    : AudioProcessor(BusesProperties().withOutput(
          "Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()) {
  initDefaultMidiCcMap();

  masterVolParam = apvts.getRawParameterValue("107");
  panParam = apvts.getRawParameterValue("90");

  synth.addSound(new Synth1Sound());
  for (int i = 0; i < 32; ++i) {
    synth.addVoice(new Synth1Voice(apvts, i, activeVoicesMask, currentHostBpm));
  }
  synth.setMinimumRenderingSubdivisionSize(16);

  delayBuffer.setSize(2, 96000);
  delayBuffer.clear();

  // Default bundled bank location relative to the executable / project tree.
  juce::File exeDir = juce::File::getSpecialLocation(
      juce::File::currentExecutableFile).getParentDirectory();
  juce::File bundledBank = exeDir.getChildFile("soundbank");

  if (!bundledBank.isDirectory()) {
    bundledBank = juce::File::getCurrentWorkingDirectory().getChildFile("soundbank");
  }

  if (!bundledBank.isDirectory()) {
    bundledBank = juce::File::getSpecialLocation(
        juce::File::userDocumentsDirectory).getChildFile("OpenSynth1/soundbank");
  }

  juce::String savedPath = readSavedBankDirectory();
  juce::File savedBank = juce::File(savedPath);
  if (savedBank.exists() && savedBank.isDirectory()) {
    setBankDirectory(savedBank);
  } else {
    setBankDirectory(bundledBank);
  }
}

OpenSynth1AudioProcessor::~OpenSynth1AudioProcessor() {}

const juce::String OpenSynth1AudioProcessor::getName() const {
  return "OpenSynth1";
}
bool OpenSynth1AudioProcessor::acceptsMidi() const { return true; }
bool OpenSynth1AudioProcessor::producesMidi() const { return false; }
bool OpenSynth1AudioProcessor::isMidiEffect() const { return false; }
double OpenSynth1AudioProcessor::getTailLengthSeconds() const { return 0.0; }
int OpenSynth1AudioProcessor::getNumPrograms() { return 128; }
int OpenSynth1AudioProcessor::getCurrentProgram() {
  return currentProgramIndex - 1;
}
void OpenSynth1AudioProcessor::setCurrentProgram(int index) {
  selectBankAndProgram(currentBankIndex, index + 1);
}
const juce::String OpenSynth1AudioProcessor::getProgramName(int) {
  return currentProgramName;
}
void OpenSynth1AudioProcessor::changeProgramName(int,
                                                 const juce::String &newName) {
  currentProgramName = newName;
}

void OpenSynth1AudioProcessor::prepareToPlay(double sampleRate,
                                             int samplesPerBlock) {
  synth.setCurrentPlaybackSampleRate(sampleRate);
  delayBuffer.setSize(2, (int)(sampleRate * 2.5));
  delayBuffer.clear();
  delayWritePos = 0;

  chorusBuffer.setSize(2, (int)(sampleRate * 0.35));
  chorusBuffer.clear();
  chorusWritePos = 0;
  chorusPhase = 0.0f;

  processorLfo1Phase = 0.0f;
  processorLfo2Phase = 0.0f;
  processorLfo1RandCurr = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
  processorLfo1RandNext = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
  processorLfo2RandCurr = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
  processorLfo2RandNext = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;

  phaserPhase = 0.0f;
  phaserFeedbackL = 0.0f;
  phaserFeedbackR = 0.0f;
  phaserInPrevL.fill(0.0f);
  phaserOutPrevL.fill(0.0f);
  phaserInPrevR.fill(0.0f);
  phaserOutPrevR.fill(0.0f);

  distLpStateL = 0.0f;
  distLpStateR = 0.0f;

  eqX1L = eqX2L = eqY1L = eqY2L = 0.0f;
  eqX1R = eqX2R = eqY1R = eqY2R = 0.0f;

  decimHeldL = decimHeldR = 0.0f;
  decimCounter = 0;
  rmPhase = 0.0f;
  compEnvFollower = 0.0f;

  arpHeldKeys.clear();
  arpPattern.clear();
  arpPatternIndex = 0;
  arpStepCounter = 0.0;
  arpNotePlaying = -1;
  arpGateSamplesRemaining = 0;
}

void OpenSynth1AudioProcessor::releaseResources() {}
bool OpenSynth1AudioProcessor::isBusesLayoutSupported(
    const BusesLayout &) const {
  return true;
}

void OpenSynth1AudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                            juce::MidiBuffer &midiMessages) {
  juce::ScopedNoDenormals noDenormals;

  if (auto *p72 = apvts.getRawParameterValue("72")) {
    masterTuneCents.store((p72->load() - 64.0f) * (100.0f / 64.0f));
  }

  if (resetVoicesPending.exchange(false)) {
    synth.allNotesOff(0, false);
    for (int v = 0; v < synth.getNumVoices(); ++v) {
      if (auto *sv = dynamic_cast<Synth1Voice *>(synth.getVoice(v))) {
        sv->resetVoiceState();
      }
    }
    monoHeldKeys.clear();
    currentMonoSoundingNote = -1;
    delayBuffer.clear();
    chorusBuffer.clear();
    distLpStateL = distLpStateR = 0.0f;
    phaserFeedbackL = phaserFeedbackR = 0.0f;
    phaserInPrevL.fill(0.0f);
    phaserOutPrevL.fill(0.0f);
    phaserInPrevR.fill(0.0f);
    phaserOutPrevR.fill(0.0f);
    eqX1L = eqX2L = eqY1L = eqY2L = 0.0f;
    eqX1R = eqX2R = eqY1R = eqY2R = 0.0f;
  }

  for (const auto metadata : midiMessages) {
    auto msg = metadata.getMessage();
    if (msg.isController()) {
      int cc = msg.getControllerNumber();
      int val = msg.getControllerValue();

      if (midiLearnActive.load()) {
        int targetParam = midiLearnTargetParam.load();
        if (targetParam >= 0) {
          setMidiCcMapping(cc, targetParam);
          lastMidiLearnMessage = "Mapped CC #" + juce::String(cc) + " to ID " +
                                 juce::String(targetParam);
          midiLearnActive = false;
        }
      } else {
        int paramId = getParamForMidiCc(cc);
        if (paramId >= 0) {
          if (auto *p = dynamic_cast<juce::RangedAudioParameter *>(
                  apvts.getParameter(juce::String(paramId)))) {
            float norm = p->getNormalisableRange().convertTo0to1((float)val);
            p->setValue(norm);
          }
        }
      }
    }
  }

  buffer.clear();

  juce::MidiBuffer synthMidi;
  bool arpOn = (apvts.getRawParameterValue("59") != nullptr &&
                apvts.getRawParameterValue("59")->load() > 0.5f);

  if (auto *ph = getPlayHead()) {
    if (auto pos = ph->getPosition()) {
      if (pos->getBpm().hasValue())
        currentHostBpm.store((float)*pos->getBpm());
    }
  }
  float hBpm = currentHostBpm.load();
  if (hBpm < 20.0f || hBpm > 400.0f) {
    hBpm = 120.0f;
    currentHostBpm.store(120.0f);
  }

  if (arpOn) {
    static const double beatDivisions[] = {
        4.0,    3.0,    2.0,    1.3333, 1.5,    1.0,    0.6667, 0.75,   0.5,
        0.3333, 0.375,  0.25,   0.1667, 0.1875, 0.125,  0.0833, 0.0625, 0.04167, 0.03125};

    float beatVal = (apvts.getRawParameterValue("33") != nullptr)
                        ? apvts.getRawParameterValue("33")->load()
                        : 11.0f;
    int divIdx = (beatVal <= 18.0f)
                     ? juce::jlimit(0, 18, (int)std::round(beatVal))
                     : juce::jlimit(0, 18, (int)std::round(beatVal / 127.0f * 18.0f));
    double beatsPerStep = beatDivisions[divIdx];
    arpSamplesPerStep =
        (60.0 / currentHostBpm) * beatsPerStep * getSampleRate();
    if (arpSamplesPerStep < 50.0)
      arpSamplesPerStep = 50.0;

    float gateVal = (apvts.getRawParameterValue("34") != nullptr)
                        ? (apvts.getRawParameterValue("34")->load() / 127.0f)
                        : 0.6f;
    gateVal = juce::jlimit(0.05f, 0.95f, gateVal * 0.9f + 0.05f);
    int gateLen = juce::jmax(10, (int)(arpSamplesPerStep * gateVal));

    for (const auto metadata : midiMessages) {
      auto msg = metadata.getMessage();
      if (msg.isNoteOn()) {
        int note = msg.getNoteNumber();
        bool wasEmpty = arpHeldKeys.empty();
        if (std::find(arpHeldKeys.begin(), arpHeldKeys.end(), note) ==
            arpHeldKeys.end()) {
          arpHeldKeys.push_back(note);
          std::sort(arpHeldKeys.begin(), arpHeldKeys.end());
        }
        if (wasEmpty) {
          arpPatternIndex = 0;
          arpStepCounter = arpSamplesPerStep;
        }
      } else if (msg.isNoteOff()) {
        int note = msg.getNoteNumber();
        auto it = std::find(arpHeldKeys.begin(), arpHeldKeys.end(), note);
        if (it != arpHeldKeys.end())
          arpHeldKeys.erase(it);
      } else {
        synthMidi.addEvent(msg, metadata.samplePosition);
      }
    }

    if (arpHeldKeys.empty()) {
      if (arpNotePlaying >= 0) {
        synthMidi.addEvent(juce::MidiMessage::noteOff(1, arpNotePlaying), 0);
        arpNotePlaying = -1;
      }
      arpPattern.clear();
      arpPatternIndex = 0;
      arpStepCounter = 999999.0;
      arpGateSamplesRemaining = 0;
    } else {
      int octRange =
          (apvts.getRawParameterValue("32") != nullptr)
              ? juce::jlimit(0, 3,
                             (int)apvts.getRawParameterValue("32")->load())
              : 0;
      int arpType =
          (apvts.getRawParameterValue("31") != nullptr)
              ? juce::jlimit(0, 3,
                             (int)apvts.getRawParameterValue("31")->load())
              : 0;

      std::vector<int> allNotes;
      for (int oct = 0; oct <= octRange; ++oct) {
        for (int n : arpHeldKeys) {
          int pitch = n + oct * 12;
          if (pitch <= 127)
            allNotes.push_back(pitch);
        }
      }

      arpPattern.clear();
      if (arpType == 0) {
        for (int n : allNotes)
          arpPattern.push_back(n);
        for (int idx = (int)allNotes.size() - 2; idx > 0; --idx)
          arpPattern.push_back(allNotes[idx]);
      } else if (arpType == 1) {
        for (int n : allNotes)
          arpPattern.push_back(n);
      } else if (arpType == 2) {
        for (auto it = allNotes.rbegin(); it != allNotes.rend(); ++it)
          arpPattern.push_back(*it);
      } else if (arpType == 3) {
        arpPattern = allNotes;
      }
      if (arpPattern.empty())
        arpPattern = arpHeldKeys;

      for (int smp = 0; smp < buffer.getNumSamples(); ++smp) {
        arpStepCounter += 1.0;
        if (arpStepCounter >= arpSamplesPerStep) {
          arpStepCounter = 0.0;
          if (arpNotePlaying >= 0) {
            synthMidi.addEvent(juce::MidiMessage::noteOff(1, arpNotePlaying),
                               smp);
            arpNotePlaying = -1;
          }

          if (!arpPattern.empty()) {
            int note = 60;
            if (arpType == 3) {
              note = arpPattern[(size_t)juce::Random::getSystemRandom().nextInt(
                  (int)arpPattern.size())];
            } else {
              if (arpPatternIndex >= (int)arpPattern.size())
                arpPatternIndex = 0;
              note = arpPattern[(size_t)arpPatternIndex];
              arpPatternIndex = (arpPatternIndex + 1) % (int)arpPattern.size();
            }

            synthMidi.addEvent(
                juce::MidiMessage::noteOn(1, note, (juce::uint8)100), smp);
            arpNotePlaying = note;
            arpGateSamplesRemaining = gateLen;
          }
        }

        if (arpGateSamplesRemaining > 0) {
          --arpGateSamplesRemaining;
          if (arpGateSamplesRemaining == 0 && arpNotePlaying >= 0) {
            synthMidi.addEvent(juce::MidiMessage::noteOff(1, arpNotePlaying),
                               smp);
            arpNotePlaying = -1;
          }
        }
      }
    }
  } else {
    arpHeldKeys.clear();
    arpNotePlaying = -1;

    int playModeVal = (apvts.getRawParameterValue("38") != nullptr)
                          ? (int)apvts.getRawParameterValue("38")->load()
                          : 0;
    if (playModeVal == 0) {
      synthMidi.addEvents(midiMessages, 0, buffer.getNumSamples(), 0);
      monoHeldKeys.clear();
      currentMonoSoundingNote = -1;
    } else {
      for (const auto metadata : midiMessages) {
        auto msg = metadata.getMessage();
        int smp = metadata.samplePosition;

        if (msg.isNoteOn()) {
          int note = msg.getNoteNumber();
          float vel = msg.getFloatVelocity();

          monoHeldKeys.erase(std::remove_if(monoHeldKeys.begin(),
                                            monoHeldKeys.end(),
                                            [note](const MonoHeldNote &n) {
                                              return n.noteNumber == note;
                                            }),
                             monoHeldKeys.end());
          monoHeldKeys.push_back({note, vel});

          if (currentMonoSoundingNote < 0) {
            synthMidi.addEvent(juce::MidiMessage::noteOn(msg.getChannel(), note,
                                                         msg.getVelocity()),
                               smp);
            currentMonoSoundingNote = note;
          } else {
            currentMonoSoundingNote = note;
            if (playModeVal == 1) {
              if (auto *v0 = dynamic_cast<Synth1Voice *>(synth.getVoice(0)))
                v0->retriggerMonoNote(note, vel);
            } else {
              if (auto *v0 = dynamic_cast<Synth1Voice *>(synth.getVoice(0)))
                v0->updateLegatoNote(note, vel);
            }
          }
        } else if (msg.isNoteOff()) {
          int note = msg.getNoteNumber();
          monoHeldKeys.erase(std::remove_if(monoHeldKeys.begin(),
                                            monoHeldKeys.end(),
                                            [note](const MonoHeldNote &n) {
                                              return n.noteNumber == note;
                                            }),
                             monoHeldKeys.end());

          if (note == currentMonoSoundingNote) {
            if (monoHeldKeys.empty()) {
              synthMidi.addEvent(juce::MidiMessage::noteOff(
                                     msg.getChannel(), note, msg.getVelocity()),
                                 smp);
              currentMonoSoundingNote = -1;
            } else {
              auto prev = monoHeldKeys.back();
              currentMonoSoundingNote = prev.noteNumber;
              if (playModeVal == 1) {
                if (auto *v0 = dynamic_cast<Synth1Voice *>(synth.getVoice(0)))
                  v0->retriggerMonoNote(prev.noteNumber, prev.velocity);
              } else {
                if (auto *v0 = dynamic_cast<Synth1Voice *>(synth.getVoice(0)))
                  v0->updateLegatoNote(prev.noteNumber, prev.velocity);
              }
            }
          }
        } else if (msg.isAllNotesOff() || msg.isAllSoundOff()) {
          monoHeldKeys.clear();
          currentMonoSoundingNote = -1;
          synthMidi.addEvent(msg, smp);
        } else {
          synthMidi.addEvent(msg, smp);
        }
      }
    }
  }

  float sRate = (float)getSampleRate();
  if (sRate <= 0.0f)
    sRate = 44100.0f;

  float lfo1Spd = (apvts.getRawParameterValue("43") != nullptr)
                      ? (apvts.getRawParameterValue("43")->load() / 127.0f)
                      : 0.5f;
  float lfo2Spd = (apvts.getRawParameterValue("48") != nullptr)
                      ? (apvts.getRawParameterValue("48")->load() / 127.0f)
                      : 0.5f;
  bool lfo1Sync = (apvts.getRawParameterValue("67") != nullptr &&
                   apvts.getRawParameterValue("67")->load() > 0.5f);
  bool lfo2Sync = (apvts.getRawParameterValue("69") != nullptr &&
                   apvts.getRawParameterValue("69")->load() > 0.5f);

  static const float lfoSyncBeats[] = {
      64.0f, 32.0f,   16.0f, 8.0f, 4.0f,    3.0f,   2.0f,  1.3333f, 1.5f,
      1.0f,  0.6667f, 0.75f, 0.5f, 0.3333f, 0.375f, 0.25f, 0.125f};
  float lfo1Hz = 0.1f * std::pow(300.0f, lfo1Spd);
  if (lfo1Sync) {
    int divIdx = juce::jlimit(0, 16, (int)std::round(lfo1Spd * 16.0f));
    float beatSec = 60.0f / hBpm;
    float cycleSec = beatSec * lfoSyncBeats[divIdx];
    if (cycleSec > 0.001f)
      lfo1Hz = 1.0f / cycleSec;
  }
  float lfo2Hz = 0.1f * std::pow(300.0f, lfo2Spd);
  if (lfo2Sync) {
    int divIdx = juce::jlimit(0, 16, (int)std::round(lfo2Spd * 16.0f));
    float beatSec = 60.0f / hBpm;
    float cycleSec = beatSec * lfoSyncBeats[divIdx];
    if (cycleSec > 0.001f)
      lfo2Hz = 1.0f / cycleSec;
  }

  for (int v = 0; v < synth.getNumVoices(); ++v) {
    if (auto *sv = dynamic_cast<Synth1Voice *>(synth.getVoice(v))) {
      sv->setGlobalLfoState(processorLfo1Phase, processorLfo2Phase,
                            processorLfo1RandCurr, processorLfo1RandNext,
                            processorLfo2RandCurr, processorLfo2RandNext);
    }
  }

  synth.renderNextBlock(buffer, synthMidi, 0, buffer.getNumSamples());

  float lfo1BlockDelta = (lfo1Hz / sRate) * (float)buffer.getNumSamples();
  float lfo2BlockDelta = (lfo2Hz / sRate) * (float)buffer.getNumSamples();
  processorLfo1Phase += lfo1BlockDelta;
  while (processorLfo1Phase >= 1.0f) {
    processorLfo1Phase -= 1.0f;
    processorLfo1RandCurr = processorLfo1RandNext;
    processorLfo1RandNext = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
  }
  processorLfo2Phase += lfo2BlockDelta;
  while (processorLfo2Phase >= 1.0f) {
    processorLfo2Phase -= 1.0f;
    processorLfo2RandCurr = processorLfo2RandNext;
    processorLfo2RandNext = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
  }

  bool effectOn = (apvts.getRawParameterValue("77") != nullptr &&
                   apvts.getRawParameterValue("77")->load() > 0.5f);
  int fxType = (apvts.getRawParameterValue("78") != nullptr)
                   ? (int)apvts.getRawParameterValue("78")->load()
                   : 0;
  float fxCtl1 = (apvts.getRawParameterValue("79") != nullptr)
                     ? (apvts.getRawParameterValue("79")->load() / 127.0f)
                     : 0.5f;
  float fxCtl2 = (apvts.getRawParameterValue("80") != nullptr)
                     ? (apvts.getRawParameterValue("80")->load() / 127.0f)
                     : 0.5f;
  float fxLevel = (apvts.getRawParameterValue("81") != nullptr)
                      ? (apvts.getRawParameterValue("81")->load() / 127.0f)
                      : 0.5f;

  float eqToneVal =
      (apvts.getRawParameterValue("60") != nullptr)
          ? ((apvts.getRawParameterValue("60")->load() - 64.0f) / 64.0f)
          : 0.0f;
  float eqFreqVal = (apvts.getRawParameterValue("61") != nullptr)
                        ? (apvts.getRawParameterValue("61")->load() / 127.0f)
                        : 0.5f;
  float eqGainVal =
      (apvts.getRawParameterValue("62") != nullptr)
          ? ((apvts.getRawParameterValue("62")->load() - 64.0f) / 64.0f * 12.0f)
          : 0.0f;
  float eqQVal =
      (apvts.getRawParameterValue("63") != nullptr)
          ? (0.5f + apvts.getRawParameterValue("63")->load() / 127.0f * 4.0f)
          : 1.0f;

  float eqF = 100.0f * std::pow(100.0f, eqFreqVal);
  float omega = 2.0f * juce::MathConstants<float>::pi * eqF / sRate;
  float sn = std::sin(omega);
  float cs = std::cos(omega);
  float A = std::pow(10.0f, eqGainVal / 40.0f);
  float alpha = sn / (2.0f * eqQVal);
  float a0 = 1.0f + alpha / A;
  float b0 = (1.0f + alpha * A) / a0;
  float b1 = (-2.0f * cs) / a0;
  float b2 = (1.0f - alpha * A) / a0;
  float a1 = (-2.0f * cs) / a0;
  float a2 = (1.0f - alpha / A) / a0;

  bool chorusOn = (apvts.getRawParameterValue("66") != nullptr &&
                   apvts.getRawParameterValue("66")->load() > 0.5f);
  int chType = (apvts.getRawParameterValue("64") != nullptr)
                   ? (int)apvts.getRawParameterValue("64")->load()
                   : 1;
  float rawChTime = (apvts.getRawParameterValue("52") != nullptr)
                        ? apvts.getRawParameterValue("52")->load()
                        : 64.0f;
  float rawChDepth = (apvts.getRawParameterValue("53") != nullptr)
                         ? apvts.getRawParameterValue("53")->load()
                         : 64.0f;
  float rawChRate = (apvts.getRawParameterValue("54") != nullptr)
                        ? apvts.getRawParameterValue("54")->load()
                        : 50.0f;
  float rawChFdbk = (apvts.getRawParameterValue("55") != nullptr)
                        ? apvts.getRawParameterValue("55")->load()
                        : 64.0f;
  float rawChLevel = (apvts.getRawParameterValue("56") != nullptr)
                         ? apvts.getRawParameterValue("56")->load()
                         : 40.0f;

  float normChTime = juce::jlimit(0.0f, 1.0f, rawChTime / 127.0f);
  float chBaseSec = 0.0003f + normChTime * normChTime * 0.035f;
  float normChDepth = juce::jlimit(0.0f, 1.0f, rawChDepth / 127.0f);
  float chModSec = 0.0015f + normChDepth * 0.012f;
  float normChRate = juce::jlimit(0.0f, 1.0f, rawChRate / 127.0f);
  float chRateHz = 0.04f * std::pow(350.0f, normChRate);
  float chFdbk = ((rawChFdbk - 64.0f) / 64.0f) * 0.95f;
  float chWetGain = std::pow(juce::jlimit(0.0f, 1.0f, rawChLevel / 127.0f), 0.75f) * 1.5f;

  bool delayOn = (apvts.getRawParameterValue("65") != nullptr &&
                  apvts.getRawParameterValue("65")->load() > 0.5f);
  int delayType = (apvts.getRawParameterValue("82") != nullptr)
                      ? (int)apvts.getRawParameterValue("82")->load()
                      : 0;
  int dIdx = (apvts.getRawParameterValue("35") != nullptr)
                 ? juce::jlimit(0, 19,
                                (int)std::round(
                                    apvts.getRawParameterValue("35")->load()))
                 : 8;
  float delaySprdVal =
      (apvts.getRawParameterValue("83") != nullptr)
          ? ((apvts.getRawParameterValue("83")->load() - 64.0f) / 64.0f)
          : 0.0f;
  float delayFdbkVal =
      (apvts.getRawParameterValue("36") != nullptr)
          ? (apvts.getRawParameterValue("36")->load() / 127.0f * 0.95f)
          : 0.4f;
  float delayToneVal = (apvts.getRawParameterValue("98") != nullptr)
                           ? (apvts.getRawParameterValue("98")->load() / 127.0f)
                           : 0.5f;

  static const float delaySyncBeats[] = {
      0.001f, 0.0833f, 0.125f,  0.1667f, 0.1875f, 0.25f,   0.3333f,
      0.375f, 0.5f,    0.6667f, 0.75f,   1.0f,    1.3333f, 1.5f,
      2.0f,   2.6667f, 3.0f,    4.0f,    8.0f,    16.0f};
  float beatSec = 60.0f / hBpm;
  float dSec = (dIdx == 0) ? 0.001f : beatSec * delaySyncBeats[dIdx];
  float spreadOffsetSec = delaySprdVal * 0.04f;
  int delayLenL =
      juce::jlimit(10, 96000, (int)((dSec - spreadOffsetSec * 0.5f) * sRate));
  int delayLenR =
      juce::jlimit(10, 96000, (int)((dSec + spreadOffsetSec * 0.5f) * sRate));
  int dBufLen = delayBuffer.getNumSamples();
  int cBufLen = chorusBuffer.getNumSamples();

  float dampCutoffHz = 600.0f * std::pow(25.0f, delayToneVal);
  float dampCoef = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi *
                                   dampCutoffHz / sRate);
  dampCoef = juce::jlimit(0.02f, 0.98f, dampCoef);

  for (int i = 0; i < buffer.getNumSamples(); ++i) {
    float left = buffer.getSample(0, i);
    float right = (buffer.getNumChannels() > 1) ? buffer.getSample(1, i) : left;

    if (std::abs(eqGainVal) > 0.01f) {
      float yL = b0 * left + b1 * eqX1L + b2 * eqX2L - a1 * eqY1L - a2 * eqY2L;
      eqX2L = eqX1L;
      eqX1L = left;
      eqY2L = eqY1L;
      eqY1L = yL;
      left = yL;

      float yR = b0 * right + b1 * eqX1R + b2 * eqX2R - a1 * eqY1R - a2 * eqY2R;
      eqX2R = eqX1R;
      eqX1R = right;
      eqY2R = eqY1R;
      eqY1R = yR;
      right = yR;
    }

    if (effectOn) {
      float dryL = left, dryR = right;
      float wetL = left, wetR = right;

      switch (fxType) {
      case 0: { // a.d.1: Analog Distortion 1 (Warm overdrive with tone filter)
        float drive = 1.0f + std::pow(fxCtl1, 2.0f) * 45.0f;
        float satL = std::tanh(left * drive) * (1.0f + 0.25f * fxCtl1);
        float satR = std::tanh(right * drive) * (1.0f + 0.25f * fxCtl1);

        float toneCutoff = 500.0f * std::pow(36.0f, fxCtl2);
        float toneAlpha = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * toneCutoff / sRate);
        distLpStateL += toneAlpha * (satL - distLpStateL);
        distLpStateR += toneAlpha * (satR - distLpStateR);
        wetL = distLpStateL;
        wetR = distLpStateR;
        break;
      }
      case 1: { // a.d.2: Analog Distortion 2 (Asymmetric tube saturation with tone filter)
        float drive = 1.0f + std::pow(fxCtl1, 2.0f) * 60.0f;
        float xL = left * drive;
        float satL = (xL > 0.0f) ? std::tanh(xL) : (xL / (1.0f + 0.8f * std::abs(xL)));
        float xR = right * drive;
        float satR = (xR > 0.0f) ? std::tanh(xR) : (xR / (1.0f + 0.8f * std::abs(xR)));

        float toneCutoff = 500.0f * std::pow(36.0f, fxCtl2);
        float toneAlpha = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * toneCutoff / sRate);
        distLpStateL += toneAlpha * (satL - distLpStateL);
        distLpStateR += toneAlpha * (satR - distLpStateR);
        wetL = distLpStateL;
        wetR = distLpStateR;
        break;
      }
      case 2: { // d.d.: Digital Distortion (Wavefolding / hard clip)
        float drive = 1.0f + std::pow(fxCtl1, 2.2f) * 70.0f;
        auto fold = [](float x) -> float {
          if (x > 1.2f) return 2.4f - x;
          if (x < -1.2f) return -2.4f - x;
          return std::sin(x * 1.30899f);
        };
        float satL = fold(left * drive) * 0.9f;
        float satR = fold(right * drive) * 0.9f;

        float toneCutoff = 800.0f * std::pow(25.0f, fxCtl2);
        float toneAlpha = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * toneCutoff / sRate);
        distLpStateL += toneAlpha * (satL - distLpStateL);
        distLpStateR += toneAlpha * (satR - distLpStateR);
        wetL = distLpStateL;
        wetR = distLpStateR;
        break;
      }
      case 3: { // deci.: Decimator (Sample rate & bit reducer)
        int decHold = 1 + (int)(std::pow(fxCtl1, 2.0f) * 60.0f);
        if (++decimCounter >= decHold) {
          decimCounter = 0;
          decimHeldL = left;
          decimHeldR = right;
        }
        float bits = std::pow(2.0f, 2.0f + (1.0f - fxCtl2) * 12.0f);
        wetL = std::round(decimHeldL * bits) / bits;
        wetR = std::round(decimHeldR * bits) / bits;
        break;
      }
      case 4: { // r.m.: Ring Modulator
        float rmFreq = 15.0f * std::pow(300.0f, fxCtl1);
        rmPhase += rmFreq * juce::MathConstants<float>::twoPi / sRate;
        if (rmPhase >= juce::MathConstants<float>::twoPi)
          rmPhase -= juce::MathConstants<float>::twoPi;
        float carrier = std::sin(rmPhase);
        if (fxCtl2 > 0.01f)
          carrier = (1.0f - fxCtl2) * carrier + fxCtl2 * (carrier > 0.0f ? 1.0f : -1.0f);
        wetL = left * carrier;
        wetR = right * carrier;
        break;
      }
      case 5: { // comp.: Compressor / Limiter
        float envIn = std::max(std::abs(left), std::abs(right));
        float attTime = 0.002f;
        float relTime = 0.02f + (1.0f - fxCtl2) * 0.3f;
        float coef = (envIn > compEnvFollower)
                         ? (1.0f - std::exp(-1.0f / (attTime * sRate)))
                         : (1.0f - std::exp(-1.0f / (relTime * sRate)));
        compEnvFollower += coef * (envIn - compEnvFollower);
        float thresh = 0.7f - fxCtl1 * 0.6f;
        float gain = (compEnvFollower > thresh && thresh > 0.001f)
                         ? std::pow(thresh / compEnvFollower, 0.75f)
                         : 1.0f;
        float makeup = 1.0f + fxCtl1 * 1.5f;
        wetL = left * gain * makeup;
        wetR = right * gain * makeup;
        break;
      }
      case 6:
      case 7:
      case 8:
      case 9: { // ph.1, ph.2, ph.3, ph.4: Multi-stage stereo Allpass Phasers
        float phRate = 0.05f * std::pow(100.0f, fxCtl1);
        phaserPhase += phRate * juce::MathConstants<float>::twoPi / sRate;
        if (phaserPhase >= juce::MathConstants<float>::twoPi)
          phaserPhase -= juce::MathConstants<float>::twoPi;

        float baseFreq = 200.0f * std::pow(25.0f, fxCtl2);
        float modL = 0.5f * (1.0f + std::sin(phaserPhase));
        float modR = 0.5f * (1.0f + std::cos(phaserPhase));
        float fL = juce::jlimit(40.0f, sRate * 0.45f, baseFreq * std::pow(8.0f, modL - 0.5f));
        float fR = juce::jlimit(40.0f, sRate * 0.45f, baseFreq * std::pow(8.0f, modR - 0.5f));

        float wL = std::tan(juce::MathConstants<float>::pi * fL / sRate);
        float aL = (wL - 1.0f) / (wL + 1.0f);
        float wR = std::tan(juce::MathConstants<float>::pi * fR / sRate);
        float aR = (wR - 1.0f) / (wR + 1.0f);

        int numStages = (fxType == 6 ? 2 : (fxType == 7 ? 4 : (fxType == 8 ? 6 : 8)));
        float fb = fxLevel * 0.90f;
        float inL = left + phaserFeedbackL * fb;
        float inR = right + phaserFeedbackR * fb;

        for (int st = 0; st < numStages; ++st) {
          float yL = aL * inL + phaserInPrevL[st] - aL * phaserOutPrevL[st];
          phaserInPrevL[st] = inL;
          phaserOutPrevL[st] = yL;
          inL = yL;

          float yR = aR * inR + phaserInPrevR[st] - aR * phaserOutPrevR[st];
          phaserInPrevR[st] = inR;
          phaserOutPrevR[st] = yR;
          inR = yR;
        }
        phaserFeedbackL = juce::jlimit(-2.5f, 2.5f, inL);
        phaserFeedbackR = juce::jlimit(-2.5f, 2.5f, inR);

        // 50/50 dry/allpass mix creates total destructive cancellation notches
        wetL = 0.5f * (left + inL);
        wetR = 0.5f * (right + inR);
        break;
      }
      }

      left = dryL * (1.0f - fxLevel) + wetL * fxLevel;
      right = dryR * (1.0f - fxLevel) + wetR * fxLevel;
    }

    if (delayOn && dBufLen > 0) {
      int clampedLenL = juce::jlimit(1, dBufLen - 1, delayLenL);
      int clampedLenR = juce::jlimit(1, dBufLen - 1, delayLenR);
      int readL = ((delayWritePos - clampedLenL) % dBufLen + dBufLen) % dBufLen;
      int readR = ((delayWritePos - clampedLenR) % dBufLen + dBufLen) % dBufLen;

      float dl = delayBuffer.getSample(0, readL);
      float dr = delayBuffer.getSample(1, readR);
      if (!std::isfinite(dl))
        dl = 0.0f;
      if (!std::isfinite(dr))
        dr = 0.0f;

      delayFilterStateL =
          delayFilterStateL + dampCoef * (dl - delayFilterStateL);
      delayFilterStateR =
          delayFilterStateR + dampCoef * (dr - delayFilterStateR);
      float fdl = delayFilterStateL;
      float fdr = delayFilterStateR;

      if (delayType == 1) {
        delayBuffer.setSample(
            0, delayWritePos,
            juce::jlimit(-3.0f, 3.0f, left + fdr * delayFdbkVal));
        delayBuffer.setSample(
            1, delayWritePos,
            juce::jlimit(-3.0f, 3.0f, right + fdl * delayFdbkVal));
      } else if (delayType == 2) {
        delayBuffer.setSample(
            0, delayWritePos,
            juce::jlimit(-3.0f, 3.0f, left + fdr * delayFdbkVal));
        delayBuffer.setSample(1, delayWritePos,
                              juce::jlimit(-3.0f, 3.0f, fdl * delayFdbkVal));
      } else {
        delayBuffer.setSample(
            0, delayWritePos,
            juce::jlimit(-3.0f, 3.0f, left + fdl * delayFdbkVal));
        delayBuffer.setSample(
            1, delayWritePos,
            juce::jlimit(-3.0f, 3.0f, right + fdr * delayFdbkVal));
      }

      float rawDw = (apvts.getRawParameterValue("37") != nullptr)
                        ? apvts.getRawParameterValue("37")->load()
                        : 38.0f;
      float delayDry = (rawDw <= 64.0f) ? 1.0f : ((127.0f - rawDw) / 63.0f);
      float delayWet =
          ((rawDw <= 64.0f) ? (std::max(0.0f, rawDw - 1.0f) / 63.0f) : 1.0f) *
          0.75f;

      left = left * delayDry + fdl * delayWet;
      right = right * delayDry + fdr * delayWet;

      delayWritePos = (delayWritePos + 1) % dBufLen;
    }

    if (chorusOn && cBufLen > 0) {
      chorusPhase += chRateHz * juce::MathConstants<float>::twoPi / sRate;
      if (chorusPhase >= juce::MathConstants<float>::twoPi)
        chorusPhase -= juce::MathConstants<float>::twoPi;

      auto readChorusInterp = [&](int channel, float delaySmp) -> float {
        float readPos = (float)chorusWritePos - delaySmp;
        while (readPos < 0.0f)
          readPos += (float)cBufLen;
        while (readPos >= (float)cBufLen)
          readPos -= (float)cBufLen;
        int idx0 = (int)readPos;
        int idx1 = (idx0 + 1) % cBufLen;
        float frac = readPos - (float)idx0;
        return (1.0f - frac) * chorusBuffer.getSample(channel, idx0) +
               frac * chorusBuffer.getSample(channel, idx1);
      };

      float chOutL = 0.0f, chOutR = 0.0f;

      if (chType == 0) {
        // x1: Single delay line modulated
        float mod = std::sin(chorusPhase);
        float dSmp = juce::jlimit(2.0f, (float)(cBufLen - 2),
                                  (chBaseSec + mod * chModSec) * sRate);
        chOutL = readChorusInterp(0, dSmp);
        chOutR = chOutL;
      } else if (chType == 1) {
        // x2: Stereo quadrature (90 deg phase offset between L and R)
        float modL = std::sin(chorusPhase);
        float modR = std::cos(chorusPhase);

        float dSmpL = juce::jlimit(2.0f, (float)(cBufLen - 2),
                                   (chBaseSec + modL * chModSec) * sRate);
        float dSmpR = juce::jlimit(2.0f, (float)(cBufLen - 2),
                                   (chBaseSec + modR * chModSec) * sRate);
        chOutL = readChorusInterp(0, dSmpL);
        chOutR = readChorusInterp(1, dSmpR);
      } else if (chType == 2) {
        // flg: Flanger (tight delay with anti-phase LFO for stereo comb sweep)
        float flgBaseSec = 0.0003f + normChTime * normChTime * 0.007f;
        float flgModSec = normChDepth * flgBaseSec * 0.90f;
        float modL = std::sin(chorusPhase);
        float modR = -std::sin(chorusPhase); // 180 deg anti-phase for wide comb sweep
        float dSmpL = juce::jlimit(1.0f, (float)(cBufLen - 2),
                                   (flgBaseSec + modL * flgModSec) * sRate);
        float dSmpR = juce::jlimit(1.0f, (float)(cBufLen - 2),
                                   (flgBaseSec + modR * flgModSec) * sRate);
        chOutL = readChorusInterp(0, dSmpL);
        chOutR = readChorusInterp(1, dSmpR);
      } else {
        // x4: 4-tap ensemble chorus distributed in phase and delay time
        float mod1 = std::sin(chorusPhase);
        float mod2 = std::cos(chorusPhase);
        float mod3 = -std::sin(chorusPhase);
        float mod4 = -std::cos(chorusPhase);

        float v1 = readChorusInterp(
            0, juce::jlimit(2.0f, (float)(cBufLen - 2),
                            (chBaseSec * 0.85f + mod1 * chModSec) * sRate));
        float v2 = readChorusInterp(
            1, juce::jlimit(2.0f, (float)(cBufLen - 2),
                            (chBaseSec * 1.00f + mod2 * chModSec) * sRate));
        float v3 = readChorusInterp(
            0, juce::jlimit(2.0f, (float)(cBufLen - 2),
                            (chBaseSec * 1.15f + mod3 * chModSec) * sRate));
        float v4 = readChorusInterp(
            1, juce::jlimit(2.0f, (float)(cBufLen - 2),
                            (chBaseSec * 1.30f + mod4 * chModSec) * sRate));

        chOutL = (v1 + v3) * 0.85f;
        chOutR = (v2 + v4) * 0.85f;
      }

      if (!std::isfinite(chOutL))
        chOutL = 0.0f;
      if (!std::isfinite(chOutR))
        chOutR = 0.0f;

      chorusBuffer.setSample(0, chorusWritePos,
                             juce::jlimit(-3.0f, 3.0f, left + chOutL * chFdbk));
      chorusBuffer.setSample(
          1, chorusWritePos,
          juce::jlimit(-3.0f, 3.0f, right + chOutR * chFdbk));
      chorusWritePos = (chorusWritePos + 1) % cBufLen;

      left += chOutL * chWetGain;
      right += chOutR * chWetGain;
    }

    // --- MASTER PAN & MASTER VOLUME (Params 90 & 107) ---
    float rawPan = (panParam != nullptr) ? panParam->load() : 64.0f;
    float normPan = juce::jlimit(0.0f, 1.0f, rawPan / 127.0f);
    float targetPanL = std::cos(normPan * juce::MathConstants<float>::halfPi);
    float targetPanR = std::sin(normPan * juce::MathConstants<float>::halfPi);
    currentPanL += 0.005f * (targetPanL - currentPanL);
    currentPanR += 0.005f * (targetPanR - currentPanR);

    float rawVol = (masterVolParam != nullptr) ? masterVolParam->load() : 100.0f;
    float normVol = juce::jlimit(0.0f, 1.0f, rawVol / 127.0f);
    float targetMasterGain = std::pow(normVol, 2.0f) * 1.6f;
    currentMasterGain += 0.005f * (targetMasterGain - currentMasterGain);

    left = left * currentPanL * currentMasterGain;
    right = right * currentPanR * currentMasterGain;

    if (!std::isfinite(left))
      left = 0.0f;
    if (!std::isfinite(right))
      right = 0.0f;
    left = juce::jlimit(-3.0f, 3.0f, left);
    right = juce::jlimit(-3.0f, 3.0f, right);

    // DC Blocker & subsonic decoupler (tight 45Hz high-pass)
    float hpOmega = 2.0f * juce::MathConstants<float>::pi * 45.0f / sRate;
    float R = juce::jlimit(0.98f, 0.998f, 1.0f - hpOmega);
    dcBlockY_L = left - dcBlockX_L + R * dcBlockY_L;
    dcBlockX_L = left;
    left = dcBlockY_L;

    dcBlockY_R = right - dcBlockX_R + R * dcBlockY_R;
    dcBlockX_R = right;
    right = dcBlockY_R;

    buffer.setSample(0, i, left);
    if (buffer.getNumChannels() > 1) {
      buffer.setSample(1, i, right);
    }
  }
}

bool OpenSynth1AudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor *OpenSynth1AudioProcessor::createEditor() {
  return new OpenSynth1AudioProcessorEditor(*this);
}

void OpenSynth1AudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
  if (auto xml = apvts.copyState().createXml())
    copyXmlToBinary(*xml, destData);
}

void OpenSynth1AudioProcessor::setStateInformation(const void *data,
                                                   int sizeInBytes) {
  if (auto xml = getXmlFromBinary(data, sizeInBytes))
    if (xml->hasTagName(apvts.state.getType()))
      apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

void OpenSynth1AudioProcessor::setBankDirectory(const juce::File &dir) {
  currentBankDir = dir;
  writeSavedBankDirectory(dir);
  scanBanks();
}

void OpenSynth1AudioProcessor::scanBanks() {
  banks.clear();
  if (!currentBankDir.exists() || !currentBankDir.isDirectory())
    return;

  auto subDirs =
      currentBankDir.findChildFiles(juce::File::findDirectories, false);
  subDirs.sort();

  int bNum = 0;
  for (auto &dir : subDirs) {
    Synth1BankInfo bInfo;
    bInfo.bankNumber = bNum++;
    bInfo.name = dir.getFileName();
    bInfo.directory = dir;

    // Cerca file .sy1 case-insensitive (Linux e' case-sensitive)
    juce::Array<juce::File> sy1Files;
    for (auto &f : dir.findChildFiles(juce::File::findFiles, false, "*.sy1"))
      sy1Files.addIfNotAlreadyThere(f);
    for (auto &f : dir.findChildFiles(juce::File::findFiles, false, "*.SY1"))
      sy1Files.addIfNotAlreadyThere(f);
    for (auto &f : dir.findChildFiles(juce::File::findFiles, false, "*.Sy1"))
      sy1Files.addIfNotAlreadyThere(f);
    sy1Files.sort();

    for (auto &pFile : sy1Files) {
      // Estrai il numero dal nome file ("042.sy1" -> 42)
      juce::String stem = pFile.getFileNameWithoutExtension();
      bool isNum = !stem.isEmpty();
      for (auto ch : stem)
        if (!juce::CharacterFunctions::isDigit(ch)) { isNum = false; break; }
      if (!isNum) continue;
      int pIdx = stem.getIntValue();
      if (pIdx < 1 || pIdx > 128) continue;

      Synth1ProgramInfo pInfo;
      pInfo.index = pIdx;
      pInfo.file = pFile;
      pInfo.name = "Empty";
      pInfo.color = "red";

      juce::StringArray lines;
      pFile.readLines(lines);
      if (lines.size() > 0)
        pInfo.name = lines[0].trim();
      if (lines.size() > 1 && lines[1].startsWith("color="))
        pInfo.color = lines[1].fromFirstOccurrenceOf("color=", false, false).trim();

      bInfo.programs.push_back(pInfo);
    }

    // Ordina per indice numerico
    std::sort(bInfo.programs.begin(), bInfo.programs.end(),
              [](const Synth1ProgramInfo &a, const Synth1ProgramInfo &b) {
                return a.index < b.index;
              });

    if (!bInfo.programs.empty())
      banks.push_back(bInfo);
  }

  // Fallback: nessuna bank trovata -> banco vuoto senza preset
  if (banks.empty()) {
    Synth1BankInfo emptyBank;
    emptyBank.bankNumber = 0;
    emptyBank.name = "default";
    emptyBank.directory = currentBankDir;
    banks.push_back(emptyBank);
  }
}

void OpenSynth1AudioProcessor::selectBankAndProgram(int bankIdx, int progIdx) {
  if (bankIdx < 0 || bankIdx >= (int)banks.size())
    return;
  currentBankIndex = bankIdx;
  currentBankName = banks[bankIdx].name;

  auto &progs = banks[bankIdx].programs;
  if (progs.empty()) return;

  // Cerca il preset con .index == progIdx; se non esiste prendi il piu' vicino
  const Synth1ProgramInfo *found = nullptr;
  int bestDist = INT_MAX;
  for (auto &p : progs) {
    int dist = std::abs(p.index - progIdx);
    if (dist < bestDist) { bestDist = dist; found = &p; }
  }
  if (!found) found = &progs[0];

  currentProgramIndex = found->index;
  currentProgramName = found->name;
  currentProgramColor = found->color;
  resetVoicesPending.store(true);

  if (found->file.existsAsFile())
    loadSy1Preset(found->file);
}

void OpenSynth1AudioProcessor::selectNextPreset() {
  if (banks.empty()) return;
  int bIdx = currentBankIndex;
  auto &progs = banks[bIdx].programs;
  // Trova la posizione corrente nell'array
  int pos = -1;
  for (int i = 0; i < (int)progs.size(); ++i)
    if (progs[i].index == currentProgramIndex) { pos = i; break; }

  if (pos < (int)progs.size() - 1) {
    // Preset successivo nella stessa bank
    auto &p = progs[pos + 1];
    currentProgramIndex = p.index;
    currentProgramName = p.name;
    currentProgramColor = p.color;
    resetVoicesPending.store(true);
    if (p.file.existsAsFile()) loadSy1Preset(p.file);
  } else if (bIdx + 1 < (int)banks.size()) {
    // Vai alla prima bank successiva con preset
    selectBankAndProgram(bIdx + 1, 1);
  }
  // Altrimenti siamo all'ultimo preset dell'ultima bank: non fare nulla
}

void OpenSynth1AudioProcessor::selectPrevPreset() {
  if (banks.empty()) return;
  int bIdx = currentBankIndex;
  auto &progs = banks[bIdx].programs;
  int pos = -1;
  for (int i = 0; i < (int)progs.size(); ++i)
    if (progs[i].index == currentProgramIndex) { pos = i; break; }

  if (pos > 0) {
    auto &p = progs[pos - 1];
    currentProgramIndex = p.index;
    currentProgramName = p.name;
    currentProgramColor = p.color;
    resetVoicesPending.store(true);
    if (p.file.existsAsFile()) loadSy1Preset(p.file);
  } else if (bIdx > 0) {
    // Vai all'ultimo preset della bank precedente
    auto &prevProgs = banks[bIdx - 1].programs;
    if (!prevProgs.empty()) {
      currentBankIndex = bIdx - 1;
      currentBankName = banks[bIdx - 1].name;
      auto &p = prevProgs.back();
      currentProgramIndex = p.index;
      currentProgramName = p.name;
      currentProgramColor = p.color;
      resetVoicesPending.store(true);
      if (p.file.existsAsFile()) loadSy1Preset(p.file);
    }
  }
}

void OpenSynth1AudioProcessor::loadSy1Preset(const juce::File &presetFile) {
  if (!presetFile.existsAsFile())
    return;

  resetVoicesPending.store(true);

  juce::StringArray lines;
  presetFile.readLines(lines);

  if (lines.size() > 0)
    currentProgramName = lines[0].trim();
  if (lines.size() > 1 && lines[1].startsWith("color=")) {
    currentProgramColor =
        lines[1].fromFirstOccurrenceOf("color=", false, false).trim();
  }

  int sy1Ver = 112;
  if (lines.size() > 2 && lines[2].startsWith("ver=")) {
    sy1Ver = lines[2].fromFirstOccurrenceOf("ver=", false, false).getIntValue();
  }

  bool has86 = false, has88 = false;
  for (auto line : lines) {
    line = line.trim();
    if (line.containsChar(',')) {
      auto idStr = line.upToFirstOccurrenceOf(",", false, false).trim();
      auto valStr = line.fromFirstOccurrenceOf(",", false, false).trim();
      int val = valStr.getIntValue();
      if (idStr == "86")
        has86 = true;
      if (idStr == "88")
        has88 = true;

      if (auto *rParam = dynamic_cast<juce::RangedAudioParameter *>(
              apvts.getParameter(idStr))) {
        if (idStr == "86" || idStr == "88") {
          if (val >= 45056 && val < 45056 + 128)
            val -= 45056;
          else if (val == 53248)
            val = 128;
        } else if (idStr == "14") {
          val = juce::jlimit(0, 4, val);
        } else if (idStr == "41" || idStr == "46") {
          val = juce::jlimit(0, 6, val - 1);
        } else if (idStr == "31") {
          val = juce::jlimit(0, 3, val - 1);
        } else if (idStr == "64") {
          val = (val >= 1 && val <= 4) ? (val - 1) : juce::jlimit(0, 3, val);
        } else if (idStr == "35") {
          val = juce::jlimit(0, 19, val);
        }

        float normalizedValue = rParam->getNormalisableRange().convertTo0to1(
            static_cast<float>(val));
        rParam->setValueNotifyingHost(normalizedValue);
      }
    }
  }
  if (!has86) {
    if (auto *p = apvts.getParameter("86"))
      p->setValueNotifyingHost(p->convertTo0to1(1.0f));
  }
  if (!has88) {
    if (auto *p = apvts.getParameter("88"))
      p->setValueNotifyingHost(p->convertTo0to1(2.0f));
  }

  resetVoicesPending.store(true);
  presetLoadedFlag.store(true);
}

void OpenSynth1AudioProcessor::resetToInitPreset() {
  resetVoicesPending.store(true);

  for (auto *param : apvts.processor.getParameters()) {
    if (auto *rParam = dynamic_cast<juce::RangedAudioParameter *>(param)) {
      rParam->setValueNotifyingHost(rParam->getDefaultValue());
    }
  }

  currentProgramName = "initial sound";
  currentProgramColor = "red";
  resetVoicesPending.store(true);
  presetLoadedFlag.store(true);
}

bool OpenSynth1AudioProcessor::saveSy1Preset(const juce::File &presetFile,
                                             const juce::String &name,
                                             const juce::String &color) {
  juce::String content;
  content << name << "\n";
  content << "color=" << color << "\n";
  content << "ver=113\n";

  const std::vector<juce::String> paramIds = {
      "0",  "45", "76", "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
      "10", "11", "12", "13", "71", "72", "91", "95", "96", "97", "14", "15",
      "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27",
      "28", "29", "30", "59", "31", "32", "33", "34", "65", "82", "35", "83",
      "36", "98", "37", "66", "64", "52", "53", "54", "55", "56", "60", "61",
      "62", "63", "90", "77", "78", "79", "80", "81", "38", "94", "73", "93",
      "75", "84", "85", "92", "39", "74", "40", "86", "50", "87", "88", "51",
      "89", "57", "41", "42", "43", "44", "67", "68", "58", "46", "47", "48",
      "49", "69", "70", "107"};

  for (auto &id : paramIds) {
    if (auto *p = apvts.getParameter(id)) {
      int intVal = (int)p->getValue() * 127;
      if (auto *rp = dynamic_cast<juce::RangedAudioParameter *>(p)) {
        intVal = (int)std::round(
            rp->getNormalisableRange().convertFrom0to1(rp->getValue()));
      }
      if (id == "41" || id == "46" || id == "31") {
        intVal = intVal + 1;
      } else if (id == "64") {
        if (intVal == 0)
          intVal = 1;
        else if (intVal == 1)
          intVal = 2;
        else
          intVal = 4;
      } else if (id == "86" || id == "88") {
        if (intVal >= 0 && intVal <= 127)
          intVal += 45056;
      }
      content << id << "," << juce::String(intVal) << "\n";
    }
  }

  bool success = presetFile.replaceWithText(content);
  if (success) {
    currentProgramName = name;
    currentProgramColor = color;
    scanBanks();
  }
  return success;
}

void OpenSynth1AudioProcessor::setMasterTuneCents(float cents) {
  masterTuneCents.store(cents);
  for (int i = 0; i < synth.getNumVoices(); ++i) {
    if (auto *v = dynamic_cast<Synth1Voice *>(synth.getVoice(i))) {
      v->setMasterTuneCents(cents);
    }
  }
}

void OpenSynth1AudioProcessor::initDefaultMidiCcMap() {
  midiCcMap.fill(-1);

  midiCcMap[1] = 86;
  midiCcMap[5] = 39;
  midiCcMap[7] = 107;
  midiCcMap[10] = 90;
  midiCcMap[14] = 0;
  midiCcMap[15] = 1;
  midiCcMap[16] = 5;
  midiCcMap[17] = 2;
  midiCcMap[18] = 3;
  midiCcMap[19] = 6;
  midiCcMap[20] = 7;
  midiCcMap[21] = 8;
  midiCcMap[70] = 45;
  midiCcMap[71] = 20;
  midiCcMap[72] = 18;
  midiCcMap[73] = 15;
  midiCcMap[74] = 19;
  midiCcMap[75] = 16;
  midiCcMap[76] = 23;
  midiCcMap[77] = 21;
  midiCcMap[78] = 22;
  midiCcMap[79] = 17;
  midiCcMap[80] = 25;
  midiCcMap[81] = 26;
  midiCcMap[82] = 27;
  midiCcMap[83] = 28;
  midiCcMap[84] = 29;
  midiCcMap[85] = 35;
  midiCcMap[86] = 36;
  midiCcMap[87] = 37;
  midiCcMap[88] = 52;
  midiCcMap[89] = 53;
  midiCcMap[90] = 54;
  midiCcMap[91] = 56;
  midiCcMap[92] = 43;
  midiCcMap[93] = 44;
  midiCcMap[94] = 48;
  midiCcMap[95] = 49;
}

void OpenSynth1AudioProcessor::setMidiCcMapping(int ccNumber, int paramId) {
  if (ccNumber >= 0 && ccNumber < 128) {
    midiCcMap[ccNumber] = paramId;
  }
}

int OpenSynth1AudioProcessor::getMidiCcForParam(int paramId) const {
  for (int i = 0; i < 128; ++i) {
    if (midiCcMap[i] == paramId)
      return i;
  }
  return -1;
}

int OpenSynth1AudioProcessor::getParamForMidiCc(int ccNumber) const {
  if (ccNumber >= 0 && ccNumber < 128)
    return midiCcMap[ccNumber];
  return -1;
}

void OpenSynth1AudioProcessor::startMidiLearn(int targetParamId) {
  midiLearnTargetParam.store(targetParamId);
  midiLearnActive.store(true);
  lastMidiLearnMessage = "MIDI Learn Active: Move a controller knob...";
}

void OpenSynth1AudioProcessor::stopMidiLearn() {
  midiLearnActive.store(false);
  midiLearnTargetParam.store(-1);
}

int OpenSynth1AudioProcessor::getActiveVoiceCount() const {
  uint32_t mask = activeVoicesMask.load();
  int count = 0;
  while (mask) {
    count += (mask & 1);
    mask >>= 1;
  }
  return count;
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new OpenSynth1AudioProcessor();
}
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // OSCILLATORS
    layout.add(std::make_unique<juce::AudioParameterChoice>("0", "Osc1 Shape", juce::StringArray{"Sine", "Saw", "Pulse", "Triangle"}, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("45", "Osc1 FM", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("76", "Osc1 Detune", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("1", "Osc2 Shape", juce::StringArray{"Off", "Saw", "Pulse", "Triangle", "Noise"}, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("2", "Osc2 Pitch", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterChoice>("108", "Osc2 Pitch On/Off", juce::StringArray{"Off", "On"}, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("3", "Osc2 Fine Tune", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterChoice>("109", "Osc2 Fine On/Off", juce::StringArray{"Off", "On"}, 1));
    layout.add(std::make_unique<juce::AudioParameterChoice>("4", "Osc2 Kbd Track", juce::StringArray{"Off", "On"}, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("5", "Osc Mix", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterChoice>("6", "Osc2 Sync", juce::StringArray{"Off", "On"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("7", "Osc2 Ring Mod", juce::StringArray{"Off", "On"}, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("8", "Osc Pulse Width", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("9", "Osc Key Shift", -24, 24, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("10", "Osc Mod Env On/Off", juce::StringArray{"Off", "On"}, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("11", "Osc Mod Env Amt", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("12", "Osc Mod Env Attack", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("13", "Osc Mod Env Decay", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("71", "Osc Mod Dest", juce::StringArray{"osc2", "FM", "p/w"}, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("72", "Osc1,2 Fine Tune", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("91", "Osc Phase Shift", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("95", "Osc1 Sub Gain", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("96", "Osc1 Sub Shape", juce::StringArray{"Sine", "Triangle", "Saw", "Pulse"}, 1));
    layout.add(std::make_unique<juce::AudioParameterChoice>("97", "Osc1 Sub Octave", juce::StringArray{"0oct", "-1oct"}, 1));

    // FILTER
    layout.add(std::make_unique<juce::AudioParameterChoice>("14", "Filter Type", juce::StringArray{"LP12", "LP24", "HP12", "BP12", "LPDL"}, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("15", "Filter Attack", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("16", "Filter Decay", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("17", "Filter Sustain", 0, 127, 32));
    layout.add(std::make_unique<juce::AudioParameterInt>("18", "Filter Release", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("19", "Filter Freq", 0, 127, 81));
    layout.add(std::make_unique<juce::AudioParameterInt>("20", "Filter Resonance", 0, 127, 14));
    layout.add(std::make_unique<juce::AudioParameterInt>("21", "Filter Amount", 0, 127, 127));
    layout.add(std::make_unique<juce::AudioParameterInt>("22", "Filter Saturation", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("23", "Filter Kbd Track", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterChoice>("24", "Filter Vel Switch", juce::StringArray{"Off", "On"}, 1));

    // AMPLIFIER
    layout.add(std::make_unique<juce::AudioParameterInt>("25", "Amp Attack", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("26", "Amp Decay", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("27", "Amp Sustain", 0, 127, 107));
    layout.add(std::make_unique<juce::AudioParameterInt>("28", "Amp Release", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("29", "Amp Gain", 0, 127, 107));
    layout.add(std::make_unique<juce::AudioParameterInt>("30", "Amp Vel Sens", 0, 127, 64));

    // ARPEGGIATOR
    layout.add(std::make_unique<juce::AudioParameterChoice>("59", "Arp On/Off", juce::StringArray{"Off", "On"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("31", "Arp Type", juce::StringArray{"Up/Down", "Up", "Down", "Random"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("32", "Arp Range", juce::StringArray{"1oct", "2oct", "3oct", "4oct"}, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("33", "Arp Beat", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("34", "Arp Gate", 0, 127, 64));

    // TEMPO DELAY
    layout.add(std::make_unique<juce::AudioParameterChoice>("65", "Delay On/Off", juce::StringArray{"Off", "On"}, 1));
    layout.add(std::make_unique<juce::AudioParameterChoice>("82", "Delay Type", juce::StringArray{"ST", "X", "PP"}, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("35", "Delay Time", 0, 19, 8));
    layout.add(std::make_unique<juce::AudioParameterInt>("83", "Delay Spread", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("36", "Delay Feedback", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("98", "Delay Tone", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("37", "Delay Dry/Wet", 0, 127, 64));

    // CHORUS / FLANGER
    layout.add(std::make_unique<juce::AudioParameterChoice>("66", "Chorus On/Off", juce::StringArray{"Off", "On"}, 1));
    layout.add(std::make_unique<juce::AudioParameterChoice>("64", "Chorus Type", juce::StringArray{"x1", "x2", "x4"}, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("52", "Chorus Time", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("53", "Chorus Depth", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("54", "Chorus Rate", 0, 127, 50));
    layout.add(std::make_unique<juce::AudioParameterInt>("55", "Chorus Feedback", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("56", "Chorus Level", 0, 127, 40));

    // EQUALIZER / PAN
    layout.add(std::make_unique<juce::AudioParameterInt>("60", "EQ Tone", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("61", "EQ Freq", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("62", "EQ Level", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("63", "EQ Q", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("90", "Pan", 0, 127, 64));

    // EFFECT
    layout.add(std::make_unique<juce::AudioParameterChoice>("77", "Effect On/Off", juce::StringArray{"Off", "On"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("78", "Effect Type", juce::StringArray{"a.d.1", "a.d.2", "d.d.", "deci.", "r.m.", "comp.", "ph.1", "ph.2", "ph.3", "ph.4"}, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("79", "Effect Ctrl1", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("80", "Effect Ctrl2", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("81", "Effect Level", 0, 127, 64));

    // VOICE & PLAY MODE
    layout.add(std::make_unique<juce::AudioParameterChoice>("38", "Play Mode", juce::StringArray{"Poly", "Mono", "Legato"}, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("94", "Polyphony", 1, 32, 16));
    layout.add(std::make_unique<juce::AudioParameterChoice>("73", "Unison Mode", juce::StringArray{"Off", "On"}, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>("93", "Unison Voices", 2, 32, 2));
    layout.add(std::make_unique<juce::AudioParameterInt>("75", "Unison Detune", 0, 127, 22));
    layout.add(std::make_unique<juce::AudioParameterInt>("84", "Unison Pan Spread", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("85", "Unison Pitch", 0, 127, 24));
    layout.add(std::make_unique<juce::AudioParameterInt>("92", "Unison Phase Shift", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("39", "Portamento Time", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("74", "Portamento Auto", juce::StringArray{"Off", "On"}, 0));

    // WHEEL / MIDI
    layout.add(std::make_unique<juce::AudioParameterInt>("40", "PitchBend Range", 0, 24, 12));
    layout.add(std::make_unique<juce::AudioParameterInt>("86", "MIDI Ctrl Src 1", 0, 127, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("50", "MIDI Ctrl Sens 1", 0, 127, 20));
    layout.add(std::make_unique<juce::AudioParameterInt>("87", "MIDI Ctrl Assign 1", -1, 98, 44));
    layout.add(std::make_unique<juce::AudioParameterInt>("88", "MIDI Ctrl Src 2", 0, 127, 2));
    layout.add(std::make_unique<juce::AudioParameterInt>("51", "MIDI Ctrl Sens 2", 0, 127, 20));
    layout.add(std::make_unique<juce::AudioParameterInt>("89", "MIDI Ctrl Assign 2", -1, 98, 43));

    // LFO 1 & 2
    layout.add(std::make_unique<juce::AudioParameterChoice>("57", "LFO1 On/Off", juce::StringArray{"Off", "On"}, 1));
    layout.add(std::make_unique<juce::AudioParameterChoice>("41", "LFO1 Dest", juce::StringArray{"osc2", "osc1,2", "filter", "amp", "p/w", "FM", "pan"}, 2));
    layout.add(std::make_unique<juce::AudioParameterChoice>("42", "LFO1 Type", juce::StringArray{"Saw", "Triangle", "Square", "Random", "Smoothed", "Sine"}, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("43", "LFO1 Speed", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("44", "LFO1 Depth", 0, 127, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("67", "LFO1 Tempo Sync", juce::StringArray{"Off", "On"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("68", "LFO1 Key Sync", juce::StringArray{"Off", "On"}, 0));

    layout.add(std::make_unique<juce::AudioParameterChoice>("58", "LFO2 On/Off", juce::StringArray{"Off", "On"}, 1));
    layout.add(std::make_unique<juce::AudioParameterChoice>("46", "LFO2 Dest", juce::StringArray{"osc2", "osc1,2", "filter", "amp", "p/w", "FM", "pan"}, 5));
    layout.add(std::make_unique<juce::AudioParameterChoice>("47", "LFO2 Type", juce::StringArray{"Saw", "Triangle", "Square", "Random", "Smoothed", "Sine"}, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("48", "LFO2 Speed", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterInt>("49", "LFO2 Depth", 0, 127, 64));
    layout.add(std::make_unique<juce::AudioParameterChoice>("69", "LFO2 Tempo Sync", juce::StringArray{"Off", "On"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("70", "LFO2 Key Sync", juce::StringArray{"Off", "On"}, 0));

    // MASTER VOLUME
    layout.add(std::make_unique<juce::AudioParameterInt>("107", "Master Vol", 0, 127, 100));

    return layout;
}

// Exact Synth1 semitone table for Osc2 Pitch (0..127 -> -60..+60 semitones with 7 duplicate steps)
static const int s_synth1Osc2PitchSemitones[128] = {
    -60, -60, -59, -58, -57, -56, -55, -54, -53, -52, -51, -50, -49, -48, -47, -46,
    -45, -44, -43, -43, -42, -41, -40, -39, -38, -37, -36, -35, -34, -33, -32, -31,
    -30, -29, -28, -27, -26, -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16,
    -15, -14, -13, -12, -11, -10,  -9,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,
      0,   1,   2,   3,   4,   5,   6,   7,   8,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  26,  27,  28,  29,
     30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60
};


// -------------------------------------------------------------
// Synth1Voice Implementation
// -------------------------------------------------------------
Synth1Voice::Synth1Voice(juce::AudioProcessorValueTreeState& apvts, int voiceIdx, std::atomic<uint32_t>& voiceMask, std::atomic<float>& hostBpmRef)
    : voiceIndex(voiceIdx), activeVoicesMask(voiceMask), hostBpm(hostBpmRef)
{
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
    filterCutoff = apvts.getRawParameterValue("19");
    filterRes = apvts.getRawParameterValue("20");
    filterAmt = apvts.getRawParameterValue("21");
    filterSat = apvts.getRawParameterValue("22");
    filterTrack = apvts.getRawParameterValue("23");
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

bool Synth1Voice::canPlaySound(juce::SynthesiserSound* sound)
{
    if (dynamic_cast<Synth1Sound*>(sound) == nullptr) return false;
    int mode = (playMode != nullptr) ? (int)playMode->load() : 0;
    if (mode > 0 && voiceIndex > 0) return false; // Mono or Legato: only voice 0 can play
    int poly = (polyphonyParam != nullptr) ? (int)polyphonyParam->load() : 16;
    if (poly < 1) poly = 1;
    return voiceIndex < poly;
}

void Synth1Voice::resetVoiceState()
{
    filter.reset();
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

void Synth1Voice::updateLegatoNote(int newMidiNoteNumber, float newVelocity)
{
    int keyShift = (oscKeyShift != nullptr) ? (int)oscKeyShift->load() : 0;
    currentMidiNote = juce::jlimit(0, 127, newMidiNoteNumber + keyShift);
    targetNoteFreq = (float)juce::MidiMessage::getMidiNoteInHertz(currentMidiNote);
    currentNoteVelocity = newVelocity;

    float velSens = (ampVel != nullptr) ? (ampVel->load() / 127.0f) : 0.5f;
    level = (1.0f - velSens) + newVelocity * velSens;

    // In Legato mode, envelopes are NOT retriggered, continuing smoothly.
    float porta = (portamentoTime != nullptr) ? (portamentoTime->load() / 127.0f) : 0.0f;
    if (porta <= 0.01f) {
        currentGlideFreq = targetNoteFreq;
    }
}

void Synth1Voice::retriggerMonoNote(int newMidiNoteNumber, float newVelocity)
{
    float sRate = (float)getSampleRate();
    if (sRate <= 0.0f) sRate = 44100.0f;

    ampAdsr.setSampleRate(sRate);
    filterAdsr.setSampleRate(sRate);
    updateEnvelopes();

    int keyShift = (oscKeyShift != nullptr) ? (int)oscKeyShift->load() : 0;
    currentMidiNote = juce::jlimit(0, 127, newMidiNoteNumber + keyShift);
    targetNoteFreq = (float)juce::MidiMessage::getMidiNoteInHertz(currentMidiNote);
    currentNoteVelocity = newVelocity;

    float velSens = (ampVel != nullptr) ? (ampVel->load() / 127.0f) : 0.5f;
    level = (1.0f - velSens) + newVelocity * velSens;

    // Retrigger envelopes for Mono mode
    ampAdsr.noteOn();
    filterAdsr.noteOn();
    modEnvPhase = 0.0f;
    modEnvActive = true;

    float porta = (portamentoTime != nullptr) ? (portamentoTime->load() / 127.0f) : 0.0f;
    if (porta <= 0.01f) {
        currentGlideFreq = targetNoteFreq;
    }
}

void Synth1Voice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int currentPitchWheelPosition)
{
    activeVoicesMask.fetch_or(1u << (voiceIndex % 32));

    float sRate = (float)getSampleRate();
    if (sRate <= 0.0f) sRate = 44100.0f;

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

        // Phase control (Param 91): if > 0, randomize initial phases on trigger
        // If <= 0, phases are free-running (analog continuity across notes)
        float pShift = (oscPhaseShift != nullptr) ? oscPhaseShift->load() : 0.0f;
        if (pShift > 0.5f) {
            float initPhase = pShift / 127.0f;
            osc1PhaseA = initPhase;
            osc1PhaseB = initPhase;
            osc2Phase = initPhase;
            subPhase = initPhase;
            for (int d = 0; d < 8; ++d) {
                osc1DetunePhases[d] = random.nextFloat();
                subDetunePhases[d] = osc1DetunePhases[d];
            }
        }
    }

    currentNoteVelocity = velocity;
    float velSens = (ampVel != nullptr) ? (ampVel->load() / 127.0f) : 0.5f;
    level = (1.0f - velSens) + velocity * velSens;

    int keyShift = (oscKeyShift != nullptr) ? (int)oscKeyShift->load() : 0;
    currentMidiNote = juce::jlimit(0, 127, midiNoteNumber + keyShift);
    targetNoteFreq = (float)juce::MidiMessage::getMidiNoteInHertz(currentMidiNote);

    float porta = (portamentoTime != nullptr) ? (portamentoTime->load() / 127.0f) : 0.0f;
    bool autoPorta = (portamentoAuto != nullptr && portamentoAuto->load() > 0.5f);

    if (porta <= 0.01f || currentGlideFreq <= 0.0f || (autoPorta && !wasPlaying)) {
        currentGlideFreq = targetNoteFreq;
    }

    pitchWheelMoved(currentPitchWheelPosition);

    if (lfo1Key != nullptr && lfo1Key->load() > 0.5f) {
        lfo1Phase = 0.0f;
    }
    if (lfo2Key != nullptr && lfo2Key->load() > 0.5f) {
        lfo2Phase = 0.0f;
    }

    float pShift = (unisonPhaseShift != nullptr) ? (unisonPhaseShift->load() / 127.0f) : 0.0f;
    for (int v = 0; v < 32; ++v) {
        uOsc1Phase[v] = std::fmod(pShift * (float)v * 0.25f, 1.0f);
        uOsc2Phase[v] = std::fmod(pShift * (float)v * 0.25f, 1.0f);
    }

    if (!wasPlaying) {
        filter.reset();
    }
}

void Synth1Voice::stopNote(float /*velocity*/, bool allowTailOff)
{
    ampAdsr.noteOff();
    filterAdsr.noteOff();

    if (!allowTailOff) {
        activeVoicesMask.fetch_and(~(1u << (voiceIndex % 32)));
        filter.reset();
        clearCurrentNote();
    }
}

void Synth1Voice::pitchWheelMoved(int newPitchWheelValue)
{
    float range = (pbRange != nullptr) ? pbRange->load() : 12.0f;
    pitchWheelBend = ((float)(newPitchWheelValue - 8192) / 8192.0f) * range;
}

void Synth1Voice::controllerMoved(int controllerNumber, int newControllerValue)
{
    int src1 = (midiCtrlSrc1 != nullptr) ? (int)midiCtrlSrc1->load() : 1;
    int src2 = (midiCtrlSrc2 != nullptr) ? (int)midiCtrlSrc2->load() : 2;

    if (controllerNumber == src1)
        midiCtrl1Value = (float)newControllerValue / 127.0f;
    if (controllerNumber == src2)
        midiCtrl2Value = (float)newControllerValue / 127.0f;
}

inline float polyBlep(float t, float dt)
{
    if (dt <= 0.0f) return 0.0f;
    if (t < dt) {
        t /= dt;
        return t + t - t * t - 1.0f;
    }
    else if (t > 1.0f - dt) {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    return 0.0f;
}

float Synth1Voice::getOsc1Sample(float phase, float dt, int shape, float pw)
{
    switch (shape)
    {
        case 0: // Sine
            return std::sin(phase * juce::MathConstants<float>::twoPi);
        case 1: // Descending Saw with PolyBLEP (authentic Synth1 polarity)
        {
            float s = 1.0f - 2.0f * phase;
            s += polyBlep(phase, dt);
            return s;
        }
        case 2: // Pulse with PW and PolyBLEP
        {
            float p = (phase < pw) ? 1.0f : -1.0f;
            p += polyBlep(phase, dt);
            p -= polyBlep(std::fmod(phase + (1.0f - pw), 1.0f), dt);
            return p;
        }
        case 3: // Triangle
            return 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
        default:
            return 0.0f;
    }
}

float Synth1Voice::getOsc2Sample(float phase, float dt, int shape, float pw)
{
    switch (shape)
    {
        case 0: // Off
            return 0.0f;
        case 1: // Descending Saw with PolyBLEP (authentic Synth1 polarity)
        {
            float s = 1.0f - 2.0f * phase;
            s += polyBlep(phase, dt);
            return s;
        }
        case 2: // Pulse with PW and PolyBLEP
        {
            float p = (phase < pw) ? 1.0f : -1.0f;
            p += polyBlep(phase, dt);
            p -= polyBlep(std::fmod(phase + (1.0f - pw), 1.0f), dt);
            return p;
        }
        case 3: // Triangle
            return 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
        case 4: // Noise
            return (random.nextFloat() * 2.0f) - 1.0f;
        default:
            return 0.0f;
    }
}

float Synth1Voice::getSubOscSample(float phase, int shape)
{
    switch (shape)
    {
        case 0: // Sine
            return std::sin(phase * juce::MathConstants<float>::twoPi);
        case 1: // Triangle
            return 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
        case 2: // Descending Saw
            return 1.0f - 2.0f * phase;
        case 3: // Pulse (always 50% square in Synth1 sub-osc)
            return (phase < 0.5f) ? 1.0f : -1.0f;
        default:
            return 0.0f;
    }
}

float Synth1Voice::getLfoSample(float phase, int shape, float& shVal)
{
    switch (shape)
    {
        case 0: // Saw (ramp down)
            return 1.0f - 2.0f * phase;
        case 1: // Triangle
            return 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
        case 2: // Square
            return (phase < 0.5f) ? 1.0f : -1.0f;
        case 3: // Random S&H
            return shVal;
        case 4: // Random Smoothed
            return shVal;
        case 5: // Sine
            return std::sin(phase * juce::MathConstants<float>::twoPi);
        default:
            return std::sin(phase * juce::MathConstants<float>::twoPi);
    }
}

void Synth1Voice::updateEnvelopes()
{
    // Parameter transfer functions
    auto mapAttack = [](float val) {
        return 0.0005f * std::pow(56000.0f, juce::jlimit(0.0f, 127.0f, val) / 127.0f);
    };
    auto mapDecayRelease = [](float val) {
        return 0.01f * std::pow(6000.0f, juce::jlimit(0.0f, 127.0f, val) / 127.0f);
    };
    auto mapSustain = [](float val) {
        if (val <= 0.001f) return 0.0f;
        return std::pow(2048.0f, std::pow(juce::jlimit(0.0f, 127.0f, val) / 127.0f, 0.3f)) / 2048.0f;
    };

    if (ampAttack && ampDecay && ampSustain && ampRelease)
    {
        juce::ADSR::Parameters ap;
        ap.attack  = mapAttack(ampAttack->load());
        ap.decay   = mapDecayRelease(ampDecay->load());
        ap.sustain = mapSustain(ampSustain->load());
        ap.release = mapDecayRelease(ampRelease->load());
        ampAdsr.setParameters(ap);
    }

    if (filterAttack && filterDecay && filterSustain && filterRelease)
    {
        juce::ADSR::Parameters fp;
        fp.attack  = mapAttack(filterAttack->load());
        fp.decay   = mapDecayRelease(filterDecay->load());
        fp.sustain = mapSustain(filterSustain->load());
        fp.release = mapDecayRelease(filterRelease->load());
        filterAdsr.setParameters(fp);
    }
}

void Synth1Voice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isVoiceActive()) {
        activeVoicesMask.fetch_and(~(1u << (voiceIndex % 32)));
        return;
    }

    updateEnvelopes();

    float sampleRate = (float)getSampleRate();
    if (sampleRate <= 0.0f) sampleRate = 44100.0f;

    // Glide / Portamento
    float portaSpeed = (portamentoTime != nullptr) ? (portamentoTime->load() / 127.0f) : 0.0f;
    float glideCoeff = (portaSpeed > 0.01f) ? (1.0f - std::exp(-1.0f / (portaSpeed * 0.5f * sampleRate))) : 1.0f;

    // Pulse width
    float basePw = (oscPulseWidth != nullptr) ? (oscPulseWidth->load() / 127.0f) : 0.5f;
    basePw = juce::jlimit(0.05f, 0.95f, basePw);

    // Osc 1 Detune
    float det1 = (osc1Detune != nullptr) ? (osc1Detune->load() / 127.0f * 0.5f) : 0.0f;

    // Osc 2 Pitch & Fine
    bool pOn = (osc2PitchOn == nullptr || osc2PitchOn->load() > 0.5f);
    int pVal = (osc2Pitch != nullptr) ? juce::jlimit(0, 127, (int)std::round(osc2Pitch->load())) : 64;
    float pitchSemi = pOn ? (float)s_synth1Osc2PitchSemitones[pVal] : 0.0f;

    bool fOn = (osc2FineOn == nullptr || osc2FineOn->load() > 0.5f);
    float pitchFine = (fOn && osc2Fine != nullptr) ? ((osc2Fine->load() - 64.0f) * 0.96875f / 100.0f) : 0.0f; // -62 to +61 cents
    bool kbdTrack2 = (osc2KbdTrack != nullptr) ? (osc2KbdTrack->load() > 0.5f) : true;

    // Mix
    float mix = (oscMix != nullptr) ? (oscMix->load() / 127.0f) : 0.5f;

    // Sync & Ring
    bool syncOn = (osc2Sync != nullptr) ? (osc2Sync->load() > 0.5f) : false;
    bool ringOn = (osc2Ring != nullptr) ? (osc2Ring->load() > 0.5f) : false;

    // FM
    float fmAmount = (osc1FM != nullptr) ? (osc1FM->load() / 127.0f) : 0.0f;

    // Mod Env
    bool modEnvOn = (oscModEnvOn != nullptr) ? (oscModEnvOn->load() > 0.5f) : false;
    float modEnvAmtVal = (oscModEnvAmt != nullptr) ? ((oscModEnvAmt->load() - 64.0f) / 64.0f) : 0.0f;
    int modDest = (oscModDest != nullptr) ? (int)oscModDest->load() : 0; // 0: osc2, 1: FM, 2: p/w

    // Sub-osc
    float subGainVal = (oscSubGain != nullptr) ? (oscSubGain->load() / 127.0f) : 0.0f;
    int subShapeVal = (oscSubShape != nullptr) ? (int)oscSubShape->load() : 1;
    int subOctVal = (oscSubOctave != nullptr) ? (int)oscSubOctave->load() : 1; // 0: 0oct, 1: -1oct

    // Filter params
    int fType = (filterType != nullptr) ? (int)filterType->load() : 1; // 0: LP12, 1: LP24, 2: HP12, 3: BP12, 4: LPDL
    float rawCutoff = (filterCutoff != nullptr) ? (filterCutoff->load() / 127.0f) : 0.6f;
    float resVal = (filterRes != nullptr) ? (filterRes->load() / 127.0f) : 0.1f;
    float rawFAmt = (filterAmt != nullptr) ? filterAmt->load() : 127.0f;
    float fAmtVal = (rawFAmt >= 63.0f) ? ((rawFAmt - 63.0f) / 64.0f) : ((rawFAmt - 63.0f) / 63.0f); // Bipolar: 0..127 (-63..+64), 63=0
    float satVal = (filterSat != nullptr) ? (filterSat->load() / 127.0f) : 0.0f;

    int shape1 = (osc1Shape != nullptr) ? (int)osc1Shape->load() : 1;
    int shape2 = (osc2Shape != nullptr) ? (int)osc2Shape->load() : 0;

    // LFO 1 setup
    bool lfo1Active = (lfo1On != nullptr && lfo1On->load() > 0.5f);
    int lfo1Shape = (lfo1Type != nullptr) ? (int)lfo1Type->load() : 1;
    float lfo1SpdNorm = (lfo1Speed != nullptr) ? (lfo1Speed->load() / 127.0f) : 0.5f;
    bool lfo1Sync = (lfo1Tempo != nullptr && lfo1Tempo->load() > 0.5f);
    float lfo1Hz = 0.1f * std::pow(300.0f, lfo1SpdNorm); // 0.1Hz - 30Hz
    if (lfo1Sync) {
        static const float lfoSyncBeats[] = {
            64.0f, 32.0f, 16.0f, 8.0f, 4.0f, 3.0f, 2.0f, 1.3333f,
            1.5f, 1.0f, 0.6667f, 0.75f, 0.5f, 0.3333f, 0.375f, 0.25f, 0.125f
        };
        int divIdx = juce::jlimit(0, 16, (int)std::round(lfo1SpdNorm * 16.0f));
        float beatSec = 60.0f / hostBpm.load();
        float cycleSec = beatSec * lfoSyncBeats[divIdx];
        if (cycleSec > 0.001f) lfo1Hz = 1.0f / cycleSec;
    }
    float lfo1DepthNorm = (lfo1Depth != nullptr) ? (lfo1Depth->load() / 127.0f) : 0.0f;
    int lfo1Dst = (lfo1Dest != nullptr) ? (int)lfo1Dest->load() : 2; // 0..6
    float lfo1Delta = lfo1Hz / sampleRate;

    // LFO 2 setup
    bool lfo2Active = (lfo2On != nullptr && lfo2On->load() > 0.5f);
    int lfo2Shape = (lfo2Type != nullptr) ? (int)lfo2Type->load() : 1;
    float lfo2SpdNorm = (lfo2Speed != nullptr) ? (lfo2Speed->load() / 127.0f) : 0.5f;
    bool lfo2Sync = (lfo2Tempo != nullptr && lfo2Tempo->load() > 0.5f);
    float lfo2Hz = 0.1f * std::pow(300.0f, lfo2SpdNorm);
    if (lfo2Sync) {
        static const float lfoSyncBeats[] = {
            64.0f, 32.0f, 16.0f, 8.0f, 4.0f, 3.0f, 2.0f, 1.3333f,
            1.5f, 1.0f, 0.6667f, 0.75f, 0.5f, 0.3333f, 0.375f, 0.25f, 0.125f
        };
        int divIdx = juce::jlimit(0, 16, (int)std::round(lfo2SpdNorm * 16.0f));
        float beatSec = 60.0f / hostBpm.load();
        float cycleSec = beatSec * lfoSyncBeats[divIdx];
        if (cycleSec > 0.001f) lfo2Hz = 1.0f / cycleSec;
    }
    float lfo2DepthNorm = (lfo2Depth != nullptr) ? (lfo2Depth->load() / 127.0f) : 0.0f;
    int lfo2Dst = (lfo2Dest != nullptr) ? (int)lfo2Dest->load() : 5;
    float lfo2Delta = lfo2Hz / sampleRate;

    // Unison setup
    bool isUnison = (unisonOn != nullptr && unisonOn->load() > 0.5f);
    int uVoices = (unisonVoices != nullptr) ? juce::jlimit(2, 32, (int)unisonVoices->load()) : 2;
    float uDetuneVal = (unisonDetune != nullptr) ? (unisonDetune->load() / 127.0f * 35.0f) : 6.0f;
    float uSpreadVal = (unisonSpread != nullptr) ? (unisonSpread->load() / 127.0f) : 0.5f;
    float uPitchVal = (unisonPitch != nullptr) ? ((unisonPitch->load() - 64.0f) / 64.0f * 12.0f) : 0.0f;

    // Precomputed unison detune factors and pan trigonometry (avoids millions of std::pow/sin/cos per second)
    float uDetFactors[32];
    float uPanCos[32];
    float uPanSin[32];
    if (isUnison)
    {
        for (int v = 0; v < uVoices; ++v)
        {
            float vSpread = (float)(v - (uVoices - 1) * 0.5f) / (float)std::max(1, uVoices - 1);
            uDetFactors[v] = std::pow(2.0f, (vSpread * uDetuneVal) / 1200.0f);
            float vPan = juce::jlimit(0.0f, 1.0f, 0.5f + vSpread * uSpreadVal * 0.5f);
            uPanCos[v] = std::cos(vPan * juce::MathConstants<float>::halfPi);
            uPanSin[v] = std::sin(vPan * juce::MathConstants<float>::halfPi);
        }
    }

    // Master fine cents
    float masterFineCents = (masterFineTune != nullptr) ? ((masterFineTune->load() - 64.0f) * (100.0f / 64.0f)) : masterTuneCents;
    float subDetFactor = (subOctVal == 1 ? 0.5f : 1.0f);

    // Precomputed 8-multiple supersaw cluster detune pitch multipliers
    float baseDetVal = (osc1Detune != nullptr) ? osc1Detune->load() : 0.0f;
    float osc1DetFactors[8];
    static const float kOddMultipliers[8] = { -7.0f, -5.0f, -3.0f, -1.0f, 1.0f, 3.0f, 5.0f, 7.0f };
    if (baseDetVal > 0.5f && (shape1 == 1 || shape1 == 2))
    {
        float detStepCents = (baseDetVal / 127.0f) * (20.0f / 7.0f);
        for (int d = 0; d < 8; ++d)
        {
            osc1DetFactors[d] = std::pow(2.0f, (kOddMultipliers[d] * detStepCents) / 1200.0f);
        }
    }

    for (int i = startSample; i < startSample + numSamples; ++i)
    {
        // Glide
        currentGlideFreq += (targetNoteFreq - currentGlideFreq) * glideCoeff;

        // LFO calculations (use per-voice if Key Sync is on, otherwise use processor global LFO)
        bool lfo1KeySync = (lfo1Key != nullptr && lfo1Key->load() > 0.5f);
        float effectiveLfo1Phase = lfo1KeySync ? lfo1Phase : globalLfo1Phase;
        float effectiveLfo1Sh = lfo1KeySync ? lfo1ShVal : globalLfo1Sh;
        float lfo1Val = 0.0f;
        if (lfo1Active) {
            lfo1Val = getLfoSample(effectiveLfo1Phase, lfo1Shape, effectiveLfo1Sh) * lfo1DepthNorm;
            if (lfo1KeySync) {
                lfo1Phase += lfo1Delta;
                if (lfo1Phase >= 1.0f) {
                    lfo1Phase -= 1.0f;
                    lfo1ShVal = random.nextFloat() * 2.0f - 1.0f;
                }
            }
        }

        bool lfo2KeySync = (lfo2Key != nullptr && lfo2Key->load() > 0.5f);
        float effectiveLfo2Phase = lfo2KeySync ? lfo2Phase : globalLfo2Phase;
        float effectiveLfo2Sh = lfo2KeySync ? lfo2ShVal : globalLfo2Sh;
        float lfo2Val = 0.0f;
        if (lfo2Active) {
            lfo2Val = getLfoSample(effectiveLfo2Phase, lfo2Shape, effectiveLfo2Sh) * lfo2DepthNorm;
            if (lfo2KeySync) {
                lfo2Phase += lfo2Delta;
                if (lfo2Phase >= 1.0f) {
                    lfo2Phase -= 1.0f;
                    lfo2ShVal = random.nextFloat() * 2.0f - 1.0f;
                }
            }
        }

        // Modulation accumulators
        float lfoPitchMod2 = 0.0f;
        float lfoPitchMod12 = 0.0f;
        float lfoFilterMod = 0.0f;
        float lfoAmpMod = 1.0f;
        float lfoPwMod = 0.0f;
        float lfoFmMod = 0.0f;
        float lfoPanMod = 0.0f;

        auto applyLfo = [&](int dst, float val) {
            switch (dst) {
                case 0: lfoPitchMod2  += val * 12.0f; break; // osc2 pitch
                case 1: lfoPitchMod12 += val * 2.0f;  break; // vibrato osc1&2
                case 2: lfoFilterMod  += val * 0.4f;  break; // filter cutoff
                case 3: lfoAmpMod     *= std::max(0.0f, 1.0f + val * 0.8f); break; // tremolo
                case 4: lfoPwMod      += val * 0.4f;  break; // pulse width
                case 5: lfoFmMod      += val * 0.5f;  break; // FM
                case 6: lfoPanMod     += val * 0.5f;  break; // pan
            }
        };
        if (lfo1Active) applyLfo(lfo1Dst, lfo1Val);
        if (lfo2Active) applyLfo(lfo2Dst, lfo2Val);

        float detVal = baseDetVal;

        // MIDI controller modulation (Mod Wheel, Breath, etc. routed by Synth1 parameter ID)
        auto applyMidiCtrl = [&](int assignParamId, float amount) {
            switch (assignParamId) {
                case 44: lfo1DepthNorm = juce::jlimit(0.0f, 1.0f, lfo1DepthNorm + amount); break; // LFO1 Depth
                case 43: lfo1Delta *= std::pow(2.0f, amount * 2.0f); break;                        // LFO1 Speed
                case 49: lfo2DepthNorm = juce::jlimit(0.0f, 1.0f, lfo2DepthNorm + amount); break; // LFO2 Depth
                case 48: lfo2Delta *= std::pow(2.0f, amount * 2.0f); break;                        // LFO2 Speed
                case 19: lfoFilterMod += amount * 0.5f; break;                                     // Filter Freq (Cutoff)
                case 20: resVal = juce::jlimit(0.0f, 1.0f, resVal + amount); break;               // Filter Res
                case 21: fAmtVal = juce::jlimit(-1.0f, 1.0f, fAmtVal + amount); break;            // Filter Amt
                case 29: lfoAmpMod *= std::max(0.0f, 1.0f + amount); break;                       // Amp Gain
                case 2:  lfoPitchMod2 += amount * 12.0f; break;                                    // Osc2 Pitch
                case 45: lfoFmMod += amount * 0.5f; break;                                         // Osc1 FM
                case 8:  lfoPwMod += amount * 0.4f; break;                                         // Pulse Width
                case 90: lfoPanMod += amount * 0.5f; break;                                        // Pan
                case 76: detVal = juce::jlimit(0.0f, 127.0f, detVal + amount * 64.0f); break;    // Osc1 Detune
                default: break;
            }
        };

        if (midiCtrlSrc1 != nullptr && midiCtrlAssign1 != nullptr && midiCtrlSens1 != nullptr) {
            float sens1 = (midiCtrlSens1->load() - 64.0f) / 64.0f;
            int a1 = (int)midiCtrlAssign1->load();
            applyMidiCtrl(a1, midiCtrl1Value * sens1);
        }
        if (midiCtrlSrc2 != nullptr && midiCtrlAssign2 != nullptr && midiCtrlSens2 != nullptr) {
            float sens2 = (midiCtrlSens2->load() - 64.0f) / 64.0f;
            int a2 = (int)midiCtrlAssign2->load();
            applyMidiCtrl(a2, midiCtrl2Value * sens2);
        }

        float baseFreq = currentGlideFreq * std::pow(2.0f, (pitchWheelBend + lfoPitchMod12 + (masterFineCents / 100.0f) + uPitchVal) / 12.0f);

        // Mod Env calculation (AD using authentic Toda timing)
        float modEnvOut = 0.0f;
        if (modEnvOn) {
            float rawAtt = (oscModEnvAttack != nullptr) ? oscModEnvAttack->load() : 0.0f;
            float rawDec = (oscModEnvDecay != nullptr) ? oscModEnvDecay->load() : 0.0f;
            float attTime = 0.0005f * std::pow(56000.0f, juce::jlimit(0.0f, 127.0f, rawAtt) / 127.0f);
            float decTime = 0.01f * std::pow(6000.0f, juce::jlimit(0.0f, 127.0f, rawDec) / 127.0f);
            if (modEnvPhase < attTime) {
                modEnvOut = modEnvPhase / attTime;
            } else {
                modEnvOut = std::max(0.0f, 1.0f - (modEnvPhase - attTime) / decTime);
            }
            modEnvPhase += 1.0f / sampleRate;
        }

        // Mod env routing
        float osc2PitchMod = (modDest == 0 && modEnvOn) ? (modEnvOut * modEnvAmtVal * 24.0f) : 0.0f;
        float fmMod = (modDest == 1 && modEnvOn) ? (modEnvOut * modEnvAmtVal) : 0.0f;
        float pwMod = (modDest == 2 && modEnvOn) ? (modEnvOut * modEnvAmtVal * 0.4f) : 0.0f;

        // Headroom-scaled pulse width modulation
        float totalPwMod = pwMod + lfoPwMod;
        float effectivePw = basePw;
        if (totalPwMod > 0.0f)
            effectivePw = basePw + totalPwMod * (0.95f - basePw);
        else
            effectivePw = basePw + totalPwMod * (basePw - 0.05f);
        effectivePw = juce::jlimit(0.05f, 0.95f, effectivePw);

        float effectiveFm = std::max(0.0f, fmAmount + fmMod + lfoFmMod);

        // Osc 1 & 2 Frequencies
        float freq1 = baseFreq;
        float fixedBaseFreq = 261.6255f * std::pow(2.0f, (pitchWheelBend + lfoPitchMod12 + (masterFineCents / 100.0f) + uPitchVal) / 12.0f);
        float freq2 = kbdTrack2 ? (baseFreq * std::pow(2.0f, (pitchSemi + pitchFine + osc2PitchMod + lfoPitchMod2) / 12.0f))
                                : (fixedBaseFreq * std::pow(2.0f, (pitchSemi + pitchFine + osc2PitchMod + lfoPitchMod2) / 12.0f));

        // Osc 2 sample with PolyBLEP
        float dt2 = freq2 / sampleRate;
        float osc2Sample = getOsc2Sample(osc2Phase, dt2, shape2, effectivePw);

        // Linear FM on Osc 1 (smooth phase modulation without PolyBLEP discontinuity)
        float fmModAmt = effectiveFm * 0.35f;

        // Sync Osc 2 to Osc 1
        if (syncOn && (osc1PhaseA + (freq1 / sampleRate) >= 1.0f)) {
            osc2Phase = 0.0f;
        }

        float osc1Sample = 0.0f;
        float subSample = 0.0f;

        if (detVal > 0.5f && (shape1 == 1 || shape1 == 2))
        {
            // 8-Oscillator Supersaw Cluster
            float s1Sum = 0.0f;
            for (int d = 0; d < 8; ++d)
            {
                float dtD = (freq1 * osc1DetFactors[d]) / sampleRate;
                float readPhase = std::fmod(std::fmod(osc1DetunePhases[d] + fmModAmt * osc2Sample, 1.0f) + 1.0f, 1.0f);
                s1Sum += getOsc1Sample(readPhase, dtD, shape1, effectivePw);
                osc1DetunePhases[d] += dtD;
                if (osc1DetunePhases[d] >= 1.0f) osc1DetunePhases[d] -= std::floor(osc1DetunePhases[d]);
                if (osc1DetunePhases[d] < 0.0f)  osc1DetunePhases[d] += 1.0f;
            }
            osc1Sample = s1Sum * 0.125f;
        }
        else
        {
            float dt1 = freq1 / sampleRate;
            float readPhase = std::fmod(std::fmod(osc1PhaseA + fmModAmt * osc2Sample, 1.0f) + 1.0f, 1.0f);
            osc1Sample = getOsc1Sample(readPhase, dt1, shape1, effectivePw);

            osc1PhaseA += dt1;
            if (osc1PhaseA >= 1.0f) osc1PhaseA -= std::floor(osc1PhaseA);
            if (osc1PhaseA < 0.0f)  osc1PhaseA += 1.0f;
        }

        // Sub-oscillator: single clean fundamental anchor
        if (subGainVal > 0.001f) {
            float subDelta = (freq1 * subDetFactor) / sampleRate;
            float subReadPhase = std::fmod(std::fmod(subPhase + fmModAmt * osc2Sample, 1.0f) + 1.0f, 1.0f);
            subSample = getSubOscSample(subReadPhase, subShapeVal);
            subPhase += subDelta;
            if (subPhase >= 1.0f) subPhase -= 1.0f;
            if (subPhase < 0.0f)  subPhase += 1.0f;
        }

        osc2Phase += dt2;
        if (osc2Phase >= 1.0f) osc2Phase -= std::floor(osc2Phase);

        // Sub-oscillator & mix summing with gain compensation:
        float normalizer = 1.0f / ((1.0f + subGainVal) * (1.0f - mix) + mix);
        float osc2Out = ringOn ? (osc1Sample * osc2Sample) : osc2Sample;
        float oscOut = ((osc1Sample + subSample * subGainVal) * (1.0f - mix) + osc2Out * mix) * normalizer;

        // Unison sub-oscillators
        float oscOutL = oscOut;
        float oscOutR = oscOut;
        if (isUnison)
        {
            float uSumL = 0.0f, uSumR = 0.0f;
            for (int v = 0; v < uVoices; ++v)
            {
                float uFreq1 = baseFreq * uDetFactors[v];
                float uFixedBaseFreq = fixedBaseFreq * uDetFactors[v];
                float uFreq2 = kbdTrack2 ? (uFreq1 * std::pow(2.0f, (pitchSemi + pitchFine + osc2PitchMod + lfoPitchMod2) / 12.0f))
                                         : (uFixedBaseFreq * std::pow(2.0f, (pitchSemi + pitchFine + osc2PitchMod + lfoPitchMod2) / 12.0f));

                float uD2 = uFreq2 / sampleRate;
                float uS2 = getOsc2Sample(uOsc2Phase[v], uD2, shape2, effectivePw);
                float uFmD = effectiveFm * uS2 * (uFreq1 / sampleRate) * 1.5f;
                float uD1 = std::max(0.00001f, (uFreq1 / sampleRate) + uFmD);

                if (syncOn && (uOsc1Phase[v] + uD1 >= 1.0f)) uOsc2Phase[v] = 0.0f;
                float uS1 = getOsc1Sample(uOsc1Phase[v], uD1, shape1, effectivePw);

                uOsc1Phase[v] += uD1;
                if (uOsc1Phase[v] >= 1.0f) uOsc1Phase[v] -= std::floor(uOsc1Phase[v]);
                if (uOsc1Phase[v] < 0.0f)  uOsc1Phase[v] += 1.0f;

                uOsc2Phase[v] += uD2;
                if (uOsc2Phase[v] >= 1.0f) uOsc2Phase[v] -= std::floor(uOsc2Phase[v]);

                float uSample = ringOn ? (uS1 * (1.0f - mix) + (uS1 * uS2) * mix) : (uS1 * (1.0f - mix) + uS2 * mix);
                uSumL += uSample * uPanCos[v];
                uSumR += uSample * uPanSin[v];
            }
            float inv = 1.0f / std::sqrt((float)uVoices);
            oscOutL = (oscOut + uSumL * inv) * 0.5f;
            oscOutR = (oscOut + uSumR * inv) * 0.5f;
        }

        // Filter Architecture
        float inSample = (oscOutL + oscOutR) * 0.5f;

        // 1. Soft Saturation BEFORE filter (Param 22)
        float rawSat = (filterSat != nullptr) ? filterSat->load() : 0.0f;
        if (rawSat > 0.01f) {
            float k = (rawSat / 127.0f) * 6.0f;
            inSample = (1.0f + k) * inSample / (1.0f + k * std::abs(inSample));
        }

        // 2. Filter Envelope
        float fEnv = filterAdsr.getNextSample();
        bool fVelOn = (filterVel != nullptr && filterVel->load() > 0.5f);
        float effectiveFAmt = fVelOn ? (fAmtVal * currentNoteVelocity) : fAmtVal;

        // Filter cutoff calculation (1024-step table, C3 pivot tracking, asymmetric modulation)
        float rawCutoffVal = (filterCutoff != nullptr) ? filterCutoff->load() : 81.0f;
        float rawTrackVal  = (filterTrack != nullptr)  ? filterTrack->load()  : 64.0f;
        float currentPitchSemiFromC3 = 12.0f * std::log2(std::max(10.0f, currentGlideFreq) / 130.81278f) + pitchWheelBend;
        int virtualNote = 48 + (int)std::round(currentPitchSemiFromC3);

        float envScale = (fAmtVal != 0.0f) ? (effectiveFAmt / fAmtVal) : 1.0f;
        float cutoffHz = OpenSynthFilter::calculateCutoffHz(rawCutoffVal, virtualNote, rawTrackVal, rawFAmt, fEnv * envScale, lfoFilterMod);
        cutoffHz = juce::jlimit(15.0f, sampleRate * 0.48f, cutoffHz);

        // 3. Process filter (0: LP12, 1: LP24, 2: HP12, 3: BP12, 4: LPDL)
        float filtered = filter.processSample(inSample, cutoffHz, resVal, fType, sampleRate);

        // Amp Envelope & Gain scaling
        float ampEnvVal = ampAdsr.getNextSample();
        float rawAmpGain = (ampGain != nullptr) ? ampGain->load() : 92.0f;
        float gainVal = (rawAmpGain <= 0.001f) ? 0.0f : (std::pow(2048.0f, std::pow(rawAmpGain / 127.0f, 0.3f)) / 2048.0f);
        float finalSample = filtered * level * ampEnvVal * lfoAmpMod * gainVal * 0.45f;

        float voicePan = juce::jlimit(0.0f, 1.0f, 0.5f + lfoPanMod);
        float pL = std::cos(voicePan * juce::MathConstants<float>::halfPi);
        float pR = std::sin(voicePan * juce::MathConstants<float>::halfPi);

        if (outputBuffer.getNumChannels() > 1) {
            outputBuffer.addSample(0, i, finalSample * pL);
            outputBuffer.addSample(1, i, finalSample * pR);
        } else {
            outputBuffer.addSample(0, i, finalSample);
        }
    }

    if (!ampAdsr.isActive()) {
        activeVoicesMask.fetch_and(~(1u << (voiceIndex % 32)));
        filter.reset();
        clearCurrentNote();
    }
}

// -------------------------------------------------------------
// OpenSynth1AudioProcessor Implementation
// -------------------------------------------------------------
OpenSynth1AudioProcessor::OpenSynth1AudioProcessor()
     : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
       apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    initDefaultMidiCcMap();

    synth.addSound(new Synth1Sound());
    for (int i = 0; i < 32; ++i) { 
        synth.addVoice(new Synth1Voice(apvts, i, activeVoicesMask, currentHostBpm));
    }
    synth.setMinimumRenderingSubdivisionSize(16);

    delayBuffer.setSize(2, 96000);
    delayBuffer.clear();

    // Default Bank setup
    auto docsDir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("OpenSynth1/soundbank");
    juce::File presetDir = docsDir.exists() ? docsDir : juce::File::getCurrentWorkingDirectory().getChildFile("soundbank");
    setBankDirectory(presetDir);
}

OpenSynth1AudioProcessor::~OpenSynth1AudioProcessor() {}

const juce::String OpenSynth1AudioProcessor::getName() const { return "OpenSynth1"; }
bool OpenSynth1AudioProcessor::acceptsMidi() const { return true; }
bool OpenSynth1AudioProcessor::producesMidi() const { return false; }
bool OpenSynth1AudioProcessor::isMidiEffect() const { return false; }
double OpenSynth1AudioProcessor::getTailLengthSeconds() const { return 0.0; }
int OpenSynth1AudioProcessor::getNumPrograms() { return 128; }
int OpenSynth1AudioProcessor::getCurrentProgram() { return currentProgramIndex - 1; }
void OpenSynth1AudioProcessor::setCurrentProgram (int index) { selectBankAndProgram(currentBankIndex, index + 1); }
const juce::String OpenSynth1AudioProcessor::getProgramName (int) { return currentProgramName; }
void OpenSynth1AudioProcessor::changeProgramName (int, const juce::String& newName) { currentProgramName = newName; }

void OpenSynth1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) 
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    delayBuffer.setSize(2, (int)(sampleRate * 2.5));
    delayBuffer.clear();
    delayWritePos = 0;

    chorusBuffer.setSize(2, (int)(sampleRate * 0.1));
    chorusBuffer.clear();
    chorusWritePos = 0;
    chorusPhase = 0.0f;

    phaserPhase = 0.0f;
    phaserFeedbackL = 0.0f;
    phaserFeedbackR = 0.0f;
    phaserAPL.fill(0.0f);
    phaserAPR.fill(0.0f);

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
bool OpenSynth1AudioProcessor::isBusesLayoutSupported (const BusesLayout&) const { return true; }

void OpenSynth1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) 
{ 
    juce::ScopedNoDenormals noDenormals;

    if (auto* p72 = apvts.getRawParameterValue("72")) {
        masterTuneCents.store((p72->load() - 64.0f) * (100.0f / 64.0f));
    }

    if (resetVoicesPending.exchange(false)) {
        synth.allNotesOff(0, false);
        for (int v = 0; v < synth.getNumVoices(); ++v) {
            if (auto* sv = dynamic_cast<Synth1Voice*>(synth.getVoice(v))) {
                sv->resetVoiceState();
            }
        }
        monoHeldKeys.clear();
        currentMonoSoundingNote = -1;
        delayBuffer.clear();
        chorusBuffer.clear();
        eqX1L = eqX2L = eqY1L = eqY2L = 0.0f;
        eqX1R = eqX2R = eqY1R = eqY2R = 0.0f;
    }

    // Handle MIDI CC messages & MIDI Learn
    for (const auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();
        if (msg.isController())
        {
            int cc = msg.getControllerNumber();
            int val = msg.getControllerValue();

            if (midiLearnActive.load())
            {
                int targetParam = midiLearnTargetParam.load();
                if (targetParam >= 0)
                {
                    setMidiCcMapping(cc, targetParam);
                    lastMidiLearnMessage = "Mapped CC #" + juce::String(cc) + " to ID " + juce::String(targetParam);
                    midiLearnActive = false;
                }
            }
            else
            {
                // Normal CC mapped parameter change
                int paramId = getParamForMidiCc(cc);
                if (paramId >= 0)
                {
                    if (auto* p = dynamic_cast<juce::RangedAudioParameter*>(apvts.getParameter(juce::String(paramId))))
                    {
                        float norm = p->getNormalisableRange().convertTo0to1((float)val);
                        p->setValue(norm);
                    }
                }
            }
        }
    }

    buffer.clear();

    // -----------------------------------------------------------------
    // Arpeggiator Engine
    // -----------------------------------------------------------------
    juce::MidiBuffer synthMidi;
    bool arpOn = (apvts.getRawParameterValue("59") != nullptr && apvts.getRawParameterValue("59")->load() > 0.5f);

    if (auto* ph = getPlayHead()) {
        if (auto pos = ph->getPosition()) {
            if (pos->getBpm().hasValue()) currentHostBpm.store((float)*pos->getBpm());
        }
    }
    float hBpm = currentHostBpm.load();
    if (hBpm < 20.0f || hBpm > 400.0f) {
        hBpm = 120.0f;
        currentHostBpm.store(120.0f);
    }

    if (arpOn)
    {
        static const double beatDivisions[] = {
            4.0,     // 0: Whole note
            3.0,     // 1: Half dotted
            2.0,     // 2: Half note
            1.3333,  // 3: Half triplet
            1.5,     // 4: Quarter dotted
            1.0,     // 5: Quarter note
            0.6667,  // 6: Quarter triplet
            0.75,    // 7: 8th dotted
            0.5,     // 8: 8th note
            0.3333,  // 9: 8th triplet
            0.375,   // 10: 16th dotted
            0.25,    // 11: 16th note (default Synth1 value)
            0.1667,  // 12: 16th triplet
            0.1875,  // 13: 32nd dotted
            0.125,   // 14: 32nd note
            0.0833,  // 15: 32nd triplet
            0.0625   // 16: 64th note
        };

        float beatVal = (apvts.getRawParameterValue("33") != nullptr) ? apvts.getRawParameterValue("33")->load() : 64.0f;
        int divIdx = juce::jlimit(0, 16, (int)std::round(beatVal / 127.0f * 16.0f));
        double beatsPerStep = beatDivisions[divIdx];
        arpSamplesPerStep = (60.0 / currentHostBpm) * beatsPerStep * getSampleRate();
        if (arpSamplesPerStep < 50.0) arpSamplesPerStep = 50.0;

        float gateVal = (apvts.getRawParameterValue("34") != nullptr) ? (apvts.getRawParameterValue("34")->load() / 127.0f) : 0.6f;
        gateVal = juce::jlimit(0.05f, 0.95f, gateVal * 0.9f + 0.05f);
        int gateLen = juce::jmax(10, (int)(arpSamplesPerStep * gateVal));

        for (const auto metadata : midiMessages)
        {
            auto msg = metadata.getMessage();
            if (msg.isNoteOn())
            {
                int note = msg.getNoteNumber();
                bool wasEmpty = arpHeldKeys.empty();
                if (std::find(arpHeldKeys.begin(), arpHeldKeys.end(), note) == arpHeldKeys.end())
                {
                    arpHeldKeys.push_back(note);
                    std::sort(arpHeldKeys.begin(), arpHeldKeys.end());
                }
                if (wasEmpty)
                {
                    arpPatternIndex = 0;
                    arpStepCounter = arpSamplesPerStep; // Immediate trigger
                }
            }
            else if (msg.isNoteOff())
            {
                int note = msg.getNoteNumber();
                auto it = std::find(arpHeldKeys.begin(), arpHeldKeys.end(), note);
                if (it != arpHeldKeys.end()) arpHeldKeys.erase(it);
            }
            else
            {
                synthMidi.addEvent(msg, metadata.samplePosition);
            }
        }

        if (arpHeldKeys.empty())
        {
            if (arpNotePlaying >= 0)
            {
                synthMidi.addEvent(juce::MidiMessage::noteOff(1, arpNotePlaying), 0);
                arpNotePlaying = -1;
            }
            arpPattern.clear();
            arpPatternIndex = 0;
            arpStepCounter = 999999.0;
            arpGateSamplesRemaining = 0;
        }
        else
        {
            int octRange = (apvts.getRawParameterValue("32") != nullptr) ? juce::jlimit(0, 3, (int)apvts.getRawParameterValue("32")->load()) : 0;
            int arpType = (apvts.getRawParameterValue("31") != nullptr) ? juce::jlimit(0, 3, (int)apvts.getRawParameterValue("31")->load()) : 0;

            std::vector<int> allNotes;
            for (int oct = 0; oct <= octRange; ++oct)
            {
                for (int n : arpHeldKeys)
                {
                    int pitch = n + oct * 12;
                    if (pitch <= 127) allNotes.push_back(pitch);
                }
            }

            arpPattern.clear();
            if (arpType == 0) // Up/Down
            {
                for (int n : allNotes) arpPattern.push_back(n);
                for (int idx = (int)allNotes.size() - 2; idx > 0; --idx) arpPattern.push_back(allNotes[idx]);
            }
            else if (arpType == 1) // Up
            {
                for (int n : allNotes) arpPattern.push_back(n);
            }
            else if (arpType == 2) // Down
            {
                for (auto it = allNotes.rbegin(); it != allNotes.rend(); ++it) arpPattern.push_back(*it);
            }
            else if (arpType == 3) // Random
            {
                arpPattern = allNotes;
            }
            if (arpPattern.empty()) arpPattern = arpHeldKeys;

            for (int smp = 0; smp < buffer.getNumSamples(); ++smp)
            {
                arpStepCounter += 1.0;
                if (arpStepCounter >= arpSamplesPerStep)
                {
                    arpStepCounter = 0.0;
                    if (arpNotePlaying >= 0)
                    {
                        synthMidi.addEvent(juce::MidiMessage::noteOff(1, arpNotePlaying), smp);
                        arpNotePlaying = -1;
                    }

                    if (!arpPattern.empty())
                    {
                        int note = 60;
                        if (arpType == 3) {
                            note = arpPattern[(size_t)juce::Random::getSystemRandom().nextInt((int)arpPattern.size())];
                        } else {
                            if (arpPatternIndex >= (int)arpPattern.size()) arpPatternIndex = 0;
                            note = arpPattern[(size_t)arpPatternIndex];
                            arpPatternIndex = (arpPatternIndex + 1) % (int)arpPattern.size();
                        }

                        synthMidi.addEvent(juce::MidiMessage::noteOn(1, note, (juce::uint8)100), smp);
                        arpNotePlaying = note;
                        arpGateSamplesRemaining = gateLen;
                    }
                }

                if (arpGateSamplesRemaining > 0)
                {
                    --arpGateSamplesRemaining;
                    if (arpGateSamplesRemaining == 0 && arpNotePlaying >= 0)
                    {
                        synthMidi.addEvent(juce::MidiMessage::noteOff(1, arpNotePlaying), smp);
                        arpNotePlaying = -1;
                    }
                }
            }
        }
    }
    else
    {
        arpHeldKeys.clear();
        arpNotePlaying = -1;

        int playModeVal = (apvts.getRawParameterValue("38") != nullptr) ? (int)apvts.getRawParameterValue("38")->load() : 0;
        if (playModeVal == 0) // Poly
        {
            synthMidi.addEvents(midiMessages, 0, buffer.getNumSamples(), 0);
            monoHeldKeys.clear();
            currentMonoSoundingNote = -1;
        }
        else // 1: Mono, 2: Legato
        {
            for (const auto metadata : midiMessages)
            {
                auto msg = metadata.getMessage();
                int smp = metadata.samplePosition;

                if (msg.isNoteOn())
                {
                    int note = msg.getNoteNumber();
                    float vel = msg.getFloatVelocity();

                    monoHeldKeys.erase(std::remove_if(monoHeldKeys.begin(), monoHeldKeys.end(),
                                                      [note](const MonoHeldNote& n) { return n.noteNumber == note; }),
                                       monoHeldKeys.end());
                    monoHeldKeys.push_back({ note, vel });

                    if (currentMonoSoundingNote < 0)
                    {
                        synthMidi.addEvent(juce::MidiMessage::noteOn(msg.getChannel(), note, msg.getVelocity()), smp);
                        currentMonoSoundingNote = note;
                    }
                    else
                    {
                        currentMonoSoundingNote = note;
                        if (playModeVal == 1) // Mono: retrigger envelopes
                        {
                            if (auto* v0 = dynamic_cast<Synth1Voice*>(synth.getVoice(0)))
                                v0->retriggerMonoNote(note, vel);
                        }
                        else // Legato: smooth pitch glide without retriggering envelopes
                        {
                            if (auto* v0 = dynamic_cast<Synth1Voice*>(synth.getVoice(0)))
                                v0->updateLegatoNote(note, vel);
                        }
                    }
                }
                else if (msg.isNoteOff())
                {
                    int note = msg.getNoteNumber();
                    monoHeldKeys.erase(std::remove_if(monoHeldKeys.begin(), monoHeldKeys.end(),
                                                      [note](const MonoHeldNote& n) { return n.noteNumber == note; }),
                                       monoHeldKeys.end());

                    if (note == currentMonoSoundingNote)
                    {
                        if (monoHeldKeys.empty())
                        {
                            synthMidi.addEvent(juce::MidiMessage::noteOff(msg.getChannel(), note, msg.getVelocity()), smp);
                            currentMonoSoundingNote = -1;
                        }
                        else
                        {
                            auto prev = monoHeldKeys.back();
                            currentMonoSoundingNote = prev.noteNumber;
                            if (playModeVal == 1) // Mono
                            {
                                if (auto* v0 = dynamic_cast<Synth1Voice*>(synth.getVoice(0)))
                                    v0->retriggerMonoNote(prev.noteNumber, prev.velocity);
                            }
                            else // Legato
                            {
                                if (auto* v0 = dynamic_cast<Synth1Voice*>(synth.getVoice(0)))
                                    v0->updateLegatoNote(prev.noteNumber, prev.velocity);
                            }
                        }
                    }
                }
                else if (msg.isAllNotesOff() || msg.isAllSoundOff())
                {
                    monoHeldKeys.clear();
                    currentMonoSoundingNote = -1;
                    synthMidi.addEvent(msg, smp);
                }
                else
                {
                    synthMidi.addEvent(msg, smp);
                }
            }
        }
    }

    float sRate = (float)getSampleRate();
    if (sRate <= 0.0f) sRate = 44100.0f;

    // Advance global processor LFOs (for voices with Key Sync = Off)
    float lfo1Spd = (apvts.getRawParameterValue("43") != nullptr) ? (apvts.getRawParameterValue("43")->load() / 127.0f) : 0.5f;
    float lfo2Spd = (apvts.getRawParameterValue("48") != nullptr) ? (apvts.getRawParameterValue("48")->load() / 127.0f) : 0.5f;
    bool lfo1Sync = (apvts.getRawParameterValue("67") != nullptr && apvts.getRawParameterValue("67")->load() > 0.5f);
    bool lfo2Sync = (apvts.getRawParameterValue("69") != nullptr && apvts.getRawParameterValue("69")->load() > 0.5f);

    static const float lfoSyncBeats[] = {
        64.0f, 32.0f, 16.0f, 8.0f, 4.0f, 3.0f, 2.0f, 1.3333f,
        1.5f, 1.0f, 0.6667f, 0.75f, 0.5f, 0.3333f, 0.375f, 0.25f, 0.125f
    };
    float lfo1Hz = 0.1f * std::pow(300.0f, lfo1Spd);
    if (lfo1Sync) {
        int divIdx = juce::jlimit(0, 16, (int)std::round(lfo1Spd * 16.0f));
        float beatSec = 60.0f / hBpm;
        float cycleSec = beatSec * lfoSyncBeats[divIdx];
        if (cycleSec > 0.001f) lfo1Hz = 1.0f / cycleSec;
    }
    float lfo2Hz = 0.1f * std::pow(300.0f, lfo2Spd);
    if (lfo2Sync) {
        int divIdx = juce::jlimit(0, 16, (int)std::round(lfo2Spd * 16.0f));
        float beatSec = 60.0f / hBpm;
        float cycleSec = beatSec * lfoSyncBeats[divIdx];
        if (cycleSec > 0.001f) lfo2Hz = 1.0f / cycleSec;
    }

    float lfo1Delta = (lfo1Hz / sRate) * (float)buffer.getNumSamples();
    float lfo2Delta = (lfo2Hz / sRate) * (float)buffer.getNumSamples();

    processorLfo1Phase += lfo1Delta;
    if (processorLfo1Phase >= 1.0f) {
        processorLfo1Phase -= std::floor(processorLfo1Phase);
        processorLfo1Sh = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
    }
    processorLfo2Phase += lfo2Delta;
    if (processorLfo2Phase >= 1.0f) {
        processorLfo2Phase -= std::floor(processorLfo2Phase);
        processorLfo2Sh = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
    }

    for (int v = 0; v < synth.getNumVoices(); ++v) {
        if (auto* sv = dynamic_cast<Synth1Voice*>(synth.getVoice(v))) {
            sv->setGlobalLfoState(processorLfo1Phase, processorLfo2Phase, processorLfo1Sh, processorLfo2Sh);
        }
    }

    // Process Synth voices
    synth.renderNextBlock(buffer, synthMidi, 0, buffer.getNumSamples());

    // Post-processing DSP: Effects, EQ, Delay, Chorus, Volume, Pan

    // 1. Effects
    bool effectOn  = (apvts.getRawParameterValue("77") != nullptr && apvts.getRawParameterValue("77")->load() > 0.5f);
    int fxType     = (apvts.getRawParameterValue("78") != nullptr) ? (int)apvts.getRawParameterValue("78")->load() : 0;
    float fxCtl1   = (apvts.getRawParameterValue("79") != nullptr) ? (apvts.getRawParameterValue("79")->load() / 127.0f) : 0.5f;
    float fxCtl2   = (apvts.getRawParameterValue("80") != nullptr) ? (apvts.getRawParameterValue("80")->load() / 127.0f) : 0.5f;
    float fxLevel  = (apvts.getRawParameterValue("81") != nullptr) ? (apvts.getRawParameterValue("81")->load() / 127.0f) : 0.5f;

    // 2. EQ parameters
    float eqToneVal  = (apvts.getRawParameterValue("60") != nullptr) ? ((apvts.getRawParameterValue("60")->load() - 64.0f) / 64.0f) : 0.0f;
    float eqFreqVal  = (apvts.getRawParameterValue("61") != nullptr) ? (apvts.getRawParameterValue("61")->load() / 127.0f) : 0.5f;
    float eqGainVal  = (apvts.getRawParameterValue("62") != nullptr) ? ((apvts.getRawParameterValue("62")->load() - 64.0f) / 64.0f * 12.0f) : 0.0f;
    float eqQVal     = (apvts.getRawParameterValue("63") != nullptr) ? (0.5f + apvts.getRawParameterValue("63")->load() / 127.0f * 4.0f) : 1.0f;

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

    // 3. Chorus / Flanger parameters
    bool chorusOn = (apvts.getRawParameterValue("66") != nullptr && apvts.getRawParameterValue("66")->load() > 0.5f);
    int chType    = (apvts.getRawParameterValue("64") != nullptr) ? (int)apvts.getRawParameterValue("64")->load() : 1;
    float chTime  = (apvts.getRawParameterValue("52") != nullptr) ? (0.001f + apvts.getRawParameterValue("52")->load() / 127.0f * 0.025f) : 0.01f;
    float chDepth = (apvts.getRawParameterValue("53") != nullptr) ? (apvts.getRawParameterValue("53")->load() / 127.0f * 0.008f) : 0.003f;
    float chRate  = (apvts.getRawParameterValue("54") != nullptr) ? (0.1f + apvts.getRawParameterValue("54")->load() / 127.0f * 5.0f) : 1.5f;
    float chFdbk  = (apvts.getRawParameterValue("55") != nullptr) ? ((apvts.getRawParameterValue("55")->load() - 64.0f) / 64.0f * 0.8f) : 0.0f;
    float chLevel = (apvts.getRawParameterValue("56") != nullptr) ? (apvts.getRawParameterValue("56")->load() / 127.0f) : 0.4f;

    // 4. Delay parameters
    bool delayOn     = (apvts.getRawParameterValue("65") != nullptr && apvts.getRawParameterValue("65")->load() > 0.5f);
    int delayType    = (apvts.getRawParameterValue("82") != nullptr) ? (int)apvts.getRawParameterValue("82")->load() : 0;
    int dIdx         = (apvts.getRawParameterValue("35") != nullptr) ? juce::jlimit(0, 19, (int)std::round(apvts.getRawParameterValue("35")->load())) : 8;
    float delaySprdVal = (apvts.getRawParameterValue("83") != nullptr) ? ((apvts.getRawParameterValue("83")->load() - 64.0f) / 64.0f) : 0.0f;
    float delayFdbkVal = (apvts.getRawParameterValue("36") != nullptr) ? (apvts.getRawParameterValue("36")->load() / 127.0f * 0.95f) : 0.4f;
    float delayToneVal = (apvts.getRawParameterValue("98") != nullptr) ? (apvts.getRawParameterValue("98")->load() / 127.0f) : 0.5f;
    float delayDwVal   = (apvts.getRawParameterValue("37") != nullptr) ? (apvts.getRawParameterValue("37")->load() / 127.0f) : 0.3f;

    static const float delaySyncBeats[] = {
        0.001f,  // 0: 0.1ms
        0.0833f, // 1: 1/32T
        0.125f,  // 2: 1/32
        0.1667f, // 3: 1/16T
        0.1875f, // 4: 1/32.
        0.25f,   // 5: 1/16
        0.3333f, // 6: 1/8T
        0.375f,  // 7: 1/16.
        0.5f,    // 8: 1/8
        0.6667f, // 9: 1/4T
        0.75f,   // 10: 1/8.
        1.0f,    // 11: 1/4
        1.3333f, // 12: 1/2T
        1.5f,    // 13: 1/4.
        2.0f,    // 14: 1/2
        2.6667f, // 15: 1T
        3.0f,    // 16: 1/2.
        4.0f,    // 17: 1
        8.0f,    // 18: 2
        16.0f    // 19: 4
    };
    float beatSec = 60.0f / hBpm;
    float dSec = (dIdx == 0) ? 0.001f : beatSec * delaySyncBeats[dIdx];
    float spreadOffsetSec = delaySprdVal * 0.04f; // up to +/-40ms stereo offset
    int delayLenL = juce::jlimit(10, 96000, (int)((dSec - spreadOffsetSec * 0.5f) * sRate));
    int delayLenR = juce::jlimit(10, 96000, (int)((dSec + spreadOffsetSec * 0.5f) * sRate));
    int dBufLen = delayBuffer.getNumSamples();
    int cBufLen = chorusBuffer.getNumSamples();

    // Delay lowpass damping filter coefficient (Tone: 0=dark, 127=bright)
    float dampCutoffHz = 600.0f * std::pow(25.0f, delayToneVal); // 600Hz to 15kHz
    float dampCoef = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * dampCutoffHz / sRate);
    dampCoef = juce::jlimit(0.02f, 0.98f, dampCoef);

    // 5. Volume & Pan
    float masterVolNorm = (apvts.getRawParameterValue("107") != nullptr) ? (apvts.getRawParameterValue("107")->load() / 127.0f) : 1.0f;
    float panVal = (apvts.getRawParameterValue("90") != nullptr) ? (apvts.getRawParameterValue("90")->load() / 127.0f) : 0.5f;
    float panL = std::cos(panVal * juce::MathConstants<float>::halfPi);
    float panR = std::sin(panVal * juce::MathConstants<float>::halfPi);

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        float left = buffer.getSample(0, i);
        float right = (buffer.getNumChannels() > 1) ? buffer.getSample(1, i) : left;

        // Equalizer processing
        if (std::abs(eqGainVal) > 0.01f)
        {
            float yL = b0 * left + b1 * eqX1L + b2 * eqX2L - a1 * eqY1L - a2 * eqY2L;
            eqX2L = eqX1L; eqX1L = left; eqY2L = eqY1L; eqY1L = yL;
            left = yL;

            float yR = b0 * right + b1 * eqX1R + b2 * eqX2R - a1 * eqY1R - a2 * eqY2R;
            eqX2R = eqX1R; eqX1R = right; eqY2R = eqY1R; eqY1R = yR;
            right = yR;
        }

        // Effect Unit processing
        if (effectOn)
        {
            float dryL = left, dryR = right;
            float wetL = left, wetR = right;

            switch (fxType)
            {
                case 0: // a.d.1: Analog distortion 1 (warm drive + tone damping)
                    wetL = std::tanh(left * (1.0f + fxCtl1 * 8.0f)) * (1.0f / (1.0f + fxCtl1 * 0.25f));
                    wetR = std::tanh(right * (1.0f + fxCtl1 * 8.0f)) * (1.0f / (1.0f + fxCtl1 * 0.25f));
                    break;

                case 1: // a.d.2: Analog distortion 2 (asymmetrical soft clip)
                    wetL = ((left > 0.0f) ? std::tanh(left * (1.0f + fxCtl1 * 10.0f)) : (std::tanh(left * (1.0f + fxCtl1 * 5.0f)) * 0.8f)) * (1.0f / (1.0f + fxCtl1 * 0.25f));
                    wetR = ((right > 0.0f) ? std::tanh(right * (1.0f + fxCtl1 * 10.0f)) : (std::tanh(right * (1.0f + fxCtl1 * 5.0f)) * 0.8f)) * (1.0f / (1.0f + fxCtl1 * 0.25f));
                    break;

                case 2: // d.d.: Digital Distortion (foldback)
                    wetL = std::sin(left * (1.0f + fxCtl1 * 6.0f));
                    wetR = std::sin(right * (1.0f + fxCtl1 * 6.0f));
                    break;

                case 3: // deci: BitCrusher / Decimator
                {
                    int decHold = 1 + (int)(fxCtl1 * 20.0f);
                    if (++decimCounter >= decHold) {
                        decimCounter = 0;
                        decimHeldL = left;
                        decimHeldR = right;
                    }
                    float bits = std::pow(2.0f, (float)(int)(2.0f + (1.0f - fxCtl2) * 12.0f));
                    wetL = std::round(decimHeldL * bits) / bits;
                    wetR = std::round(decimHeldR * bits) / bits;
                    break;
                }

                case 4: // r.m.: Ring Modulator
                {
                    rmPhase += (30.0f + fxCtl1 * 800.0f) * 2.0f * juce::MathConstants<float>::pi / sRate;
                    if (rmPhase >= juce::MathConstants<float>::twoPi) rmPhase -= juce::MathConstants<float>::twoPi;
                    float carrier = std::sin(rmPhase);
                    wetL = left * carrier;
                    wetR = right * carrier;
                    break;
                }

                case 5: // comp.: Compressor
                {
                    float envIn = std::max(std::abs(left), std::abs(right));
                    float att = 0.005f + fxCtl2 * 0.05f;
                    compEnvFollower += (envIn - compEnvFollower) * (1.0f / (att * sRate));
                    float thresh = 0.5f - fxCtl1 * 0.45f;
                    float gainRed = (compEnvFollower > thresh) ? std::max(0.1f, thresh / compEnvFollower) : 1.0f;
                    wetL = left * gainRed;
                    wetR = right * gainRed;
                    break;
                }

                case 6: case 7: case 8: case 9: // ph.1 .. ph.4 (Phaser stages 1, 2, 4, 6)
                {
                    phaserPhase += (0.1f + fxCtl2 * 3.0f) * 2.0f * juce::MathConstants<float>::pi / sRate;
                    if (phaserPhase >= juce::MathConstants<float>::twoPi) phaserPhase -= juce::MathConstants<float>::twoPi;
                    float mod = (std::sin(phaserPhase) + 1.0f) * 0.5f * fxCtl1;
                    float apG = (1.0f - mod * 0.8f) / (1.0f + mod * 0.8f);

                    int numStages = (fxType == 6 ? 1 : (fxType == 7 ? 2 : (fxType == 8 ? 4 : 6)));
                    float inL = left + phaserFeedbackL * 0.6f;
                    float inR = right + phaserFeedbackR * 0.6f;
                    for (int st = 0; st < numStages; ++st) {
                        float outL = apG * inL + phaserAPL[st] - apG * phaserAPL[st];
                        phaserAPL[st] = inL;
                        inL = outL;

                        float outR = apG * inR + phaserAPR[st] - apG * phaserAPR[st];
                        phaserAPR[st] = inR;
                        inR = outR;
                    }
                    phaserFeedbackL = inL;
                    phaserFeedbackR = inR;
                    wetL = (left + inL) * 0.5f;
                    wetR = (right + inR) * 0.5f;
                    break;
                }
            }

            left = dryL * (1.0f - fxLevel) + wetL * fxLevel;
            right = dryR * (1.0f - fxLevel) + wetR * fxLevel;
        }

        // -------------------------------------------------------------
        // 3. Tempo Delay processing (runs BEFORE Chorus, exactly like Synth1)
        // -------------------------------------------------------------
        if (delayOn && dBufLen > 0)
        {
            int clampedLenL = juce::jlimit(1, dBufLen - 1, delayLenL);
            int clampedLenR = juce::jlimit(1, dBufLen - 1, delayLenR);
            int readL = ((delayWritePos - clampedLenL) % dBufLen + dBufLen) % dBufLen;
            int readR = ((delayWritePos - clampedLenR) % dBufLen + dBufLen) % dBufLen;

            float dl = delayBuffer.getSample(0, readL);
            float dr = delayBuffer.getSample(1, readR);
            if (!std::isfinite(dl)) dl = 0.0f;
            if (!std::isfinite(dr)) dr = 0.0f;

            // Low-pass damping governed by Delay Tone (param 98)
            delayFilterStateL = delayFilterStateL + dampCoef * (dl - delayFilterStateL);
            delayFilterStateR = delayFilterStateR + dampCoef * (dr - delayFilterStateR);
            float fdl = delayFilterStateL;
            float fdr = delayFilterStateR;

            if (delayType == 1) // Cross
            {
                delayBuffer.setSample(0, delayWritePos, juce::jlimit(-3.0f, 3.0f, left + fdr * delayFdbkVal));
                delayBuffer.setSample(1, delayWritePos, juce::jlimit(-3.0f, 3.0f, right + fdl * delayFdbkVal));
            }
            else if (delayType == 2) // Ping-Pong
            {
                delayBuffer.setSample(0, delayWritePos, juce::jlimit(-3.0f, 3.0f, left + fdr * delayFdbkVal));
                delayBuffer.setSample(1, delayWritePos, juce::jlimit(-3.0f, 3.0f, fdl * delayFdbkVal));
            }
            else // Stereo ST
            {
                delayBuffer.setSample(0, delayWritePos, juce::jlimit(-3.0f, 3.0f, left + fdl * delayFdbkVal));
                delayBuffer.setSample(1, delayWritePos, juce::jlimit(-3.0f, 3.0f, right + fdr * delayFdbkVal));
            }

            // Crossfade taper:
            // 0..64: dry = 1.0, wet = (val - 1) / 63.0
            // 64..127: wet = 1.0, dry = (127 - val) / 63.0
            float rawDw = (apvts.getRawParameterValue("37") != nullptr) ? apvts.getRawParameterValue("37")->load() : 38.0f;
            float delayDry = (rawDw <= 64.0f) ? 1.0f : ((127.0f - rawDw) / 63.0f);
            float delayWet = ((rawDw <= 64.0f) ? (std::max(0.0f, rawDw - 1.0f) / 63.0f) : 1.0f) * 0.75f;

            left = left * delayDry + fdl * delayWet;
            right = right * delayDry + fdr * delayWet;

            delayWritePos = (delayWritePos + 1) % dBufLen;
        }

        // -------------------------------------------------------------
        // 4. Chorus / Flanger processing (runs AFTER Delay, diffusing echoes into lush stereo)
        // -------------------------------------------------------------
        if (chorusOn && cBufLen > 0)
        {
            chorusPhase += chRate * 2.0f * juce::MathConstants<float>::pi / sRate;
            if (chorusPhase >= juce::MathConstants<float>::twoPi) chorusPhase -= juce::MathConstants<float>::twoPi;

            auto readChorusInterp = [&](int channel, float delaySmp) -> float {
                float readPos = (float)chorusWritePos - delaySmp;
                while (readPos < 0.0f) readPos += (float)cBufLen;
                while (readPos >= (float)cBufLen) readPos -= (float)cBufLen;
                int idx0 = (int)readPos;
                int idx1 = (idx0 + 1) % cBufLen;
                float frac = readPos - (float)idx0;
                return (1.0f - frac) * chorusBuffer.getSample(channel, idx0) + frac * chorusBuffer.getSample(channel, idx1);
            };

            float chOutL = 0.0f, chOutR = 0.0f;

            if (chType == 0) // x1: 1 voice mono flanger/chorus
            {
                float mod = (std::sin(chorusPhase) + 1.0f) * 0.5f;
                float dSmp = juce::jlimit(1.0f, (float)(cBufLen - 2), (chTime + mod * chDepth) * sRate);
                chOutL = readChorusInterp(0, dSmp);
                chOutR = chOutL;
            }
            else if (chType == 1) // x2: 2 voice stereo chorus (90° quadrature phase offset)
            {
                float modL = (std::cos(chorusPhase) + 1.0f) * 0.5f;
                float modR = (std::sin(chorusPhase) + 1.0f) * 0.5f;

                chOutL = readChorusInterp(0, juce::jlimit(1.0f, (float)(cBufLen - 2), (chTime + modL * chDepth) * sRate));
                chOutR = readChorusInterp(1, juce::jlimit(1.0f, (float)(cBufLen - 2), (chTime + modR * chDepth) * sRate));
            }
            else // x4: 4 voice quadrature ensemble chorus (Synth1 signature lush strings mode)
            {
                float mod1 = (std::sin(chorusPhase) + 1.0f) * 0.5f;
                float mod2 = (std::sin(chorusPhase + juce::MathConstants<float>::halfPi) + 1.0f) * 0.5f;
                float mod3 = (std::sin(chorusPhase + juce::MathConstants<float>::pi) + 1.0f) * 0.5f;
                float mod4 = (std::sin(chorusPhase + juce::MathConstants<float>::pi * 1.5f) + 1.0f) * 0.5f;

                float v1 = readChorusInterp(0, juce::jlimit(1.0f, (float)(cBufLen - 2), (chTime * 0.85f + mod1 * chDepth) * sRate));
                float v2 = readChorusInterp(1, juce::jlimit(1.0f, (float)(cBufLen - 2), (chTime * 1.00f + mod2 * chDepth) * sRate));
                float v3 = readChorusInterp(0, juce::jlimit(1.0f, (float)(cBufLen - 2), (chTime * 1.15f + mod3 * chDepth) * sRate));
                float v4 = readChorusInterp(1, juce::jlimit(1.0f, (float)(cBufLen - 2), (chTime * 1.30f + mod4 * chDepth) * sRate));

                chOutL = (v1 + v3) * 0.5f;
                chOutR = (v2 + v4) * 0.5f;
            }

            if (!std::isfinite(chOutL)) chOutL = 0.0f;
            if (!std::isfinite(chOutR)) chOutR = 0.0f;

            chorusBuffer.setSample(0, chorusWritePos, juce::jlimit(-3.0f, 3.0f, left + chOutL * chFdbk));
            chorusBuffer.setSample(1, chorusWritePos, juce::jlimit(-3.0f, 3.0f, right + chOutR * chFdbk));
            chorusWritePos = (chorusWritePos + 1) % cBufLen;

            // Balanced blend to prevent sudden volume boost on chorus activation
            float dryGain = 1.0f - chLevel * 0.25f;
            float wetGain = chLevel * 0.70f;
            left  = left  * dryGain + chOutL * wetGain;
            right = right * dryGain + chOutR * wetGain;
        }

        if (!std::isfinite(left)) left = 0.0f;
        if (!std::isfinite(right)) right = 0.0f;
        left  = juce::jlimit(-3.0f, 3.0f, left);
        right = juce::jlimit(-3.0f, 3.0f, right);

        left  *= masterVolNorm * panL * 1.25f;
        right *= masterVolNorm * panR * 1.25f;

        // Master analog-style transparent soft saturation to eliminate DAW digital clipping
        left  = std::tanh(left * 0.9f) * 1.11f;
        right = std::tanh(right * 0.9f) * 1.11f;

        buffer.setSample(0, i, left);
        if (buffer.getNumChannels() > 1) {
            buffer.setSample(1, i, right);
        }
    }
}

bool OpenSynth1AudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* OpenSynth1AudioProcessor::createEditor() { return new OpenSynth1AudioProcessorEditor (*this); }

void OpenSynth1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void OpenSynth1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

// -------------------------------------------------------------
// Preset & Bank Management
// -------------------------------------------------------------
void OpenSynth1AudioProcessor::setBankDirectory(const juce::File& dir)
{
    currentBankDir = dir;
    scanBanks();
}

void OpenSynth1AudioProcessor::scanBanks()
{
    banks.clear();
    if (!currentBankDir.exists() || !currentBankDir.isDirectory()) return;

    auto subDirs = currentBankDir.findChildFiles(juce::File::findDirectories, false);
    subDirs.sort();

    int bNum = 0;
    for (auto& dir : subDirs)
    {
        Synth1BankInfo bInfo;
        bInfo.bankNumber = bNum++;
        bInfo.name = dir.getFileName();
        bInfo.directory = dir;

        for (int pIdx = 1; pIdx <= 128; ++pIdx)
        {
            juce::String fileName = juce::String::formatted("%03d.sy1", pIdx);
            juce::File pFile = dir.getChildFile(fileName);

            Synth1ProgramInfo pInfo;
            pInfo.index = pIdx;
            pInfo.file = pFile;
            pInfo.name = "Empty";
            pInfo.color = "red";

            if (pFile.existsAsFile())
            {
                juce::StringArray lines;
                pFile.readLines(lines);
                if (lines.size() > 0) pInfo.name = lines[0].trim();
                if (lines.size() > 1 && lines[1].startsWith("color=")) {
                    pInfo.color = lines[1].fromFirstOccurrenceOf("color=", false, false).trim();
                }
            }
            bInfo.programs.push_back(pInfo);
        }
        banks.push_back(bInfo);
    }

    if (banks.empty())
    {
        // Add default empty bank
        Synth1BankInfo defaultBank;
        defaultBank.bankNumber = 0;
        defaultBank.name = "00 Empty";
        defaultBank.directory = currentBankDir;
        for (int pIdx = 1; pIdx <= 128; ++pIdx) {
            Synth1ProgramInfo pInfo;
            pInfo.index = pIdx;
            pInfo.name = (pIdx == 1) ? "initial sound" : "Empty";
            pInfo.color = "red";
            pInfo.file = currentBankDir.getChildFile(juce::String::formatted("%03d.sy1", pIdx));
            defaultBank.programs.push_back(pInfo);
        }
        banks.push_back(defaultBank);
    }
}

void OpenSynth1AudioProcessor::selectBankAndProgram(int bankIdx, int progIdx)
{
    if (bankIdx < 0 || bankIdx >= (int)banks.size()) return;
    currentBankIndex = bankIdx;
    currentBankName = banks[bankIdx].name;

    if (progIdx < 1 || progIdx > 128) progIdx = 1;
    currentProgramIndex = progIdx;

    resetVoicesPending.store(true);

    if (progIdx - 1 < (int)banks[bankIdx].programs.size())
    {
        auto& pInfo = banks[bankIdx].programs[progIdx - 1];
        currentProgramName = pInfo.name;
        currentProgramColor = pInfo.color;
        if (pInfo.file.existsAsFile()) {
            loadSy1Preset(pInfo.file);
        }
    }
}

void OpenSynth1AudioProcessor::loadSy1Preset(const juce::File& presetFile)
{
    if (!presetFile.existsAsFile()) return;

    resetVoicesPending.store(true);

    juce::StringArray lines;
    presetFile.readLines(lines);

    if (lines.size() > 0) currentProgramName = lines[0].trim();
    if (lines.size() > 1 && lines[1].startsWith("color=")) {
        currentProgramColor = lines[1].fromFirstOccurrenceOf("color=", false, false).trim();
    }

    int sy1Ver = 112;
    if (lines.size() > 2 && lines[2].startsWith("ver=")) {
        sy1Ver = lines[2].fromFirstOccurrenceOf("ver=", false, false).getIntValue();
    }

    bool has86 = false, has88 = false;
    for (auto line : lines)
    {
        line = line.trim();
        if (line.containsChar(','))
        {
            auto idStr = line.upToFirstOccurrenceOf(",", false, false).trim();
            auto valStr = line.fromFirstOccurrenceOf(",", false, false).trim();
            int val = valStr.getIntValue();
            if (idStr == "86") has86 = true;
            if (idStr == "88") has88 = true;

            if (auto* rParam = dynamic_cast<juce::RangedAudioParameter*>(apvts.getParameter(idStr)))
            {
                if (idStr == "86" || idStr == "88") {
                    if (val >= 45056 && val < 45056 + 128) val -= 45056;
                    else if (val == 53248) val = 128;
                }
                else if (idStr == "14") {
                    // Filter mode enum: 0=LP12, 1=LP24, 2=HP12, 3=BP12, 4=LPDL
                    val = juce::jlimit(0, 4, val);
                }
                else if (idStr == "41" || idStr == "46") {
                    // sy1 format is 1-based (1..7: osc2, osc1,2, filter, amp, p/w, FM, pan)
                    val = juce::jlimit(0, 6, val - 1);
                }
                else if (idStr == "31") {
                    // sy1 format is 1-based: 1=Up/Down (0), 2=Up (1), 3=Down (2), 4=Random (3)
                    val = juce::jlimit(0, 3, val - 1);
                }
                else if (idStr == "64") {
                    // Chorus type in sy1: 1 -> x1 (0), 2 -> x2 (1), 4 -> x4 (2)
                    if (val <= 1) val = 0;
                    else if (val == 2) val = 1;
                    else val = 2;
                }
                else if (idStr == "35") {
                    // Delay time index 0..19
                    val = juce::jlimit(0, 19, val);
                }

                float normalizedValue = rParam->getNormalisableRange().convertTo0to1(static_cast<float>(val));
                rParam->setValueNotifyingHost(normalizedValue);
            }
        }
    }
    if (!has86) {
        if (auto* p = apvts.getParameter("86")) p->setValueNotifyingHost(p->convertTo0to1(1.0f));
    }
    if (!has88) {
        if (auto* p = apvts.getParameter("88")) p->setValueNotifyingHost(p->convertTo0to1(2.0f));
    }

    resetVoicesPending.store(true);
    presetLoadedFlag.store(true);
}

void OpenSynth1AudioProcessor::resetToInitPreset()
{
    resetVoicesPending.store(true);

    for (auto* param : apvts.processor.getParameters())
    {
        if (auto* rParam = dynamic_cast<juce::RangedAudioParameter*>(param))
        {
            rParam->setValueNotifyingHost(rParam->getDefaultValue());
        }
    }

    currentProgramName = "initial sound";
    currentProgramColor = "red";
    resetVoicesPending.store(true);
    presetLoadedFlag.store(true);
}

bool OpenSynth1AudioProcessor::saveSy1Preset(const juce::File& presetFile, const juce::String& name, const juce::String& color)
{
    juce::String content;
    content << name << "\n";
    content << "color=" << color << "\n";
    content << "ver=113\n";

    // All parameters 0..98
    const std::vector<juce::String> paramIds = {
        "0", "45", "76", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "10", "11", "12", "13", "71", "72", "91", "95", "96", "97",
        "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24",
        "25", "26", "27", "28", "29", "30",
        "59", "31", "32", "33", "34",
        "65", "82", "35", "83", "36", "98", "37",
        "66", "64", "52", "53", "54", "55", "56",
        "60", "61", "62", "63", "90",
        "77", "78", "79", "80", "81",
        "38", "94", "73", "93", "75", "84", "85", "92", "39", "74",
        "40", "86", "50", "87", "88", "51", "89",
        "57", "41", "42", "43", "44", "67", "68",
        "58", "46", "47", "48", "49", "69", "70",
        "107"
    };

    for (auto& id : paramIds)
    {
        if (auto* p = apvts.getParameter(id)) {
            int intVal = (int)p->getValue() * 127;
            if (auto* rp = dynamic_cast<juce::RangedAudioParameter*>(p)) {
                intVal = (int)std::round(rp->getNormalisableRange().convertFrom0to1(rp->getValue()));
            }
            if (id == "41" || id == "46" || id == "31") {
                intVal = intVal + 1; // 1-based for sy1 format
            }
            else if (id == "64") {
                if (intVal == 0) intVal = 1;
                else if (intVal == 1) intVal = 2;
                else intVal = 4;
            }
            else if (id == "86" || id == "88") {
                if (intVal >= 0 && intVal <= 127) intVal += 45056;
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

void OpenSynth1AudioProcessor::setMasterTuneCents(float cents)
{
    masterTuneCents.store(cents);
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto* v = dynamic_cast<Synth1Voice*>(synth.getVoice(i))) {
            v->setMasterTuneCents(cents);
        }
    }
}

// -------------------------------------------------------------
// MIDI CC Map & MIDI Learn
// -------------------------------------------------------------
void OpenSynth1AudioProcessor::initDefaultMidiCcMap()
{
    midiCcMap.fill(-1);

    // Standard Nord Lead 2 / Synth1 CC Defaults
    midiCcMap[1]  = 86;  // Mod Wheel -> Src 1
    midiCcMap[5]  = 39;  // Portamento Time
    midiCcMap[7]  = 107; // Master Volume
    midiCcMap[10] = 90;  // Pan
    midiCcMap[14] = 0;   // Osc1 Shape
    midiCcMap[15] = 1;   // Osc2 Shape
    midiCcMap[16] = 5;   // Osc Mix
    midiCcMap[17] = 2;   // Osc2 Pitch
    midiCcMap[18] = 3;   // Osc2 Fine Tune
    midiCcMap[19] = 6;   // Osc2 Sync
    midiCcMap[20] = 7;   // Osc2 Ring Mod
    midiCcMap[21] = 8;   // Osc Pulse Width
    midiCcMap[70] = 45;  // FM
    midiCcMap[71] = 20;  // Filter Resonance
    midiCcMap[72] = 18;  // Filter Release
    midiCcMap[73] = 15;  // Filter Attack
    midiCcMap[74] = 19;  // Filter Frequency
    midiCcMap[75] = 16;  // Filter Decay
    midiCcMap[76] = 22;  // Filter Saturation
    midiCcMap[77] = 21;  // Filter Amount
    midiCcMap[78] = 23;  // Filter Track
    midiCcMap[79] = 17;  // Filter Sustain
    midiCcMap[80] = 25;  // Amp Attack
    midiCcMap[81] = 26;  // Amp Decay
    midiCcMap[82] = 27;  // Amp Sustain
    midiCcMap[83] = 28;  // Amp Release
    midiCcMap[84] = 29;  // Amp Gain
    midiCcMap[85] = 35;  // Delay Time
    midiCcMap[86] = 36;  // Delay Feedback
    midiCcMap[87] = 37;  // Delay Dry/Wet
    midiCcMap[88] = 52;  // Chorus Time
    midiCcMap[89] = 53;  // Chorus Depth
    midiCcMap[90] = 54;  // Chorus Rate
    midiCcMap[91] = 56;  // Chorus Level
    midiCcMap[92] = 43;  // LFO1 Speed
    midiCcMap[93] = 44;  // LFO1 Depth
    midiCcMap[94] = 48;  // LFO2 Speed
    midiCcMap[95] = 49;  // LFO2 Depth
}

void OpenSynth1AudioProcessor::setMidiCcMapping(int ccNumber, int paramId)
{
    if (ccNumber >= 0 && ccNumber < 128) {
        midiCcMap[ccNumber] = paramId;
    }
}

int OpenSynth1AudioProcessor::getMidiCcForParam(int paramId) const
{
    for (int i = 0; i < 128; ++i) {
        if (midiCcMap[i] == paramId) return i;
    }
    return -1;
}

int OpenSynth1AudioProcessor::getParamForMidiCc(int ccNumber) const
{
    if (ccNumber >= 0 && ccNumber < 128) return midiCcMap[ccNumber];
    return -1;
}

void OpenSynth1AudioProcessor::startMidiLearn(int targetParamId)
{
    midiLearnTargetParam.store(targetParamId);
    midiLearnActive.store(true);
    lastMidiLearnMessage = "MIDI Learn Active: Move a controller knob...";
}

void OpenSynth1AudioProcessor::stopMidiLearn()
{
    midiLearnActive.store(false);
    midiLearnTargetParam.store(-1);
}

int OpenSynth1AudioProcessor::getActiveVoiceCount() const
{
    uint32_t mask = activeVoicesMask.load();
    int count = 0;
    while (mask) {
        count += (mask & 1);
        mask >>= 1;
    }
    return count;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new OpenSynth1AudioProcessor(); }
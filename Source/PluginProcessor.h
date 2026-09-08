#pragma once

#include <JuceHeader.h>
#include <array>
#include <vector>
#include "OpenSynthFilter.h"

// --- CLASSI BASE PER IL MOTORE AUDIO E MIDI ---
class Synth1Sound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

class Synth1Voice : public juce::SynthesiserVoice
{
public:
    Synth1Voice(juce::AudioProcessorValueTreeState& apvts, int voiceIdx, std::atomic<uint32_t>& voiceMask, std::atomic<float>& hostBpmRef);

    bool canPlaySound (juce::SynthesiserSound* sound) override;

    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void setMasterTuneCents(float cents) { masterTuneCents = cents; }
    void setGlobalLfoState(float p1, float p2, float sh1, float sh2)
    {
        globalLfo1Phase = p1;
        globalLfo2Phase = p2;
        globalLfo1Sh = sh1;
        globalLfo2Sh = sh2;
    }
    void resetVoiceState();
    void updateLegatoNote(int newMidiNoteNumber, float newVelocity);
    void retriggerMonoNote(int newMidiNoteNumber, float newVelocity);

private:
    float getOsc1Sample(float phase, float dt, int shape, float pw);
    float getOsc2Sample(float phase, float dt, int shape, float pw);
    float getSubOscSample(float phase, int shape);
    float getLfoSample(float phase, int shape, float& shVal);
    void updateEnvelopes();

    int voiceIndex = 0;
    std::atomic<uint32_t>& activeVoicesMask;
    std::atomic<float>& hostBpm;

    float osc1PhaseA = 0.0f;
    float osc1PhaseB = 0.0f;
    float osc2Phase = 0.0f;
    float subPhase = 0.0f;
    float lfo1Phase = 0.0f;
    float lfo2Phase = 0.0f;
    float lfo1ShVal = 0.0f;
    float lfo2ShVal = 0.0f;
    float globalLfo1Phase = 0.0f;
    float globalLfo2Phase = 0.0f;
    float globalLfo1Sh = 0.0f;
    float globalLfo2Sh = 0.0f;
    float modEnvPhase = 0.0f;

    // Unison sub-phases (up to 8 voices)
    float uOsc1Phase[32] = {0.0f};
    float uOsc2Phase[32] = {0.0f};

    // Authentic Synth1 8-multiple detune sub-phases for Osc1 and Sub-osc
    float osc1DetunePhases[8] = {0.0f};
    float subDetunePhases[8] = {0.0f};

    float targetNoteFreq = 440.0f;
    float currentGlideFreq = 440.0f;
    int currentMidiNote = 60;
    float currentNoteVelocity = 0.8f;
    float level = 0.0f;
    float pitchWheelBend = 0.0f;
    float masterTuneCents = 0.0f;

    // OpenSynth1 Filter Engine
    OpenSynthFilter filter;

    juce::ADSR ampAdsr;
    juce::ADSR filterAdsr;
    juce::Random random;

    // Mod env state
    float modEnvVal = 0.0f;
    bool modEnvActive = false;

    // APVTS parameter references
    std::atomic<float>* osc1Shape = nullptr;
    std::atomic<float>* osc1FM = nullptr;
    std::atomic<float>* osc1Detune = nullptr;
    std::atomic<float>* osc2Shape = nullptr;
    std::atomic<float>* osc2Pitch = nullptr;
    std::atomic<float>* osc2PitchOn = nullptr;
    std::atomic<float>* osc2Fine = nullptr;
    std::atomic<float>* osc2FineOn = nullptr;
    std::atomic<float>* osc2KbdTrack = nullptr;
    std::atomic<float>* oscMix = nullptr;
    std::atomic<float>* osc2Sync = nullptr;
    std::atomic<float>* osc2Ring = nullptr;
    std::atomic<float>* oscPulseWidth = nullptr;
    std::atomic<float>* oscKeyShift = nullptr;
    std::atomic<float>* oscModEnvOn = nullptr;
    std::atomic<float>* oscModEnvAmt = nullptr;
    std::atomic<float>* oscModEnvAttack = nullptr;
    std::atomic<float>* oscModEnvDecay = nullptr;
    std::atomic<float>* oscModDest = nullptr;
    std::atomic<float>* masterFineTune = nullptr;
    std::atomic<float>* oscPhaseShift = nullptr;
    std::atomic<float>* oscSubGain = nullptr;
    std::atomic<float>* oscSubShape = nullptr;
    std::atomic<float>* oscSubOctave = nullptr;

    std::atomic<float>* filterType = nullptr;
    std::atomic<float>* filterAttack = nullptr;
    std::atomic<float>* filterDecay = nullptr;
    std::atomic<float>* filterSustain = nullptr;
    std::atomic<float>* filterRelease = nullptr;
    std::atomic<float>* filterCutoff = nullptr;
    std::atomic<float>* filterRes = nullptr;
    std::atomic<float>* filterAmt = nullptr;
    std::atomic<float>* filterTrack = nullptr;
    std::atomic<float>* filterSat = nullptr;
    std::atomic<float>* filterVel = nullptr;

    std::atomic<float>* ampAttack = nullptr;
    std::atomic<float>* ampDecay = nullptr;
    std::atomic<float>* ampSustain = nullptr;
    std::atomic<float>* ampRelease = nullptr;
    std::atomic<float>* ampGain = nullptr;
    std::atomic<float>* ampVel = nullptr;

    std::atomic<float>* lfo1On = nullptr;
    std::atomic<float>* lfo1Dest = nullptr;
    std::atomic<float>* lfo1Type = nullptr;
    std::atomic<float>* lfo1Speed = nullptr;
    std::atomic<float>* lfo1Depth = nullptr;
    std::atomic<float>* lfo1Tempo = nullptr;
    std::atomic<float>* lfo1Key = nullptr;

    std::atomic<float>* lfo2On = nullptr;
    std::atomic<float>* lfo2Dest = nullptr;
    std::atomic<float>* lfo2Type = nullptr;
    std::atomic<float>* lfo2Speed = nullptr;
    std::atomic<float>* lfo2Depth = nullptr;
    std::atomic<float>* lfo2Tempo = nullptr;
    std::atomic<float>* lfo2Key = nullptr;

    std::atomic<float>* playMode = nullptr;
    std::atomic<float>* polyphonyParam = nullptr;
    std::atomic<float>* portamentoTime = nullptr;
    std::atomic<float>* portamentoAuto = nullptr;
    std::atomic<float>* pbRange = nullptr;

    std::atomic<float>* unisonOn = nullptr;
    std::atomic<float>* unisonVoices = nullptr;
    std::atomic<float>* unisonDetune = nullptr;
    std::atomic<float>* unisonSpread = nullptr;
    std::atomic<float>* unisonPitch = nullptr;
    std::atomic<float>* unisonPhaseShift = nullptr;

    std::atomic<float>* midiCtrlSrc1 = nullptr;
    std::atomic<float>* midiCtrlSens1 = nullptr;
    std::atomic<float>* midiCtrlAssign1 = nullptr;
    std::atomic<float>* midiCtrlSrc2 = nullptr;
    std::atomic<float>* midiCtrlSens2 = nullptr;
    std::atomic<float>* midiCtrlAssign2 = nullptr;
    float midiCtrl1Value = 0.0f;
    float midiCtrl2Value = 0.0f;
};

// --- STRUTTURA PROGRAMMA / PRESET ---
struct Synth1ProgramInfo
{
    int index = 1; // 1..128
    juce::String name = "Empty";
    juce::String color = "red";
    juce::File file;
};

struct Synth1BankInfo
{
    int bankNumber = 0;
    juce::String name = "Empty";
    juce::File directory;
    std::vector<Synth1ProgramInfo> programs;
};

class OpenSynth1AudioProcessor  : public juce::AudioProcessor
{
public:
    OpenSynth1AudioProcessor();
    ~OpenSynth1AudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Preset & Bank Management
    void scanBanks();
    void setBankDirectory(const juce::File& dir);
    juce::File getBankDirectory() const { return currentBankDir; }
    const std::vector<Synth1BankInfo>& getBanks() const { return banks; }
    void selectBankAndProgram(int bankIdx, int progIdx);
    void loadSy1Preset(const juce::File& presetFile);
    bool saveSy1Preset(const juce::File& presetFile, const juce::String& name, const juce::String& color);
    void resetToInitPreset();
    
    juce::String getCurrentProgramName() const { return currentProgramName; }
    juce::String getCurrentBankName() const { return currentBankName; }
    juce::String getCurrentColorName() const { return currentProgramColor; }
    int getCurrentBankNumber() const { return currentBankIndex; }
    int getCurrentProgramNumber() const { return currentProgramIndex; }

    // Master Tuning (-100 to +100 cents)
    void setMasterTuneCents(float cents);
    float getMasterTuneCents() const { return masterTuneCents.load(); }

    // MIDI CC Mapping & Learn Mode
    void initDefaultMidiCcMap();
    void setMidiCcMapping(int ccNumber, int paramId);
    int getMidiCcForParam(int paramId) const;
    int getParamForMidiCc(int ccNumber) const;
    void startMidiLearn(int targetParamId);
    void stopMidiLearn();
    bool isMidiLearnActive() const { return midiLearnActive.load(); }
    int getMidiLearnTargetParam() const { return midiLearnTargetParam.load(); }
    juce::String getLastMidiLearnMessage() const { return lastMidiLearnMessage; }

    // Voice Activity
    uint32_t getActiveVoicesMask() const { return activeVoicesMask.load(); }
    int getActiveVoiceCount() const;

    // Settings (Colors & UI scale)
    float getUiScale() const { return uiScale; }
    void setUiScale(float scale) { uiScale = scale; }
    juce::Colour getPanelCustomColour() const { return panelCustomColour; }
    void setPanelCustomColour(juce::Colour c) { panelCustomColour = c; }
    juce::Colour getTextCustomColour() const { return textCustomColour; }
    void setTextCustomColour(juce::Colour c) { textCustomColour = c; }
    bool isPanelCustomColourEnabled() const { return panelColorEnabled; }
    void setPanelCustomColourEnabled(bool en) { panelColorEnabled = en; }
    bool isTextCustomColourEnabled() const { return textColorEnabled; }
    void setTextCustomColourEnabled(bool en) { textColorEnabled = en; }
    bool consumePresetLoaded() { return presetLoadedFlag.exchange(false); }

    juce::AudioProcessorValueTreeState apvts;

private:
    juce::Synthesiser synth;
    std::atomic<uint32_t> activeVoicesMask { 0 };
    std::atomic<float> masterTuneCents { 0.0f };
    std::atomic<bool> resetVoicesPending { false };

    // Banks and presets
    juce::File currentBankDir;
    std::vector<Synth1BankInfo> banks;
    int currentBankIndex = 0;
    int currentProgramIndex = 1;
    juce::String currentBankName = "Bank 00";
    juce::String currentProgramName = "initial sound";
    juce::String currentProgramColor = "red";

    // MIDI CC Map: array of 128 elements storing parameter ID (or -1 if unmapped)
    std::array<int, 128> midiCcMap;
    std::atomic<bool> midiLearnActive { false };
    std::atomic<int> midiLearnTargetParam { -1 };
    juce::String lastMidiLearnMessage;

    // FX units (Master bus)
    juce::AudioBuffer<float> delayBuffer;
    int delayWritePos = 0;
    float delayFilterStateL = 0.0f;
    float delayFilterStateR = 0.0f;


    juce::AudioBuffer<float> chorusBuffer;
    int chorusWritePos = 0;
    float chorusPhase = 0.0f;

    // Global LFO states (for free-running Key Sync = Off mode)
    float processorLfo1Phase = 0.0f;
    float processorLfo2Phase = 0.0f;
    float processorLfo1Sh = 0.0f;
    float processorLfo2Sh = 0.0f;

    // Phaser state (up to 6 all-pass stages)
    std::array<float, 6> phaserAPL {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::array<float, 6> phaserAPR {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float phaserPhase = 0.0f;
    float phaserFeedbackL = 0.0f;
    float phaserFeedbackR = 0.0f;

    // EQ biquad state
    float eqX1L = 0.0f, eqX2L = 0.0f, eqY1L = 0.0f, eqY2L = 0.0f;
    float eqX1R = 0.0f, eqX2R = 0.0f, eqY1R = 0.0f, eqY2R = 0.0f;

    // Decimator / BitCrusher state
    float decimHeldL = 0.0f, decimHeldR = 0.0f;
    int decimCounter = 0;

    // RingMod FX carrier phase
    float rmPhase = 0.0f;

    // Compressor state
    float compEnvFollower = 0.0f;

    // Arpeggiator Engine State
    std::vector<int> arpHeldKeys;
    std::vector<int> arpPattern;
    int arpPatternIndex = 0;
    double arpSamplesPerStep = 11025.0;
    double arpStepCounter = 0.0;
    int arpNotePlaying = -1;
    int arpGateSamplesRemaining = 0;
    int arpDirection = 1; // for up/down
    std::atomic<float> currentHostBpm { 120.0f };
    std::atomic<bool> presetLoadedFlag { false };

    // Monophonic / Legato note stack (Play Mode 1: Mono, 2: Legato)
    struct MonoHeldNote {
        int noteNumber = 0;
        float velocity = 0.8f;
    };
    std::vector<MonoHeldNote> monoHeldKeys;
    int currentMonoSoundingNote = -1;

    // UI Customization
    float uiScale = 1.0f;
    bool panelColorEnabled = false;
    bool textColorEnabled = false;
    juce::Colour panelCustomColour { 0xff22283c };
    juce::Colour textCustomColour { 0xffc8c8c8 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenSynth1AudioProcessor)
};

// Backward compatibility alias
using Synth1CloneAudioProcessor = OpenSynth1AudioProcessor;
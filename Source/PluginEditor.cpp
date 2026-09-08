#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h>

// Helper for official Synth1 parameter names
static juce::String getSynth1OfficialParamName(int id)
{
    switch (id)
    {
        case 0: return "osc1 shape";
        case 76: return "osc1 detune";
        case 45: return "osc1 FM";
        case 95: return "osc1 sub gain";
        case 96: return "osc1 sub shape";
        case 97: return "osc1 sub octave";
        case 1: return "osc2 shape";
        case 6: return "osc2 sync";
        case 7: return "osc2 ring modulation";
        case 4: return "osc2 kbd track";
        case 2: return "osc2 pitch";
        case 3: return "osc2 fine tune";
        case 9: return "osc key shift";
        case 5: return "osc mix";
        case 8: return "osc pulse width";
        case 10: return "osc mod env on/off";
        case 12: return "osc mod env attack";
        case 13: return "osc mod env decay";
        case 11: return "osc mod env amount";
        case 71: return "osc mod env dest";
        case 91: return "osc1 phase";
        case 72: return "osc2 pitch tune";
        case 14: return "filter type";
        case 15: return "filter attack";
        case 16: return "filter decay";
        case 17: return "filter sustain";
        case 18: return "filter release";
        case 19: return "filter freq";
        case 20: return "filter resonance";
        case 21: return "filter amount";
        case 22: return "filter kbd track";
        case 23: return "filter saturation";
        case 24: return "filter velocity";
        case 25: return "amp attack";
        case 26: return "amp decay";
        case 27: return "amp sustain";
        case 28: return "amp release";
        case 29: return "amp gain";
        case 30: return "amp velocity";
        case 59: return "arpeggiator on/off";
        case 31: return "arpeggiator type";
        case 32: return "arpeggiator range";
        case 33: return "arpeggiator beat";
        case 34: return "arpeggiator gate";
        case 43: return "lfo1 speed";
        case 44: return "lfo1 amount";
        case 41: return "lfo1 dest";
        case 67: return "lfo1 tempo/sync";
        case 68: return "lfo1 key sync";
        case 48: return "lfo2 speed";
        case 49: return "lfo2 amount";
        case 46: return "lfo2 dest";
        case 69: return "lfo2 tempo/sync";
        case 70: return "lfo2 key sync";
        case 74: return "voice mode";
        case 73: return "unison mode";
        case 75: return "unison detune";
        case 92: return "unison phase";
        case 84: return "unison spread";
        case 85: return "unison pitch";
        case 50: return "effect on/off";
        case 51: return "effect type";
        case 52: return "effect ctl1";
        case 53: return "effect ctl2";
        case 54: return "effect level";
        case 80: return "delay on/off";
        case 81: return "delay type";
        case 82: return "delay time";
        case 86: return "delay spread";
        case 83: return "delay feedback";
        case 87: return "delay tone";
        case 88: return "delay dry/wet";
        case 60: return "chorus on/off";
        case 61: return "chorus type";
        case 62: return "chorus time";
        case 63: return "chorus depth";
        case 64: return "chorus rate";
        case 65: return "chorus feedback";
        case 66: return "chorus level";
        case 55: return "eq freq";
        case 56: return "eq level";
        case 57: return "eq Q";
        case 89: return "eq tone";
        case 58: return "pan (L-R)";
        case 90: return "pitch bend range";
        default: return "param " + juce::String(id);
    }
}

// ==============================================================================
// OscillatorsSection Implementation
// ==============================================================================
OscillatorsSection::OscillatorsSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Oscillators")
{
    // Row 1: Osc 1
    // 2x2 waveform grid (Sine, Saw, Tri, Pulse)
    // Parameter "0": 0=Sine, 1=Saw, 2=Pulse, 3=Triangle
    // WaveformIcon: 0=Saw, 1=Tri, 2=Square/Pulse, 3=S&H, 4=Noise, 5=Sine
    addRadioToggle(apvts, "0", 0, 1001, "", 28, 22, 14, 14); // Sine (val 0)
    addWfIcon(44, 25, 5);
    addRadioToggle(apvts, "0", 1, 1001, "", 62, 22, 14, 14); // Saw (val 1)
    addWfIcon(78, 25, 0);

    addRadioToggle(apvts, "0", 3, 1001, "", 28, 42, 14, 14); // Tri (val 3)
    addWfIcon(44, 45, 1);
    addRadioToggle(apvts, "0", 2, 1001, "", 62, 42, 14, 14); // Pulse (val 2)
    addWfIcon(78, 45, 2);

    addKnob(apvts, "76", "det", 102, 22, 28);
    addKnob(apvts, "45", "FM", 144, 22, 28);

    // Sub-osc controls
    addKnob(apvts, "95", "", 192, 33, 24); // Sub gain knob

    // Sub 2x2 waveforms: Sine, Saw, Tri, Pulse
    // Parameter "96": 0=Sine, 1=Triangle, 2=Saw, 3=Pulse
    addRadioToggle(apvts, "96", 0, 1002, "", 220, 22, 14, 14); // Sine
    addWfIcon(234, 24, 5);
    addRadioToggle(apvts, "96", 2, 1002, "", 248, 22, 14, 14); // Saw
    addWfIcon(262, 24, 0);

    addRadioToggle(apvts, "96", 1, 1002, "", 220, 39, 14, 14); // Tri
    addWfIcon(234, 41, 1);
    addRadioToggle(apvts, "96", 3, 1002, "", 248, 39, 14, 14); // Pulse
    addWfIcon(262, 41, 2);

    // Sub octave selection
    addRadioToggle(apvts, "97", 0, 1003, "0oct", 184, 58, 44, 15);
    addRadioToggle(apvts, "97", 1, 1003, "-1oct", 230, 58, 46, 15);

    // Row 2: Osc 2 (2x2 waveform grid: Tri, Pulse, Saw, Noise)
    // Parameter "1": 0=Saw, 1=Pulse, 2=Triangle, 3=Noise
    addRadioToggle(apvts, "1", 3, 1004, "", 28, 82, 14, 14); // Tri (val 3)
    addWfIcon(44, 85, 1);
    addRadioToggle(apvts, "1", 2, 1004, "", 62, 82, 14, 14); // Pulse (val 2)
    addWfIcon(78, 85, 2);

    addRadioToggle(apvts, "1", 1, 1004, "", 28, 101, 14, 14); // Saw (val 1)
    addWfIcon(44, 104, 0);
    addRadioToggle(apvts, "1", 4, 1004, "", 62, 101, 14, 14); // Noise (val 4)
    addWfIcon(78, 104, 4);

    addLedToggle(apvts, "7", "ring", 102, 82, 54, 18, true);
    addLedToggle(apvts, "6", "sync", 162, 82, 54, 18, true);

    // Row 3: Osc 2 Track, Pitch, Fine
    addLedToggle(apvts, "4", "track", 8, 122, 56, 18, true);
    addKnob(apvts, "2", "pitch", 96, 104, 26);
    addLedToggle(apvts, "108", "on", 126, 108, 34, 16, true);
    addKnob(apvts, "3", "fine", 164, 104, 26);
    addLedToggle(apvts, "109", "on", 194, 108, 34, 16, true);

    // Right of divider: Key shift, Mix, P/W, Phase, Tune
    pitchLcd = std::make_unique<CyanLcdBox>("+00", 14.0f);
    pitchLcd->setBounds(308, 26, 80, 26);
    addAndMakeVisible(*pitchLcd);

    pitchLcd->onDelta = [this, &apvts](int delta) {
        if (auto* p = apvts.getParameter("9")) {
            float cur = p->convertFrom0to1(p->getValue());
            float next = juce::jlimit(-24.0f, 24.0f, cur + delta);
            p->setValueNotifyingHost(p->convertTo0to1(next));
            pitchLcd->setText(juce::String::formatted("%+03d", (int)next));
        }
    };
    pitchLcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        m.addItem(1, "Reset (0)");
        m.addSeparator();
        m.addItem(2, "+24 (+2 Octaves)");
        m.addItem(3, "+12 (+1 Octave)");
        m.addItem(4, "+7 (5th)");
        m.addItem(5, "+5 (4th)");
        m.addItem(6, "-5 (-4th)");
        m.addItem(7, "-7 (-5th)");
        m.addItem(8, "-12 (-1 Octave)");
        m.addItem(9, "-24 (-2 Octaves)");

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(pitchLcd.get()),
            [this, &apvts](int result) {
                float val = 0.0f;
                if (result == 1) val = 0.0f;
                else if (result == 2) val = 24.0f;
                else if (result == 3) val = 12.0f;
                else if (result == 4) val = 7.0f;
                else if (result == 5) val = 5.0f;
                else if (result == 6) val = -5.0f;
                else if (result == 7) val = -7.0f;
                else if (result == 8) val = -12.0f;
                else if (result == 9) val = -24.0f;
                else return;

                if (auto* p = apvts.getParameter("9")) {
                    p->setValueNotifyingHost(p->convertTo0to1(val));
                    pitchLcd->setText(juce::String::formatted("%+03d", (int)val));
                }
            });
    };

    addKnob(apvts, "5", "mix", 294, 82, 36);
    addKnob(apvts, "8", "p/w", 360, 82, 36);
    addKnob(apvts, "91", "phase", 294, 150, 36);
    addKnob(apvts, "72", "tune", 360, 150, 36);

    // Row 4: Mod Envelope (generously separated from Mod Dest to eliminate overlapping text!)
    addLedToggle(apvts, "10", "m.env", 8, 156, 58, 18, true);
    addKnob(apvts, "12", "A", 84, 153, 26);
    addKnob(apvts, "13", "D", 136, 153, 26);
    addKnob(apvts, "11", "amt", 188, 153, 26);

    // Mod Dest radio buttons (placed on dedicated bottom row y = 196)
    addRadioToggle(apvts, "71", 0, 1005, "osc2", 58, 196, 54, 16);
    addRadioToggle(apvts, "71", 1, 1005, "FM", 120, 196, 48, 16);
    addRadioToggle(apvts, "71", 2, 1005, "p/w", 176, 196, 48, 16);

    updateFromParameters(apvts);
}

void OscillatorsSection::updateFromParameters(juce::AudioProcessorValueTreeState& apvts)
{
    if (pitchLcd && apvts.getRawParameterValue("9")) {
        int val = (int)apvts.getRawParameterValue("9")->load();
        pitchLcd->setText(juce::String::formatted("%+03d", val));
    }
}

void OscillatorsSection::paint(juce::Graphics& g)
{
    Synth1Section::paint(g);

    // Divider lines
    g.setColour(juce::Colour(0xff363a48));
    g.drawLine(8, 77, 268, 77, 1.0f);
    g.drawLine(8, 148, 268, 148, 1.0f);
    g.drawLine(182, 20, 182, 74, 1.0f); // FM / Sub divider
    g.drawLine(274, 20, 274, 216, 1.0f); // Left / Right divider

    // Badges 1 and 2
    g.setColour(juce::Colour(0xff14151a));
    g.fillRect(8, 24, 18, 18);
    g.fillRect(8, 83, 18, 18);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
    g.drawText("1", 8, 24, 18, 18, juce::Justification::centred);
    g.drawText("2", 8, 83, 18, 18, juce::Justification::centred);

    // Sub label clearly visible below silver header
    g.setColour(juce::Colour(0xffb0b4c4));
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    g.drawText("sub", 190, 20, 34, 12, juce::Justification::centred);

    // Key shift label below 00 LCD
    g.setFont(juce::Font(juce::FontOptions(11.5f).withStyle("Bold")));
    g.drawText("key shift", 294, 54, 108, 14, juce::Justification::centred);

    // Mod Dest label on dedicated row
    g.setColour(juce::Colour(0xff14151a));
    g.fillRect(8, 196, 44, 16);
    g.setColour(juce::Colour(0xffb0b4c4));
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    g.drawText("dest.", 8, 196, 44, 16, juce::Justification::centred);
}

// ==============================================================================
// FilterSection Implementation
// ==============================================================================
FilterSection::FilterSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Filter")
{
    // Row 1: Envelope controls (size 28)
    addKnob(apvts, "15", "A", 8, 20, 28);
    addKnob(apvts, "16", "D", 42, 20, 28);
    addKnob(apvts, "17", "S", 76, 20, 28);
    addKnob(apvts, "18", "R", 110, 20, 28);
    addKnob(apvts, "21", "amt", 144, 20, 28);

    // Type radio buttons
    addRadioToggle(apvts, "14", 0, 2001, "LP12", 196, 30, 54, 14);
    addRadioToggle(apvts, "14", 1, 2001, "LP24", 196, 44, 54, 14);
    addRadioToggle(apvts, "14", 2, 2001, "HP12", 196, 58, 54, 14);
    addRadioToggle(apvts, "14", 3, 2001, "BP12", 196, 72, 54, 14);
    addRadioToggle(apvts, "14", 4, 2001, "LPDL", 196, 86, 54, 14);

    // Row 2: Frequency, Res, Saturation, Track, Velocity
    addKnob(apvts, "19", "frq", 10, 72, 38);
    addKnob(apvts, "20", "res", 58, 72, 38);
    addKnob(apvts, "22", "sat", 108, 76, 32);
    addKnob(apvts, "23", "trk", 152, 76, 32);

    addLedToggle(apvts, "24", "vel", 196, 102, 50, 18, true);
}

void FilterSection::paint(juce::Graphics& g)
{
    Synth1Section::paint(g);

    g.setColour(juce::Colour(0xffb0b4c4));
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    g.drawText("type", 196, 18, 44, 12, juce::Justification::centredLeft);
}

// ==============================================================================
// AmpSection Implementation
// ==============================================================================
AmpSection::AmpSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Amplifier")
{
    addKnob(apvts, "25", "A", 8, 20, 32);
    addKnob(apvts, "26", "D", 46, 20, 32);
    addKnob(apvts, "27", "S", 84, 20, 32);
    addKnob(apvts, "28", "R", 122, 20, 32);
    addKnob(apvts, "29", "gain", 162, 20, 32);
    addKnob(apvts, "30", "vel", 202, 20, 32);
}

// ==============================================================================
// EffectSection Implementation
// ==============================================================================
EffectSection::EffectSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Effect")
{
    addLedToggle(apvts, "77", "ON", 8, 22, 36, 18, true);

    typeLcd = std::make_unique<CyanLcdBox>("a.d.1", 11.5f);
    typeLcd->setBounds(48, 20, 50, 22);
    addAndMakeVisible(*typeLcd);

    static const juce::StringArray fxNames = {
        "a.d.1", "a.d.2", "d.d.", "deci.", "r.m.", "comp.", "ph.1", "ph.2", "ph.3", "ph.4"
    };
    static const juce::StringArray fxDescriptions = {
        "1: a.d.1 (Auto-Pan / Ducking 1)",
        "2: a.d.2 (Auto-Pan / Ducking 2)",
        "3: d.d. (Digital Distortion)",
        "4: deci. (Decimator / Bitcrusher)",
        "5: r.m. (Ring Modulator)",
        "6: comp. (Compressor)",
        "7: ph.1 (Phaser 1)",
        "8: ph.2 (Phaser 2)",
        "9: ph.3 (Phaser 3)",
        "10: ph.4 (Phaser 4)"
    };

    typeLcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (int i = 0; i < fxDescriptions.size(); ++i)
            m.addItem(i + 1, fxDescriptions[i]);

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(typeLcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= 10) {
                    int idx = result - 1;
                    if (auto* p = apvts.getParameter("78")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)idx));
                        typeLcd->setText(fxNames[idx]);
                    }
                }
            });
    };

    typeLcd->onDelta = [this, &apvts](int delta) {
        if (auto* p = apvts.getParameter("78")) {
            int cur = (int)std::round(p->convertFrom0to1(p->getValue()));
            int nxt = juce::jlimit(0, 9, cur + delta);
            p->setValueNotifyingHost(p->convertTo0to1((float)nxt));
            typeLcd->setText(fxNames[nxt]);
        }
    };

    addKnob(apvts, "79", "ctl1", 108, 18, 28);
    addKnob(apvts, "80", "ctl2", 154, 18, 28);
    addKnob(apvts, "81", "level", 200, 18, 28);

    updateFromParameters(apvts);
}

void EffectSection::updateFromParameters(juce::AudioProcessorValueTreeState& apvts)
{
    static const juce::StringArray fxNames = {
        "a.d.1", "a.d.2", "d.d.", "deci.", "r.m.", "comp.", "ph.1", "ph.2", "ph.3", "ph.4"
    };
    if (typeLcd && apvts.getRawParameterValue("78")) {
        int idx = juce::jlimit(0, 9, (int)std::round(apvts.getRawParameterValue("78")->load()));
        typeLcd->setText(fxNames[idx]);
    }
}

// ==============================================================================
// EqSection Implementation
// ==============================================================================
EqSection::EqSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Equalizer / Pan")
{
    addKnob(apvts, "61", "freq", 8, 18, 28);
    addKnob(apvts, "62", "level", 56, 18, 28);
    addKnob(apvts, "63", "Q", 104, 18, 28);
    addKnob(apvts, "60", "tone", 152, 18, 28);
    addKnob(apvts, "90", "pan", 202, 18, 28);
}

// ==============================================================================
// DelaySection Implementation
// ==============================================================================
DelaySection::DelaySection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Tempo Delay")
{
    addLedToggle(apvts, "65", "ON", 8, 22, 36, 18, true);

    typeLcd = std::make_unique<CyanLcdBox>("ST", 12.0f);
    typeLcd->setBounds(8, 48, 36, 22);
    addAndMakeVisible(*typeLcd);

    static const juce::StringArray delayNames = { "ST", "X", "PP" };
    static const juce::StringArray delayDescs = { "1: ST (Stereo Delay)", "2: X (Cross Delay)", "3: PP (Ping-Pong Delay)" };

    typeLcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (int i = 0; i < delayDescs.size(); ++i)
            m.addItem(i + 1, delayDescs[i]);

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(typeLcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= 3) {
                    int idx = result - 1;
                    if (auto* p = apvts.getParameter("82")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)idx));
                        typeLcd->setText(delayNames[idx]);
                    }
                }
            });
    };

    typeLcd->onDelta = [this, &apvts](int delta) {
        if (auto* p = apvts.getParameter("82")) {
            int cur = (int)std::round(p->convertFrom0to1(p->getValue()));
            int nxt = juce::jlimit(0, 2, cur + delta);
            p->setValueNotifyingHost(p->convertTo0to1((float)nxt));
            typeLcd->setText(delayNames[nxt]);
        }
    };

    auto* dTimeKnob = addKnob(apvts, "35", "time", 52, 24, 28, &timeLabel);
    if (dTimeKnob) {
        dTimeKnob->onValueChange = [this, dTimeKnob]() {
            if (timeLabel) timeLabel->setText(getDelayTimeString((int)std::round(dTimeKnob->getValue())), juce::dontSendNotification);
        };
    }
    addKnob(apvts, "83", "sprd", 92, 24, 28);
    addKnob(apvts, "36", "fdbk", 132, 24, 28);
    addKnob(apvts, "98", "tone", 172, 24, 28);
    addKnob(apvts, "37", "d/w", 212, 24, 28);

    updateFromParameters(apvts);
}

void DelaySection::updateFromParameters(juce::AudioProcessorValueTreeState& apvts)
{
    static const juce::StringArray delayNames = { "ST", "X", "PP" };
    if (typeLcd && apvts.getRawParameterValue("82")) {
        int idx = juce::jlimit(0, 2, (int)std::round(apvts.getRawParameterValue("82")->load()));
        typeLcd->setText(delayNames[idx]);
    }
    if (timeLabel && apvts.getRawParameterValue("35")) {
        int val = (int)std::round(apvts.getRawParameterValue("35")->load());
        timeLabel->setText(getDelayTimeString(val), juce::dontSendNotification);
    }
}

// ==============================================================================
// ChorusSection Implementation
// ==============================================================================
ChorusSection::ChorusSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Chorus / Flanger")
{
    addLedToggle(apvts, "66", "ON", 8, 20, 36, 18, true);

    typeLcd = std::make_unique<CyanLcdBox>("x2", 12.0f);
    typeLcd->setBounds(8, 48, 36, 22);
    addAndMakeVisible(*typeLcd);

    static const juce::StringArray chorusNames = { "x1", "x2", "x4" };
    static const juce::StringArray chorusDescs = { "1: x1 (Chorus 1)", "2: x2 (Chorus 2)", "3: x4 (Chorus 4 / Flanger)" };

    typeLcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (int i = 0; i < chorusDescs.size(); ++i)
            m.addItem(i + 1, chorusDescs[i]);

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(typeLcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= 3) {
                    int idx = result - 1;
                    if (auto* p = apvts.getParameter("64")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)idx));
                        typeLcd->setText(chorusNames[idx]);
                    }
                }
            });
    };

    typeLcd->onDelta = [this, &apvts](int delta) {
        if (auto* p = apvts.getParameter("64")) {
            int cur = (int)std::round(p->convertFrom0to1(p->getValue()));
            int nxt = juce::jlimit(0, 2, cur + delta);
            p->setValueNotifyingHost(p->convertTo0to1((float)nxt));
            typeLcd->setText(chorusNames[nxt]);
        }
    };

    addKnob(apvts, "52", "time", 50, 22, 30);
    addKnob(apvts, "53", "deph", 86, 22, 30);
    addKnob(apvts, "54", "rate", 122, 22, 30);
    addKnob(apvts, "55", "fdbk", 158, 22, 30);
    addKnob(apvts, "56", "levl", 194, 22, 30);

    updateFromParameters(apvts);
}

void ChorusSection::updateFromParameters(juce::AudioProcessorValueTreeState& apvts)
{
    static const juce::StringArray chorusNames = { "x1", "x2", "x4" };
    if (typeLcd && apvts.getRawParameterValue("64")) {
        int idx = juce::jlimit(0, 2, (int)std::round(apvts.getRawParameterValue("64")->load()));
        typeLcd->setText(chorusNames[idx]);
    }
}

// ==============================================================================
// VoiceSection Implementation
// ==============================================================================
VoiceSection::VoiceSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Voice")
{
    addRadioToggle(apvts, "38", 0, 3001, "poly", 8, 22, 52, 15);
    addRadioToggle(apvts, "38", 1, 3001, "mono", 8, 38, 52, 15);
    addRadioToggle(apvts, "38", 2, 3001, "legato", 8, 54, 54, 15);

    polyLcd = std::make_unique<CyanLcdBox>("16", 13.0f);
    polyLcd->setBounds(68, 24, 34, 24);
    addAndMakeVisible(*polyLcd);

    static const std::vector<int> polyValues = { 1, 2, 4, 8, 12, 16, 24, 32 };
    polyLcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (size_t i = 0; i < polyValues.size(); ++i)
            m.addItem((int)i + 1, juce::String(polyValues[i]) + " voices");

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(polyLcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= (int)polyValues.size()) {
                    int val = polyValues[(size_t)result - 1];
                    if (auto* p = apvts.getParameter("94")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)val));
                        polyLcd->setText(juce::String(val));
                    }
                }
            });
    };

    polyLcd->onDelta = [this, &apvts](int delta) {
        if (auto* p = apvts.getParameter("94")) {
            int cur = (int)std::round(p->convertFrom0to1(p->getValue()));
            int curIdx = 5; // default 16
            for (size_t i = 0; i < polyValues.size(); ++i) {
                if (polyValues[i] == cur) { curIdx = (int)i; break; }
            }
            int nextIdx = juce::jlimit(0, (int)polyValues.size() - 1, curIdx + delta);
            int nextVal = polyValues[(size_t)nextIdx];
            p->setValueNotifyingHost(p->convertTo0to1((float)nextVal));
            polyLcd->setText(juce::String(nextVal));
        }
    };

    addKnob(apvts, "39", "", 132, 22, 30);
    addLedToggle(apvts, "74", "auto", 182, 28, 56, 16, true);

    addLedToggle(apvts, "73", "unison", 8, 73, 62, 18, true);

    unisonNumLcd = std::make_unique<CyanLcdBox>("2", 13.0f);
    unisonNumLcd->setBounds(8, 95, 30, 23);
    static const std::vector<int> unisonValues = { 2, 3, 4, 5, 6, 7, 8, 12, 16, 24, 32 };
    unisonNumLcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (size_t i = 0; i < unisonValues.size(); ++i)
            m.addItem((int)i + 1, juce::String(unisonValues[i]) + " voices");

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(unisonNumLcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= (int)unisonValues.size()) {
                    int v = unisonValues[(size_t)result - 1];
                    if (auto* p = apvts.getParameter("93")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)v));
                        unisonNumLcd->setText(juce::String(v));
                    }
                }
            });
    };
    unisonNumLcd->onDelta = [this, &apvts](int delta) {
        if (auto* p = apvts.getParameter("93")) {
            int cur = (int)std::round(p->convertFrom0to1(p->getValue()));
            int curIdx = 0;
            for (size_t i = 0; i < unisonValues.size(); ++i) {
                if (unisonValues[i] == cur) { curIdx = (int)i; break; }
            }
            int nxtIdx = juce::jlimit(0, (int)unisonValues.size() - 1, curIdx + delta);
            int nxt = unisonValues[(size_t)nxtIdx];
            p->setValueNotifyingHost(p->convertTo0to1((float)nxt));
            unisonNumLcd->setText(juce::String(nxt));
        }
    };
    addAndMakeVisible(*unisonNumLcd);

    addKnob(apvts, "75", "det", 50, 86, 32);
    addKnob(apvts, "92", "phase", 106, 86, 32);
    addKnob(apvts, "84", "sprd", 162, 86, 32);
    addKnob(apvts, "85", "pitch", 218, 86, 32);

    updateFromParameters(apvts);
}

void VoiceSection::updateFromParameters(juce::AudioProcessorValueTreeState& apvts)
{
    if (polyLcd && apvts.getRawParameterValue("94")) {
        int val = (int)std::round(apvts.getRawParameterValue("94")->load());
        polyLcd->setText(juce::String(val));
    }
    if (unisonNumLcd && apvts.getRawParameterValue("93")) {
        int val = (int)std::round(apvts.getRawParameterValue("93")->load());
        unisonNumLcd->setText(juce::String(val));
    }
}

void VoiceSection::paint(juce::Graphics& g)
{
    Synth1Section::paint(g);

    g.setColour(juce::Colour(0xffdcdfe8));
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    g.drawText("poly", 66, 49, 38, 14, juce::Justification::centred);

    // Portamento box border & label (width 90 so "portamento" has plenty of breathing room!)
    g.setColour(juce::Colour(0xff363a48));
    g.drawRect(120, 19, 154, 49, 1);
    g.setColour(juce::Colour(0xffdcdfe8));
    g.setFont(juce::Font(juce::FontOptions(10.5f).withStyle("Bold")));
    g.drawText("portamento", 124, 52, 90, 14, juce::Justification::centred);

    g.drawText("num", 6, 118, 34, 13, juce::Justification::centred);
}

// ==============================================================================
// ArpSection Implementation
// ==============================================================================
ArpSection::ArpSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Arpeggiator")
{
    addLedToggle(apvts, "59", "ON", 8, 24, 38, 18, true);

    // Types
    addRadioToggle(apvts, "31", 0, 4001, "updown", 50, 32, 58, 14);
    addRadioToggle(apvts, "31", 1, 4001, "up", 50, 47, 44, 14);
    addRadioToggle(apvts, "31", 2, 4001, "down", 50, 62, 48, 14);
    addRadioToggle(apvts, "31", 3, 4001, "random", 50, 77, 58, 14);

    // Oct ranges
    addRadioToggle(apvts, "32", 0, 4002, "1oct", 112, 32, 44, 14);
    addRadioToggle(apvts, "32", 1, 4002, "2oct", 112, 47, 44, 14);
    addRadioToggle(apvts, "32", 2, 4002, "3oct", 112, 62, 44, 14);
    addRadioToggle(apvts, "32", 3, 4002, "4oct", 112, 77, 44, 14);

    // Beat & Gate knobs (34px)
    auto* bKnob = addKnob(apvts, "33", "beat", 160, 24, 34, &beatLabel);
    if (bKnob) {
        bKnob->onValueChange = [this, bKnob]() {
            if (beatLabel) beatLabel->setText(getArpBeatString((int)std::round(bKnob->getValue())), juce::dontSendNotification);
        };
    }
    addKnob(apvts, "34", "gate", 198, 24, 34);

    updateFromParameters(apvts);
}

void ArpSection::updateFromParameters(juce::AudioProcessorValueTreeState& apvts)
{
    if (beatLabel && apvts.getRawParameterValue("33")) {
        int val = (int)std::round(apvts.getRawParameterValue("33")->load());
        beatLabel->setText(getArpBeatString(val), juce::dontSendNotification);
    }
}

void ArpSection::paint(juce::Graphics& g)
{
    Synth1Section::paint(g);

    g.setColour(juce::Colour(0xffdcdfe8));
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    g.drawText("type", 52, 19, 45, 12, juce::Justification::centredLeft);
    g.drawText("range", 114, 19, 45, 12, juce::Justification::centredLeft);
}

// ==============================================================================
// LfoSection Implementation
// ==============================================================================
LfoSection::LfoSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("LFO")
{
    // LFO 1 On/Off badge button
    lfo1BadgeBtn = std::make_unique<LfoBadgeButton>("1");
    lfo1BadgeBtn->setBounds(10, 20, 22, 20);
    lfo1BadgeBtn->onClick = [this, &apvts]() {
        if (auto* p = apvts.getParameter("57")) {
            float cur = p->getValue();
            float nxt = (cur > 0.5f) ? 0.0f : 1.0f;
            p->setValueNotifyingHost(nxt);
            lfo1BadgeBtn->setOn(nxt > 0.5f);
        }
    };
    addAndMakeVisible(*lfo1BadgeBtn);

    // LFO 1 Waveform dropdown icon
    lfo1Icon = std::make_unique<WaveformIcon>(1);
    lfo1Icon->setBounds(36, 20, 32, 20);
    lfo1Icon->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        m.addItem(1, "Saw (ramp down)");
        m.addItem(2, "Triangle");
        m.addItem(3, "Square / Pulse");
        m.addItem(4, "Random (S&H)");
        m.addItem(5, "Random (Smoothed)");
        m.addItem(6, "Sine");

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(lfo1Icon.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= 6) {
                    int wf = result - 1;
                    lfo1Icon->type = wf;
                    lfo1Icon->repaint();
                    if (auto* p = apvts.getParameter("42"))
                        p->setValueNotifyingHost(p->convertTo0to1((float)wf));
                }
            });
    };
    addAndMakeVisible(*lfo1Icon);

    // LFO 1 Knobs (34px)
    auto* s1 = addKnob(apvts, "43", "spd", 16, 46, 34, &lfo1SpdLabel);
    if (s1) {
        s1->onValueChange = [this, s1, &apvts]() {
            if (lfo1SpdLabel) {
                int val = (int)std::round(s1->getValue());
                bool synced = (apvts.getRawParameterValue("67") && apvts.getRawParameterValue("67")->load() > 0.5f);
                lfo1SpdLabel->setText(getLfoSpeedString(val, synced), juce::dontSendNotification);
            }
        };
    }
    addKnob(apvts, "44", "amt", 70, 46, 34);

    // LFO 1 Sync toggles
    auto* t1 = addLedToggle(apvts, "67", "tempo", 10, 106, 52, 16, true);
    if (t1) {
        t1->onClick = [this, &apvts]() {
            if (lfo1SpdLabel && apvts.getRawParameterValue("43") && apvts.getRawParameterValue("67")) {
                int val = (int)std::round(apvts.getRawParameterValue("43")->load());
                bool synced = (apvts.getRawParameterValue("67")->load() > 0.5f);
                lfo1SpdLabel->setText(getLfoSpeedString(val, synced), juce::dontSendNotification);
            }
        };
    }
    addLedToggle(apvts, "68", "key", 64, 106, 46, 16, true);

    // Shared Destination matrix in center (x = 140..234)
    for (int i = 0; i < 7; ++i)
    {
        addRadioToggle(apvts, "41", i, 5001, "", 140, 20 + i * 15, 14, 14);
        addRadioToggle(apvts, "46", i, 5002, "", 220, 20 + i * 15, 14, 14);
    }

    // LFO 2
    lfo2BadgeBtn = std::make_unique<LfoBadgeButton>("2");
    lfo2BadgeBtn->setBounds(310, 20, 22, 20);
    lfo2BadgeBtn->onClick = [this, &apvts]() {
        if (auto* p = apvts.getParameter("58")) {
            float cur = p->getValue();
            float nxt = (cur > 0.5f) ? 0.0f : 1.0f;
            p->setValueNotifyingHost(nxt);
            lfo2BadgeBtn->setOn(nxt > 0.5f);
        }
    };
    addAndMakeVisible(*lfo2BadgeBtn);

    lfo2Icon = std::make_unique<WaveformIcon>(1);
    lfo2Icon->setBounds(274, 20, 32, 20);
    lfo2Icon->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        m.addItem(1, "Saw (ramp down)");
        m.addItem(2, "Triangle");
        m.addItem(3, "Square / Pulse");
        m.addItem(4, "Random (S&H)");
        m.addItem(5, "Random (Smoothed)");
        m.addItem(6, "Sine");

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(lfo2Icon.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= 6) {
                    int wf = result - 1;
                    lfo2Icon->type = wf;
                    lfo2Icon->repaint();
                    if (auto* p = apvts.getParameter("47"))
                        p->setValueNotifyingHost(p->convertTo0to1((float)wf));
                }
            });
    };
    addAndMakeVisible(*lfo2Icon);

    // LFO 2 Knobs (34px)
    auto* s2 = addKnob(apvts, "48", "spd", 250, 46, 34, &lfo2SpdLabel);
    if (s2) {
        s2->onValueChange = [this, s2, &apvts]() {
            if (lfo2SpdLabel) {
                int val = (int)std::round(s2->getValue());
                bool synced = (apvts.getRawParameterValue("69") && apvts.getRawParameterValue("69")->load() > 0.5f);
                lfo2SpdLabel->setText(getLfoSpeedString(val, synced), juce::dontSendNotification);
            }
        };
    }
    addKnob(apvts, "49", "amt", 304, 46, 34);

    // LFO 2 Sync toggles
    auto* t2 = addLedToggle(apvts, "69", "tempo", 244, 106, 52, 16, true);
    if (t2) {
        t2->onClick = [this, &apvts]() {
            if (lfo2SpdLabel && apvts.getRawParameterValue("48") && apvts.getRawParameterValue("69")) {
                int val = (int)std::round(apvts.getRawParameterValue("48")->load());
                bool synced = (apvts.getRawParameterValue("69")->load() > 0.5f);
                lfo2SpdLabel->setText(getLfoSpeedString(val, synced), juce::dontSendNotification);
            }
        };
    }
    addLedToggle(apvts, "70", "key", 298, 106, 46, 16, true);

    updateFromParameters(apvts);
}

void LfoSection::updateFromParameters(juce::AudioProcessorValueTreeState& apvts)
{
    if (lfo1BadgeBtn && apvts.getRawParameterValue("57"))
        lfo1BadgeBtn->setOn(apvts.getRawParameterValue("57")->load() > 0.5f);
    if (lfo2BadgeBtn && apvts.getRawParameterValue("58"))
        lfo2BadgeBtn->setOn(apvts.getRawParameterValue("58")->load() > 0.5f);

    if (lfo1Icon && apvts.getRawParameterValue("42")) {
        int wf = juce::jlimit(0, 5, (int)std::round(apvts.getRawParameterValue("42")->load()));
        if (lfo1Icon->type != wf) {
            lfo1Icon->type = wf;
            lfo1Icon->repaint();
        }
    }
    if (lfo2Icon && apvts.getRawParameterValue("47")) {
        int wf = juce::jlimit(0, 5, (int)std::round(apvts.getRawParameterValue("47")->load()));
        if (lfo2Icon->type != wf) {
            lfo2Icon->type = wf;
            lfo2Icon->repaint();
        }
    }

    if (lfo1SpdLabel && apvts.getRawParameterValue("43")) {
        int val = (int)std::round(apvts.getRawParameterValue("43")->load());
        bool synced = (apvts.getRawParameterValue("67") && apvts.getRawParameterValue("67")->load() > 0.5f);
        lfo1SpdLabel->setText(getLfoSpeedString(val, synced), juce::dontSendNotification);
    }
    if (lfo2SpdLabel && apvts.getRawParameterValue("48")) {
        int val = (int)std::round(apvts.getRawParameterValue("48")->load());
        bool synced = (apvts.getRawParameterValue("69") && apvts.getRawParameterValue("69")->load() > 0.5f);
        lfo2SpdLabel->setText(getLfoSpeedString(val, synced), juce::dontSendNotification);
    }
}

void LfoSection::paint(juce::Graphics& g)
{
    Synth1Section::paint(g);

    // Dst badges
    g.setColour(juce::Colour(0xff14151a));
    g.fillRect(72, 22, 24, 16);
    g.fillRect(244, 22, 24, 16);
    g.setColour(juce::Colour(0xffdcdfe8));
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    g.drawText("dst", 72, 22, 24, 16, juce::Justification::centred);
    g.drawText("dst", 244, 22, 24, 16, juce::Justification::centred);

    // Sync labels
    g.setColour(juce::Colour(0xff39b54a)); // green text
    g.setFont(juce::Font(juce::FontOptions(10.5f).withStyle("Bold")));
    g.drawText("-sync-", 14, 91, 90, 13, juce::Justification::centred);
    g.drawText("-sync-", 248, 91, 90, 13, juce::Justification::centred);

    // Center destination labels
    const char* dstLabels[7] = { "osc2", "osc1,2", "filter", "amp", "p/w", "FM", "pan" };
    g.setColour(juce::Colour(0xffdcdfe8));
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    for (int i = 0; i < 7; ++i)
    {
        g.drawText(dstLabels[i], 158, 20 + i * 15, 58, 14, juce::Justification::centred);
    }
}

// ==============================================================================
// WheelMidiSection Implementation
// ==============================================================================
WheelMidiSection::WheelMidiSection(juce::AudioProcessorValueTreeState& apvts)
    : Synth1Section("Wheel / MIDI")
{
    pbLcd = std::make_unique<CyanLcdBox>("12", 15.0f);
    pbLcd->setBounds(76, 30, 38, 30);
    addAndMakeVisible(*pbLcd);

    static const std::vector<int> pbOptions = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 24 };
    pbLcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (size_t i = 0; i < pbOptions.size(); ++i)
            m.addItem((int)i + 1, juce::String(pbOptions[i]) + " semi");

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(pbLcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= (int)pbOptions.size()) {
                    int val = pbOptions[(size_t)result - 1];
                    if (auto* p = apvts.getParameter("40")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)val));
                        pbLcd->setText(juce::String(val));
                    }
                }
            });
    };
    pbLcd->onDelta = [this, &apvts](int delta) {
        if (auto* p = apvts.getParameter("40")) {
            int cur = (int)std::round(p->convertFrom0to1(p->getValue()));
            int nxt = juce::jlimit(0, 24, cur + delta);
            p->setValueNotifyingHost(p->convertTo0to1((float)nxt));
            pbLcd->setText(juce::String(nxt));
        }
    };

    static const juce::StringArray midiSrcNames = {
        "#1:mod wheel", "#2:breath", "#4:foot", "#7:volume", "#11:expression", "aftertouch"
    };
    static const std::vector<int> midiSrcCc = { 1, 2, 4, 7, 11, 128 };

    static const juce::StringArray midiDstNames = {
        "lfo1 depth", "lfo1 speed", "lfo2 depth", "lfo2 speed", "filter freq", "filter amt", "amp level", "osc2 pitch", "osc1 FM", "osc p/w", "pan"
    };

    // Src 1 row
    src1Lcd = std::make_unique<CyanLcdBox>("#1:mod wheel", 12.0f);
    src1Lcd->setBounds(164, 22, 154, 26);
    src1Lcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (int i = 0; i < midiSrcNames.size(); ++i)
            m.addItem(i + 1, midiSrcNames[i]);

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(src1Lcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= midiSrcNames.size()) {
                    int cc = midiSrcCc[(size_t)result - 1];
                    if (auto* p = apvts.getParameter("86")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)cc));
                        src1Lcd->setText(midiSrcNames[result - 1]);
                    }
                }
            });
    };
    addAndMakeVisible(*src1Lcd);

    addKnob(apvts, "50", "", 348, 20, 32);

    dst1Lcd = std::make_unique<CyanLcdBox>("lfo1 depth", 12.0f);
    dst1Lcd->setBounds(390, 22, 210, 26);
    dst1Lcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (int i = 0; i < midiDstNames.size(); ++i)
            m.addItem(i + 1, midiDstNames[i]);

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(dst1Lcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= midiDstNames.size()) {
                    int idx = result - 1;
                    int targetIds[] = { 44, 43, 49, 48, 19, 21, 29, 2, 45, 8, 90 };
                    if (auto* p = apvts.getParameter("87")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)targetIds[idx]));
                        dst1Lcd->setText(midiDstNames[idx]);
                    }
                }
            });
    };
    addAndMakeVisible(*dst1Lcd);

    // Src 2 row
    src2Lcd = std::make_unique<CyanLcdBox>("#2:breath", 12.0f);
    src2Lcd->setBounds(164, 54, 154, 26);
    src2Lcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (int i = 0; i < midiSrcNames.size(); ++i)
            m.addItem(i + 1, midiSrcNames[i]);

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(src2Lcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= midiSrcNames.size()) {
                    int cc = midiSrcCc[(size_t)result - 1];
                    if (auto* p = apvts.getParameter("88")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)cc));
                        src2Lcd->setText(midiSrcNames[result - 1]);
                    }
                }
            });
    };
    addAndMakeVisible(*src2Lcd);

    addKnob(apvts, "51", "", 348, 52, 32);

    dst2Lcd = std::make_unique<CyanLcdBox>("lfo1 speed", 12.0f);
    dst2Lcd->setBounds(390, 54, 210, 26);
    dst2Lcd->onClick = [this, &apvts]() {
        juce::PopupMenu m;
        for (int i = 0; i < midiDstNames.size(); ++i)
            m.addItem(i + 1, midiDstNames[i]);

        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(dst2Lcd.get()),
            [this, &apvts](int result) {
                if (result >= 1 && result <= midiDstNames.size()) {
                    int idx = result - 1;
                    int targetIds[] = { 44, 43, 49, 48, 19, 21, 29, 2, 45, 8, 90 };
                    if (auto* p = apvts.getParameter("89")) {
                        p->setValueNotifyingHost(p->convertTo0to1((float)targetIds[idx]));
                        dst2Lcd->setText(midiDstNames[idx]);
                    }
                }
            });
    };
    addAndMakeVisible(*dst2Lcd);

    updateFromParameters(apvts);
}

void WheelMidiSection::updateFromParameters(juce::AudioProcessorValueTreeState& apvts)
{
    static const juce::StringArray midiDstNames = {
        "lfo1 depth", "lfo1 speed", "lfo2 depth", "lfo2 speed", "filter freq", "filter amt", "amp level", "osc2 pitch", "osc1 FM", "osc p/w", "pan"
    };

    if (pbLcd && apvts.getRawParameterValue("40")) {
        int v = (int)std::round(apvts.getRawParameterValue("40")->load());
        pbLcd->setText(juce::String(v));
    }

    auto getSrcName = [](int val) -> juce::String {
        int cc = val;
        if (cc >= 45056 && cc < 45056 + 128) cc -= 45056;
        else if (cc == 53248) return "aftertouch";
        else if (cc == 57344) return "pitch bender";

        if (cc == 1) return "#1:mod wheel";
        if (cc == 2) return "#2:breath";
        if (cc == 4) return "#4:foot";
        if (cc == 7) return "#7:volume";
        if (cc == 11) return "#11:expression";
        if (cc == 128) return "aftertouch";
        return "#" + juce::String(cc);
    };

    if (src1Lcd && apvts.getRawParameterValue("86")) {
        int cc = (int)std::round(apvts.getRawParameterValue("86")->load());
        src1Lcd->setText(getSrcName(cc));
    }
    if (src2Lcd && apvts.getRawParameterValue("88")) {
        int cc = (int)std::round(apvts.getRawParameterValue("88")->load());
        src2Lcd->setText(getSrcName(cc));
    }

    auto getDstName = [](int targetId) -> juce::String {
        switch (targetId) {
            case 44: return "lfo1 depth";
            case 43: return "lfo1 speed";
            case 49: return "lfo2 depth";
            case 48: return "lfo2 speed";
            case 19: return "filter freq";
            case 21: return "filter amt";
            case 29: return "amp level";
            case 2:  return "osc2 pitch";
            case 45: return "osc1 FM";
            case 8:  return "osc p/w";
            case 90: return "pan";
            default:
                if (targetId >= 0 && targetId < 11) {
                    static const char* defs[] = {
                        "lfo1 depth", "lfo1 speed", "lfo2 depth", "lfo2 speed",
                        "filter freq", "filter amt", "amp level", "osc2 pitch",
                        "osc1 FM", "osc p/w", "pan"
                    };
                    return defs[targetId];
                }
                return "dest " + juce::String(targetId);
        }
    };

    if (dst1Lcd && apvts.getRawParameterValue("87")) {
        int val = (int)std::round(apvts.getRawParameterValue("87")->load());
        dst1Lcd->setText(getDstName(val));
    }
    if (dst2Lcd && apvts.getRawParameterValue("89")) {
        int val = (int)std::round(apvts.getRawParameterValue("89")->load());
        dst2Lcd->setText(getDstName(val));
    }
}

void WheelMidiSection::paint(juce::Graphics& g)
{
    Synth1Section::paint(g);

    g.setColour(juce::Colour(0xffdcdfe8));
    g.setFont(juce::Font(juce::FontOptions(11.5f).withStyle("Bold")));
    g.drawText("p.b", 10, 26, 62, 16, juce::Justification::centred);
    g.drawText("range", 10, 42, 62, 16, juce::Justification::centred);

    g.drawText("src1", 126, 26, 34, 18, juce::Justification::centredRight);
    g.drawText("src2", 126, 58, 34, 18, juce::Justification::centredRight);

    g.setFont(juce::Font(juce::FontOptions(14.0f).withStyle("Bold")));
    g.drawText("->", 322, 26, 22, 18, juce::Justification::centred);
    g.drawText("->", 322, 58, 22, 18, juce::Justification::centred);
}

// ==============================================================================
// PresetBrowserOverlay Implementation (PDF page 73)
// ==============================================================================
PresetBrowserOverlay::PresetBrowserOverlay(OpenSynth1AudioProcessor& proc, std::function<void()> onClose)
    : processor(proc), closeCallback(onClose), bankListModel(*this), programGrid(*this)
{
    bankListBox.setModel(&bankListModel);
    bankListBox.setRowHeight(18);
    bankListBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::white);
    bankListBox.setColour(juce::ListBox::outlineColourId, juce::Colour(0xff808080));
    addAndMakeVisible(bankListBox);

    addAndMakeVisible(programGrid);

    closeButton.setButtonText("X");
    closeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffcc2222));
    closeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    closeButton.onClick = [this]() { if (closeCallback) closeCallback(); };
    addAndMakeVisible(closeButton);

    listInfoTab.setButtonText("List/Info");
    listInfoTab.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffe0e0e0));
    listInfoTab.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    addAndMakeVisible(listInfoTab);

    refresh();
}

void PresetBrowserOverlay::refresh()
{
    bankListBox.updateContent();
    bankListBox.selectRow(processor.getCurrentBankNumber());
    programGrid.repaint();
}

void PresetBrowserOverlay::paint(juce::Graphics& g)
{
    // Semi-transparent backdrop
    g.fillAll(juce::Colours::black.withAlpha(0.65f));

    // Dialog window box
    auto bounds = getLocalBounds().reduced(15).toFloat();
    g.setColour(juce::Colour(0xffece9d8)); // Classic WinXP dialog beige
    g.fillRect(bounds);
    g.setColour(juce::Colour(0xff707070));
    g.drawRect(bounds, 1.5f);

    // Dialog title bar
    auto titleBar = bounds.removeFromTop(24);
    juce::ColourGradient grad(juce::Colour(0xff0055ea), 0, titleBar.getY(),
                              juce::Colour(0xff3d95ff), 0, titleBar.getBottom(), false);
    g.setGradientFill(grad);
    g.fillRect(titleBar);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
    g.drawText("Synth1 Preset Browser - " + processor.getCurrentBankName(), 10, (int)titleBar.getY(), 400, 24, juce::Justification::centredLeft);

    // Sub-header info
    g.setColour(juce::Colours::black);
    g.setFont(juce::Font(juce::FontOptions(11.0f)));
    g.drawText("ExtBank: " + processor.getCurrentBankName(), 250, 42, 250, 20, juce::Justification::centredLeft);
    g.drawText("128 Programs", getWidth() - 150, 42, 100, 20, juce::Justification::centredRight);
}

void PresetBrowserOverlay::resized()
{
    closeButton.setBounds(getWidth() - 36, 17, 18, 18);
    listInfoTab.setBounds(20, 42, 60, 20);

    bankListBox.setBounds(20, 66, 180, getHeight() - 90);
    programGrid.setBounds(206, 66, getWidth() - 226, getHeight() - 90);
}

int PresetBrowserOverlay::BankListModel::getNumRows()
{
    return (int)owner.processor.getBanks().size();
}

void PresetBrowserOverlay::BankListModel::paintListBoxItem(int row, juce::Graphics& g, int w, int h, bool rowIsSelected)
{
    if (row < 0 || row >= (int)owner.processor.getBanks().size()) return;

    if (rowIsSelected) {
        g.setColour(juce::Colour(0xff316ac5));
        g.fillRect(0, 0, w, h);
        g.setColour(juce::Colours::white);
    } else {
        g.setColour(juce::Colours::black);
    }

    g.setFont(juce::Font(juce::FontOptions(11.0f)));
    auto name = owner.processor.getBanks()[row].name;
    g.drawText(name, 4, 0, w - 8, h, juce::Justification::centredLeft, true);
}

void PresetBrowserOverlay::BankListModel::selectedRowsChanged(int lastRowSelected)
{
    owner.processor.selectBankAndProgram(lastRowSelected, 1);
    owner.programGrid.repaint();
}

void PresetBrowserOverlay::ProgramGridComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colour(0xff808080));
    g.drawRect(getLocalBounds(), 1);

    int curBank = owner.processor.getCurrentBankNumber();
    if (curBank < 0 || curBank >= (int)owner.processor.getBanks().size()) return;

    auto& bInfo = owner.processor.getBanks()[curBank];
    int totalProgs = (int)bInfo.programs.size();

    float colW = (float)getWidth() / 4.0f;
    float rowH = (float)getHeight() / 32.0f;

    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 32; ++row)
        {
            int pIdx = col * 32 + row + 1; // 1..128
            float x = col * colW;
            float y = row * rowH;

            // Row separator
            g.setColour(juce::Colour(0xffe8e8e8));
            g.drawHorizontalLine((int)(y + rowH - 1), x, x + colW);

            if (pIdx - 1 < totalProgs)
            {
                auto& p = bInfo.programs[pIdx - 1];

                // Check color tag
                juce::Colour tagCol = juce::Colours::black;
                if (p.color == "red") tagCol = juce::Colour(0xffd00000);
                else if (p.color == "blue") tagCol = juce::Colour(0xff0044d0);
                else if (p.color == "green") tagCol = juce::Colour(0xff008800);
                else if (p.color == "yellow") tagCol = juce::Colour(0xffbb9900);
                else if (p.color == "magenta") tagCol = juce::Colour(0xffcc00cc);
                else if (p.color == "cyan") tagCol = juce::Colour(0xff0099aa);

                bool isSelected = (pIdx == owner.processor.getCurrentProgramNumber());
                if (isSelected) {
                    g.setColour(juce::Colour(0xff316ac5));
                    g.fillRect(x + 1.0f, y + 1.0f, colW - 2.0f, rowH - 1.0f);
                    tagCol = juce::Colours::white;
                }

                g.setColour(tagCol);
                g.setFont(juce::Font(juce::FontOptions(10.0f).withStyle(isSelected ? "Bold" : "Plain")));
                juce::String text = juce::String(pIdx) + ":" + p.name;
                g.drawText(text, (int)x + 3, (int)y, (int)colW - 6, (int)rowH, juce::Justification::centredLeft, true);
            }
        }
        g.setColour(juce::Colour(0xffd0d0d0));
        g.drawVerticalLine((int)((col + 1) * colW), 0.0f, (float)getHeight());
    }
}

void PresetBrowserOverlay::ProgramGridComponent::mouseDown(const juce::MouseEvent& e)
{
    float colW = (float)getWidth() / 4.0f;
    float rowH = (float)getHeight() / 32.0f;

    int col = juce::jlimit(0, 3, (int)(e.position.x / colW));
    int row = juce::jlimit(0, 31, (int)(e.position.y / rowH));

    int pIdx = col * 32 + row + 1;
    owner.processor.selectBankAndProgram(owner.processor.getCurrentBankNumber(), pIdx);
    repaint();

    if (e.getNumberOfClicks() >= 2) {
        if (owner.closeCallback) owner.closeCallback();
    }
}

// ==============================================================================
// WriteProgramDialog Implementation (PDF page 68)
// ==============================================================================
WriteProgramDialog::WriteProgramDialog(OpenSynth1AudioProcessor& proc, std::function<void()> onClose)
    : processor(proc), closeCallback(onClose)
{
    // Populate banks
    for (size_t i = 0; i < processor.getBanks().size(); ++i) {
        bankCombo.addItem(processor.getBanks()[i].name, (int)i + 1);
    }
    bankCombo.setSelectedId(processor.getCurrentBankNumber() + 1);
    addAndMakeVisible(bankCombo);

    // Populate programs 1..128
    for (int i = 1; i <= 128; ++i) {
        programCombo.addItem(juce::String(i) + ":" + processor.getCurrentProgramName(), i);
    }
    programCombo.setSelectedId(processor.getCurrentProgramNumber());
    addAndMakeVisible(programCombo);

    nameEditor.setText(processor.getCurrentProgramName());
    addAndMakeVisible(nameEditor);

    colorCombo.addItem("red", 1);
    colorCombo.addItem("blue", 2);
    colorCombo.addItem("green", 3);
    colorCombo.addItem("yellow", 4);
    colorCombo.addItem("magenta", 5);
    colorCombo.addItem("cyan", 6);
    colorCombo.setSelectedId(1);
    addAndMakeVisible(colorCombo);

    okButton.setButtonText("OK");
    okButton.onClick = [this]() {
        int bIdx = bankCombo.getSelectedId() - 1;
        int pIdx = programCombo.getSelectedId();
        if (bIdx >= 0 && bIdx < (int)processor.getBanks().size()) {
            auto file = processor.getBanks()[bIdx].directory.getChildFile(juce::String::formatted("%03d.sy1", pIdx));
            processor.saveSy1Preset(file, nameEditor.getText(), colorCombo.getText());
        }
        if (closeCallback) closeCallback();
    };
    addAndMakeVisible(okButton);

    cancelButton.setButtonText("Cancel");
    cancelButton.onClick = [this]() { if (closeCallback) closeCallback(); };
    addAndMakeVisible(cancelButton);

    closeButton.setButtonText("X");
    closeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffcc2222));
    closeButton.onClick = [this]() { if (closeCallback) closeCallback(); };
    addAndMakeVisible(closeButton);
}

void WriteProgramDialog::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black.withAlpha(0.6f));

    auto bounds = getLocalBounds().reduced(getWidth() / 4, getHeight() / 4).toFloat();
    g.setColour(juce::Colour(0xffece9d8));
    g.fillRect(bounds);
    g.setColour(juce::Colour(0xff707070));
    g.drawRect(bounds, 1.5f);

    // Title bar
    auto titleBar = bounds.removeFromTop(24);
    juce::ColourGradient grad(juce::Colour(0xff0055ea), 0, titleBar.getY(),
                              juce::Colour(0xff3d95ff), 0, titleBar.getBottom(), false);
    g.setGradientFill(grad);
    g.fillRect(titleBar);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
    g.drawText("Over Write Program", (int)titleBar.getX() + 8, (int)titleBar.getY(), 200, 24, juce::Justification::centredLeft);

    // Labels
    g.setColour(juce::Colours::black);
    g.setFont(juce::Font(juce::FontOptions(11.0f)));
    int startY = (int)titleBar.getBottom() + 15;
    g.drawText("Bank:", (int)bounds.getX() + 15, startY, 70, 22, juce::Justification::centredLeft);
    g.drawText("Program:", (int)bounds.getX() + 15, startY + 30, 70, 22, juce::Justification::centredLeft);
    g.drawText("New Name:", (int)bounds.getX() + 15, startY + 60, 70, 22, juce::Justification::centredLeft);
    g.drawText("Color:", (int)bounds.getX() + 15, startY + 90, 70, 22, juce::Justification::centredLeft);
}

void WriteProgramDialog::resized()
{
    auto bounds = getLocalBounds().reduced(getWidth() / 4, getHeight() / 4);
    closeButton.setBounds(bounds.getRight() - 22, bounds.getY() + 3, 18, 18);

    int startY = bounds.getY() + 39;
    int editX = bounds.getX() + 90;
    int editW = bounds.getWidth() - 110;

    bankCombo.setBounds(editX, startY, editW, 22);
    programCombo.setBounds(editX, startY + 30, editW, 22);
    nameEditor.setBounds(editX, startY + 60, editW, 22);
    colorCombo.setBounds(editX, startY + 90, 80, 22);

    okButton.setBounds(bounds.getX() + bounds.getWidth() / 2 - 75, bounds.getBottom() - 35, 65, 24);
    cancelButton.setBounds(bounds.getX() + bounds.getWidth() / 2 + 10, bounds.getBottom() - 35, 65, 24);
}

// ==============================================================================
// OptionsDialog Implementation (PDF pages 69 & 72)
// ==============================================================================
OptionsDialog::OptionsDialog(OpenSynth1AudioProcessor& proc, OpenSynth1AudioProcessorEditor* ed, std::function<void()> onClose)
    : processor(proc), editor(ed), closeCallback(onClose), tabs(juce::TabbedButtonBar::TabsAtTop)
{
    tabs.addTab("General", juce::Colour(0xffece9d8), new GeneralTab(proc, ed), true);
    tabs.addTab("MIDI", juce::Colour(0xffece9d8), new MidiTab(proc), true);
    tabs.addTab("Audio", juce::Colour(0xffece9d8), new AudioTab(proc, ed), true);
    addAndMakeVisible(tabs);

    okButton.setButtonText("OK");
    okButton.onClick = [this]() { if (closeCallback) closeCallback(); };
    addAndMakeVisible(okButton);

    cancelButton.setButtonText("Cancel");
    cancelButton.onClick = [this]() { if (closeCallback) closeCallback(); };
    addAndMakeVisible(cancelButton);

    auto styleDlgBtn = [](juce::TextButton& btn) {
        btn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffdfdcd0));
        btn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffccc8b8));
        btn.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff181818));
        btn.setColour(juce::TextButton::textColourOnId, juce::Colour(0xff181818));
    };
    styleDlgBtn(okButton);
    styleDlgBtn(cancelButton);

    closeButton.setButtonText("X");
    closeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffcc2222));
    closeButton.onClick = [this]() { if (closeCallback) closeCallback(); };
    addAndMakeVisible(closeButton);
}

void OptionsDialog::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black.withAlpha(0.65f));

    auto bounds = getLocalBounds().withSizeKeepingCentre(630, 400).toFloat();
    g.setColour(juce::Colour(0xffece9d8));
    g.fillRect(bounds);
    g.setColour(juce::Colour(0xff707070));
    g.drawRect(bounds, 1.5f);

    auto titleBar = bounds.removeFromTop(26);
    juce::ColourGradient grad(juce::Colour(0xff0055ea), 0, titleBar.getY(),
                              juce::Colour(0xff3d95ff), 0, titleBar.getBottom(), false);
    g.setGradientFill(grad);
    g.fillRect(titleBar);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(13.0f).withStyle("Bold")));
    g.drawText("Options", (int)titleBar.getX() + 10, (int)titleBar.getY(), 200, 26, juce::Justification::centredLeft);
}

void OptionsDialog::resized()
{
    auto bounds = getLocalBounds().withSizeKeepingCentre(630, 400);
    closeButton.setBounds(bounds.getRight() - 24, bounds.getY() + 3, 20, 20);

    tabs.setBounds(bounds.getX() + 10, bounds.getY() + 32, bounds.getWidth() - 20, bounds.getHeight() - 76);
    okButton.setBounds(bounds.getX() + bounds.getWidth() / 2 - 76, bounds.getBottom() - 36, 70, 26);
    cancelButton.setBounds(bounds.getX() + bounds.getWidth() / 2 + 6, bounds.getBottom() - 36, 70, 26);
}

// GeneralTab
OptionsDialog::GeneralTab::GeneralTab(OpenSynth1AudioProcessor& proc, OpenSynth1AudioProcessorEditor* ed)
    : processor(proc), editor(ed)
{
    auto styleDlgBtn = [](juce::TextButton& btn) {
        btn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffdfdcd0));
        btn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffccc8b8));
        btn.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff181818));
        btn.setColour(juce::TextButton::textColourOnId, juce::Colour(0xff181818));
    };

    auto styleDlgCombo = [](juce::ComboBox& combo) {
        combo.setColour(juce::ComboBox::backgroundColourId, juce::Colours::white);
        combo.setColour(juce::ComboBox::textColourId, juce::Colours::black);
        combo.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff7f9db9));
        combo.setColour(juce::ComboBox::arrowColourId, juce::Colour(0xff333333));
    };

    auto styleDlgTextEditor = [](juce::TextEditor& ed) {
        ed.setColour(juce::TextEditor::textColourId, juce::Colours::black);
        ed.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
        ed.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff7f9db9));
        ed.setColour(juce::TextEditor::highlightColourId, juce::Colour(0xff316ac5));
        ed.setColour(juce::TextEditor::highlightedTextColourId, juce::Colours::white);
        ed.applyColourToAllText(juce::Colours::black);
    };

    bankPathEditor.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    bankPathEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    bankPathEditor.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff7f9db9));
    bankPathEditor.setText(processor.getBankDirectory().getFullPathName());
    styleDlgTextEditor(bankPathEditor);
    addAndMakeVisible(bankPathEditor);

    browseBankBtn.setButtonText("Browse...");
    styleDlgBtn(browseBankBtn);
    browseBankBtn.onClick = [this]() {
        fileChooser = std::make_unique<juce::FileChooser>("Select Synth1 Presets Folder", processor.getBankDirectory(), "");
        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories,
            [this](const juce::FileChooser& fc) {
                auto dir = fc.getResult();
                if (dir.isDirectory()) {
                    processor.setBankDirectory(dir);
                    bankPathEditor.setText(dir.getFullPathName());
                }
            });
    };
    addAndMakeVisible(browseBankBtn);

    tuneSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    tuneSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 48, 22);
    tuneSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0xffc8c8c8));
    tuneSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff316ac5));
    tuneSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff316ac5));
    tuneSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    tuneSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::white);
    tuneSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff7f9db9));
    tuneSlider.setRange(-100.0, 100.0, 1.0);
    tuneSlider.setValue(processor.getMasterTuneCents());
    tuneSlider.onValueChange = [this]() {
        processor.setMasterTuneCents((float)tuneSlider.getValue());
        float hz = 440.0f * std::pow(2.0f, (float)tuneSlider.getValue() / 1200.0f);
        tuneReadoutLabel.setText(juce::String((int)tuneSlider.getValue()) + " Cents   A4=" + juce::String(hz, 3) + "Hz", juce::dontSendNotification);
    };
    addAndMakeVisible(tuneSlider);

    minusTuneBtn.setButtonText("-");
    styleDlgBtn(minusTuneBtn);
    minusTuneBtn.onClick = [this]() { tuneSlider.setValue(tuneSlider.getValue() - 1.0); };
    addAndMakeVisible(minusTuneBtn);

    plusTuneBtn.setButtonText("+");
    styleDlgBtn(plusTuneBtn);
    plusTuneBtn.onClick = [this]() { tuneSlider.setValue(tuneSlider.getValue() + 1.0); };
    addAndMakeVisible(plusTuneBtn);

    resetTuneBtn.setButtonText("Reset");
    styleDlgBtn(resetTuneBtn);
    resetTuneBtn.onClick = [this]() { tuneSlider.setValue(0.0); };
    addAndMakeVisible(resetTuneBtn);

    tuneReadoutLabel.setText("0 Cents   A4=440.000Hz", juce::dontSendNotification);
    tuneReadoutLabel.setColour(juce::Label::textColourId, juce::Colour(0xff181818));
    tuneReadoutLabel.setFont(juce::Font(juce::FontOptions(10.5f).withStyle("Bold")));
    addAndMakeVisible(tuneReadoutLabel);

    sizeCombo.addItem("100%", 1);
    sizeCombo.addItem("125%", 2);
    sizeCombo.addItem("150%", 3);
    sizeCombo.addItem("200%", 4);
    int curSizeId = 1;
    if (editor != nullptr) {
        if (editor->getWidth() >= 2000) curSizeId = 4;
        else if (editor->getWidth() >= 1500) curSizeId = 3;
        else if (editor->getWidth() >= 1200) curSizeId = 2;
        else curSizeId = 1;
    }
    sizeCombo.setSelectedId(curSizeId, juce::dontSendNotification);
    styleDlgCombo(sizeCombo);
    sizeCombo.onChange = [this]() {
        float s = 1.0f;
        if (sizeCombo.getSelectedId() == 2) s = 1.25f;
        else if (sizeCombo.getSelectedId() == 3) s = 1.5f;
        else if (sizeCombo.getSelectedId() == 4) s = 2.0f;
        if (editor != nullptr)
            editor->setUiScale(s);
        processor.setUiScale(s);
    };
    addAndMakeVisible(sizeCombo);

    alwaysOnTopToggle.setButtonText("Always on Top");
    alwaysOnTopToggle.getProperties().set("isCheckbox", true);
    alwaysOnTopToggle.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff181818));
    if (editor != nullptr)
        alwaysOnTopToggle.setToggleState(editor->isAlwaysOnTop(), juce::dontSendNotification);
    alwaysOnTopToggle.onClick = [this]() {
        if (editor != nullptr)
            editor->setAlwaysOnTopState(alwaysOnTopToggle.getToggleState());
    };
    addAndMakeVisible(alwaysOnTopToggle);

    sliderWinToggle.setButtonText("Slider Window");
    sliderWinToggle.getProperties().set("isCheckbox", true);
    sliderWinToggle.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff181818));
    addAndMakeVisible(sliderWinToggle);

    selectProgToggle.setButtonText("select program from host.");
    selectProgToggle.getProperties().set("isCheckbox", true);
    selectProgToggle.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff181818));
    addAndMakeVisible(selectProgToggle);

    panelColToggle.setButtonText("Panel");
    panelColToggle.getProperties().set("isCheckbox", true);
    panelColToggle.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff181818));
    panelColToggle.setToggleState(processor.isPanelCustomColourEnabled(), juce::dontSendNotification);
    panelColToggle.onClick = [this]() {
        processor.setPanelCustomColourEnabled(panelColToggle.getToggleState());
    };
    addAndMakeVisible(panelColToggle);

    panelColEditor.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    panelColEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    panelColEditor.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff7f9db9));
    panelColEditor.setText("#22283c");
    styleDlgTextEditor(panelColEditor);
    panelColEditor.onReturnKey = [this]() {
        juce::Colour c = juce::Colour::fromString("ff" + panelColEditor.getText().trimCharactersAtStart("#"));
        processor.setPanelCustomColour(c);
        processor.setPanelCustomColourEnabled(true);
        panelColToggle.setToggleState(true, juce::dontSendNotification);
    };
    addAndMakeVisible(panelColEditor);

    textColToggle.setButtonText("Text");
    textColToggle.getProperties().set("isCheckbox", true);
    textColToggle.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff181818));
    textColToggle.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(textColToggle);

    textColEditor.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    textColEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    textColEditor.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff7f9db9));
    textColEditor.setText("#d3d8d2");
    styleDlgTextEditor(textColEditor);
    addAndMakeVisible(textColEditor);

    transpToggle.setButtonText("Transparency Window");
    transpToggle.getProperties().set("isCheckbox", true);
    transpToggle.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff181818));
    addAndMakeVisible(transpToggle);

    themePresetCombo.addItem("Nord Red (#981129)", 1);
    themePresetCombo.addItem("Classic Dark (#22283c)", 2);
    themePresetCombo.addItem("Bright Blue (#122460)", 3);
    themePresetCombo.addItem("Dark Green (#005820)", 4);
    styleDlgCombo(themePresetCombo);
    themePresetCombo.onChange = [this]() {
        if (themePresetCombo.getSelectedId() == 1) {
            panelColEditor.setText("#981129");
            processor.setPanelCustomColour(juce::Colour::fromString("#ff981129"));
            processor.setPanelCustomColourEnabled(true);
            panelColToggle.setToggleState(true, juce::dontSendNotification);
        } else if (themePresetCombo.getSelectedId() == 2) {
            panelColEditor.setText("#22283c");
            processor.setPanelCustomColour(juce::Colour::fromString("#ff22283c"));
            processor.setPanelCustomColourEnabled(true);
            panelColToggle.setToggleState(true, juce::dontSendNotification);
        } else if (themePresetCombo.getSelectedId() == 3) {
            panelColEditor.setText("#122460");
            processor.setPanelCustomColour(juce::Colour::fromString("#ff122460"));
            processor.setPanelCustomColourEnabled(true);
            panelColToggle.setToggleState(true, juce::dontSendNotification);
        } else if (themePresetCombo.getSelectedId() == 4) {
            panelColEditor.setText("#005820");
            processor.setPanelCustomColour(juce::Colour::fromString("#ff005820"));
            processor.setPanelCustomColourEnabled(true);
            panelColToggle.setToggleState(true, juce::dontSendNotification);
        }
    };
    addAndMakeVisible(themePresetCombo);
}

void OptionsDialog::GeneralTab::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xffece9d8));
    g.setColour(juce::Colours::black);
    g.setFont(juce::Font(juce::FontOptions(11.5f).withStyle("Bold")));

    g.drawText("Program bank folders:", 14, 8, 220, 18, juce::Justification::centredLeft);

    auto drawGroupBox = [&](int x, int y, int w, int h, const juce::String& title) {
        g.setColour(juce::Colour(0xff909090));
        g.drawRect(x, y + 7, w, h - 7, 1);
        int tw = title.length() * 7 + 16;
        g.setColour(juce::Colour(0xffece9d8));
        g.fillRect(x + 10, y, tw, 14);
        g.setColour(juce::Colour(0xff111111));
        g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
        g.drawText(title, x + 12, y, tw, 14, juce::Justification::centredLeft);
    };

    drawGroupBox(14, 58, 186, 220, "Master Tuning");
    drawGroupBox(208, 58, 176, 220, "Panel Size");
    drawGroupBox(392, 58, 204, 220, "Color Customize (#RRGGBB)");
}

void OptionsDialog::GeneralTab::resized()
{
    bankPathEditor.setBounds(14, 28, 470, 24);
    browseBankBtn.setBounds(490, 28, 106, 24);

    // Master Tuning (14, 58, 186, 220)
    tuneSlider.setBounds(24, 84, 166, 24);
    minusTuneBtn.setBounds(24, 118, 30, 22);
    plusTuneBtn.setBounds(58, 118, 30, 22);
    resetTuneBtn.setBounds(94, 118, 54, 22);
    tuneReadoutLabel.setBounds(20, 150, 174, 24);

    // Panel Size (208, 58, 176, 220)
    sizeCombo.setBounds(220, 84, 86, 24);
    alwaysOnTopToggle.setBounds(220, 114, 154, 22);
    sliderWinToggle.setBounds(220, 142, 154, 22);
    selectProgToggle.setBounds(220, 170, 154, 22);

    // Color Customize (392, 58, 204, 220)
    panelColToggle.setBounds(402, 84, 70, 22);
    panelColEditor.setBounds(476, 84, 110, 22);
    textColToggle.setBounds(402, 114, 70, 22);
    textColEditor.setBounds(476, 114, 110, 22);
    transpToggle.setBounds(402, 144, 184, 22);
    themePresetCombo.setBounds(402, 178, 184, 24);
}

// MidiTab
OptionsDialog::MidiTab::MidiTab(OpenSynth1AudioProcessor& proc)
    : processor(proc), ccModel(proc)
{
    auto styleDlgBtn = [](juce::TextButton& btn) {
        btn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffdfdcd0));
        btn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffccc8b8));
        btn.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff181818));
        btn.setColour(juce::TextButton::textColourOnId, juce::Colour(0xff181818));
    };

    auto styleDlgTextEditor = [](juce::TextEditor& ed) {
        ed.setColour(juce::TextEditor::textColourId, juce::Colours::black);
        ed.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
        ed.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff7f9db9));
        ed.setColour(juce::TextEditor::highlightColourId, juce::Colour(0xff316ac5));
        ed.setColour(juce::TextEditor::highlightedTextColourId, juce::Colours::white);
        ed.applyColourToAllText(juce::Colours::black);
    };

    ccListBox.setModel(&ccModel);
    ccListBox.setRowHeight(20);
    ccListBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::white);
    ccListBox.setColour(juce::ListBox::outlineColourId, juce::Colour(0xff7f9db9));
    addAndMakeVisible(ccListBox);

    updateCcBtn.setButtonText("Update");
    styleDlgBtn(updateCcBtn);
    updateCcBtn.onClick = [this]() {
        int selRow = ccListBox.getSelectedRow();
        int ccNum = ccInputEditor.getText().getIntValue();
        if (selRow >= 0 && ccNum >= 0 && ccNum < 128) {
            processor.setMidiCcMapping(ccNum, selRow);
            ccListBox.repaint();
        }
    };
    addAndMakeVisible(updateCcBtn);

    ccInputEditor.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    ccInputEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    ccInputEditor.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff7f9db9));
    ccInputEditor.setText("74");
    styleDlgTextEditor(ccInputEditor);
    addAndMakeVisible(ccInputEditor);

    rxBankToggle.setButtonText("Receive Bank Select");
    rxBankToggle.getProperties().set("isCheckbox", true);
    rxBankToggle.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff181818));
    rxBankToggle.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(rxBankToggle);

    rxProgToggle.setButtonText("Receive Program Change");
    rxProgToggle.getProperties().set("isCheckbox", true);
    rxProgToggle.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff181818));
    rxProgToggle.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(rxProgToggle);

    loadCcBtn.setButtonText("Load");
    styleDlgBtn(loadCcBtn);
    loadCcBtn.onClick = [this]() {
        processor.initDefaultMidiCcMap();
        ccListBox.repaint();
    };
    addAndMakeVisible(loadCcBtn);

    saveCcBtn.setButtonText("Save");
    styleDlgBtn(saveCcBtn);
    addAndMakeVisible(saveCcBtn);
}

int OptionsDialog::MidiTab::CcListModel::getNumRows() { return 99; }

void OptionsDialog::MidiTab::CcListModel::paintListBoxItem(int row, juce::Graphics& g, int w, int h, bool rowIsSelected)
{
    if (rowIsSelected) {
        g.setColour(juce::Colour(0xff316ac5));
        g.fillRect(0, 0, w, h);
        g.setColour(juce::Colours::white);
    } else {
        g.setColour(row % 2 == 0 ? juce::Colours::white : juce::Colour(0xfff7f7f7));
        g.fillRect(0, 0, w, h);
        g.setColour(juce::Colour(0xff181818));
    }

    g.setFont(juce::Font(juce::FontOptions(11.0f)));
    int mappedCc = proc.getMidiCcForParam(row);
    juce::String ccStr = (mappedCc >= 0) ? juce::String(mappedCc) : "-";

    g.drawText(getSynth1OfficialParamName(row), 6, 0, w - 50, h, juce::Justification::centredLeft);
    g.drawText(ccStr, w - 44, 0, 36, h, juce::Justification::centred);
}

void OptionsDialog::MidiTab::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xffece9d8));

    // Headers
    g.setColour(juce::Colours::black);
    g.setFont(juce::Font(juce::FontOptions(11.5f).withStyle("Bold")));
    g.drawText("Parameter", 14, 6, 160, 18, juce::Justification::centredLeft);
    g.drawText("CC#", 204, 6, 40, 18, juce::Justification::centred);

    auto drawGroupBox = [&](int x, int y, int w, int h, const juce::String& title) {
        g.setColour(juce::Colour(0xff909090));
        g.drawRect(x, y + 7, w, h - 7, 1);
        int tw = title.length() * 7 + 16;
        g.setColour(juce::Colour(0xffece9d8));
        g.fillRect(x + 10, y, tw, 14);
        g.setColour(juce::Colour(0xff111111));
        g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
        g.drawText(title, x + 12, y, tw, 14, juce::Justification::centredLeft);
    };

    drawGroupBox(270, 14, 326, 76, "Receive Message");
    drawGroupBox(270, 98, 326, 68, "Control Change Map");
    drawGroupBox(270, 174, 326, 92, "MIDI CC# Learn Mode");

    g.setColour(juce::Colour(0xff222222));
    g.setFont(juce::Font(juce::FontOptions(10.5f)));
    g.drawText("Click the [opt] button while holding the Shift key!\nThen move a knob on Synth1 and your MIDI keyboard to bind.",
               282, 196, 304, 56, juce::Justification::topLeft);
}

void OptionsDialog::MidiTab::resized()
{
    ccListBox.setBounds(14, 26, 242, 212);
    updateCcBtn.setBounds(14, 246, 76, 24);
    ccInputEditor.setBounds(96, 246, 50, 24);

    rxBankToggle.setBounds(284, 34, 290, 22);
    rxProgToggle.setBounds(284, 58, 290, 22);

    loadCcBtn.setBounds(294, 126, 80, 26);
    saveCcBtn.setBounds(390, 126, 80, 26);
}

// AudioTab
OptionsDialog::AudioTab::AudioTab(OpenSynth1AudioProcessor& proc, OpenSynth1AudioProcessorEditor* ed)
    : processor(proc), editor(ed)
{
    auto styleDlgBtn = [](juce::TextButton& btn) {
        btn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffdfdcd0));
        btn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffccc8b8));
        btn.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff181818));
        btn.setColour(juce::TextButton::textColourOnId, juce::Colour(0xff181818));
    };

    openAudioSettingsBtn.setButtonText("Audio & MIDI Device Settings...");
    styleDlgBtn(openAudioSettingsBtn);
    openAudioSettingsBtn.onClick = [this]() {
        if (editor)
            editor->openAudioSettings(true);
    };
    addAndMakeVisible(openAudioSettingsBtn);

    statusLabel.setColour(juce::Label::textColourId, juce::Colour(0xff333333));
    statusLabel.setFont(juce::Font(juce::FontOptions(12.5f)));
    statusLabel.setText(
        "OpenSynth1 Standalone communicates directly with your Linux audio and MIDI subsystem.\n\n"
        "Click the button below to select your audio driver (ALSA, PipeWire, JACK),\n"
        "configure output channels, sample rate (44.1k/48k/96k), buffer size, and enable\n"
        "connected MIDI keyboard controllers.",
        juce::dontSendNotification);
    addAndMakeVisible(statusLabel);
}

void OptionsDialog::AudioTab::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xffece9d8));

    g.setColour(juce::Colour(0xff181818));
    g.setFont(juce::Font(juce::FontOptions(14.0f).withStyle("Bold")));
    g.drawText("Hardware Audio & MIDI Configuration", 20, 16, 400, 24, juce::Justification::centredLeft);

    g.setColour(juce::Colour(0xffaaaaaa));
    g.drawRect(20, 44, 570, 1, 1);
}

void OptionsDialog::AudioTab::resized()
{
    statusLabel.setBounds(20, 52, 570, 95);
    openAudioSettingsBtn.setBounds(20, 165, 270, 32);
}

// ==============================================================================
// Main Plugin Editor Component
// ==============================================================================
OpenSynth1AudioProcessorEditor::OpenSynth1AudioProcessorEditor (OpenSynth1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&synth1LookAndFeel);

    // Add mainContainer to this editor
    addAndMakeVisible(mainContainer);

    // Sections creation (attached to mainContainer)
    oscSec = std::make_unique<OscillatorsSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*oscSec);

    filterSec = std::make_unique<FilterSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*filterSec);

    ampSec = std::make_unique<AmpSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*ampSec);

    effectSec = std::make_unique<EffectSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*effectSec);

    eqSec = std::make_unique<EqSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*eqSec);

    delaySec = std::make_unique<DelaySection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*delaySec);

    chorusSec = std::make_unique<ChorusSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*chorusSec);

    voiceSec = std::make_unique<VoiceSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*voiceSec);

    arpSec = std::make_unique<ArpSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*arpSec);

    lfoSec = std::make_unique<LfoSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*lfoSec);

    wheelSec = std::make_unique<WheelMidiSection>(audioProcessor.apvts);
    mainContainer.addAndMakeVisible(*wheelSec);

    // Master Volume knob
    masterVolSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    masterVolSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mainContainer.addAndMakeVisible(masterVolSlider);
    volAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "107", masterVolSlider);

    volLabel.setText("vol", juce::dontSendNotification);
    volLabel.setJustificationType(juce::Justification::centred);
    volLabel.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold")));
    volLabel.setColour(juce::Label::textColourId, juce::Colour(0xffc8ccd8));
    mainContainer.addAndMakeVisible(volLabel);

    // Program Up / Down buttons
    progUpBtn = std::make_unique<Synth1ArrowButton>(true);
    progUpBtn->onClick = [this]() {
        int curProg = audioProcessor.getCurrentProgramNumber();
        if (curProg < 128) {
            audioProcessor.selectBankAndProgram(audioProcessor.getCurrentBankNumber(), curProg + 1);
            updateAllSectionsFromParameters();
            mainContainer.repaint();
        }
    };
    mainContainer.addAndMakeVisible(*progUpBtn);

    progDownBtn = std::make_unique<Synth1ArrowButton>(false);
    progDownBtn->onClick = [this]() {
        int curProg = audioProcessor.getCurrentProgramNumber();
        if (curProg > 1) {
            audioProcessor.selectBankAndProgram(audioProcessor.getCurrentBankNumber(), curProg - 1);
            updateAllSectionsFromParameters();
            mainContainer.repaint();
        }
    };
    mainContainer.addAndMakeVisible(*progDownBtn);

    // Bank/Program Cyan LCD
    programLcd = std::make_unique<MainLcdDisplay>();
    programLcd->onClick = [this]() { showPresetBrowser(); };
    mainContainer.addAndMakeVisible(*programLcd);

    // Write button (dark red)
    writeButton.setButtonText("write");
    writeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff8a1616));
    writeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    writeButton.onClick = [this]() { showWriteDialog(); };
    mainContainer.addAndMakeVisible(writeButton);

    // Init button (dark slate)
    initButton.setButtonText("init");
    initButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3a4252));
    initButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    initButton.onClick = [this]() {
        audioProcessor.resetToInitPreset();
        updateAllSectionsFromParameters();
        if (programLcd) {
            int curBank = audioProcessor.getCurrentBankNumber();
            int curProg = audioProcessor.getCurrentProgramNumber();
            juce::String top = juce::String(curProg) + ":" + audioProcessor.getCurrentProgramName();
            juce::String btm = juce::String::formatted("%02d:00 ", curBank) + audioProcessor.getCurrentBankName();
            programLcd->setTexts(top, btm);
        }
        mainContainer.repaint();
    };
    mainContainer.addAndMakeVisible(initButton);

    // Opt button (dark grey)
    optButton.setButtonText("opt");
    optButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3c3e48));
    optButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    optButton.onClick = [this]() {
        if (juce::ModifierKeys::getCurrentModifiers().isShiftDown()) {
            toggleMidiLearn();
        } else {
            showOptionsDialog();
        }
    };
    mainContainer.addAndMakeVisible(optButton);

    // Voice Activity Meter
    voiceMeter = std::make_unique<VoiceActivityMeter>();
    mainContainer.addAndMakeVisible(*voiceMeter);

    // Pin Always on Top Button (in-GUI quick toggle)
    pinButton.setButtonText("PIN TOP");
    pinButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff383a46));
    pinButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    pinButton.onClick = [this]() {
        setAlwaysOnTopState(!alwaysOnTopState);
    };
    mainContainer.addAndMakeVisible(pinButton);

    // Audio / Hardware Settings Button (standalone device setup)
    audioButton.setButtonText("AUDIO");
    audioButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d303c));
    audioButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff3e4252));
    audioButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xffe8e8e8));
    audioButton.onClick = [this]() {
        openAudioSettings(false);
    };
    mainContainer.addAndMakeVisible(audioButton);

    // Enable Resizing (allows corner dragging and host window resize)
    setResizable(true, true);
    setResizeLimits(765, 465, 2040, 1240);
    if (auto* c = getConstrainer())
        c->setFixedAspectRatio(1020.0 / 620.0);

    setSize(1020, 620);
    startTimerHz(20);
}

OpenSynth1AudioProcessorEditor::~OpenSynth1AudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

#if JUCE_LINUX
#include <dlfcn.h>
static void setLinuxWindowAlwaysOnTop(void* windowHandle, bool onTop)
{
    if (windowHandle == nullptr) return;

    void* x11Lib = dlopen("libX11.so.6", RTLD_LAZY);
    if (!x11Lib) return;

    typedef void* (*XOpenDisplayFunc)(const char*);
    typedef int (*XCloseDisplayFunc)(void*);
    typedef unsigned long (*XDefaultRootWindowFunc)(void*);
    typedef unsigned long (*XInternAtomFunc)(void*, const char*, int);
    typedef int (*XSendEventFunc)(void*, unsigned long, int, long, void*);
    typedef int (*XFlushFunc)(void*);

    auto pXOpenDisplay = (XOpenDisplayFunc) dlsym(x11Lib, "XOpenDisplay");
    auto pXCloseDisplay = (XCloseDisplayFunc) dlsym(x11Lib, "XCloseDisplay");
    auto pXDefaultRootWindow = (XDefaultRootWindowFunc) dlsym(x11Lib, "XDefaultRootWindow");
    auto pXInternAtom = (XInternAtomFunc) dlsym(x11Lib, "XInternAtom");
    auto pXSendEvent = (XSendEventFunc) dlsym(x11Lib, "XSendEvent");
    auto pXFlush = (XFlushFunc) dlsym(x11Lib, "XFlush");

    if (pXOpenDisplay && pXCloseDisplay && pXDefaultRootWindow && pXInternAtom && pXSendEvent && pXFlush)
    {
        if (void* disp = pXOpenDisplay(nullptr))
        {
            unsigned long root = pXDefaultRootWindow(disp);
            unsigned long netWmState = pXInternAtom(disp, "_NET_WM_STATE", 0);
            unsigned long netWmAbove = pXInternAtom(disp, "_NET_WM_STATE_ABOVE", 0);

            struct {
                int type;
                unsigned long serial;
                int send_event;
                void* display;
                unsigned long window;
                unsigned long message_type;
                int format;
                long data[5];
            } ev {};

            ev.type = 33; // ClientMessage
            ev.window = (unsigned long)(uintptr_t) windowHandle;
            ev.message_type = netWmState;
            ev.format = 32;
            ev.data[0] = onTop ? 1 : 0; // 1 = _NET_WM_STATE_ADD, 0 = _NET_WM_STATE_REMOVE
            ev.data[1] = (long) netWmAbove;
            ev.data[2] = 0;
            ev.data[3] = 1;
            ev.data[4] = 0;

            long mask = (1L << 20) | (1L << 19); // SubstructureRedirectMask | SubstructureNotifyMask
            pXSendEvent(disp, root, 0, mask, &ev);
            pXFlush(disp);
            pXCloseDisplay(disp);
        }
    }
    dlclose(x11Lib);
}
#endif

void OpenSynth1AudioProcessorEditor::parentHierarchyChanged()
{
    juce::Component::SafePointer<OpenSynth1AudioProcessorEditor> safeThis (this);
    juce::MessageManager::callAsync ([safeThis]() {
        if (safeThis == nullptr) return;

        if (auto* top = safeThis->getTopLevelComponent())
        {
            if (auto* tlw = dynamic_cast<juce::TopLevelWindow*>(top))
            {
                // Enable native Linux / XFCE window title bar!
                // When using the native title bar, XFCE (xfwm4) renders the standard window title bar
                // with icon, title, buttons, and the full right-click window menu:
                // "Sempre in primo piano", "Ridimensiona", minimize, maximize, close.
                if (!tlw->isUsingNativeTitleBar())
                    tlw->setUsingNativeTitleBar(true);

                if (auto* dw = dynamic_cast<juce::DocumentWindow*>(tlw))
                {
                    dw->setTitleBarButtonsRequired(juce::DocumentWindow::minimiseButton
                                                   | juce::DocumentWindow::maximiseButton
                                                   | juce::DocumentWindow::closeButton,
                                                   true);
                }

                if (auto* rw = dynamic_cast<juce::ResizableWindow*>(tlw))
                {
                    rw->setResizable(true, true);
                    rw->setResizeLimits(765, 465, 2040, 1240);
                }

                if (safeThis->alwaysOnTopState)
                {
                    tlw->setAlwaysOnTop(true);
                   #if JUCE_LINUX
                    setLinuxWindowAlwaysOnTop(tlw->getWindowHandle(), true);
                   #endif
                }
            }
        }
    });
}

void OpenSynth1AudioProcessorEditor::setUiScale(float s)
{
    s = juce::jlimit(0.5f, 3.0f, s);
    int newW = (int)std::round(1020.0f * s);
    int newH = (int)std::round(620.0f * s);

    if (auto* top = getTopLevelComponent())
    {
        if (auto* rw = dynamic_cast<juce::ResizableWindow*>(top))
        {
            rw->setSize(newW, newH);
        }
    }
    setSize(newW, newH);
}

void OpenSynth1AudioProcessorEditor::setAlwaysOnTopState(bool onTop)
{
    alwaysOnTopState = onTop;
    pinButton.setButtonText(alwaysOnTopState ? "PINNED" : "PIN TOP");
    pinButton.setColour(juce::TextButton::buttonColourId,
                        alwaysOnTopState ? juce::Colour(0xff0d7a3e) : juce::Colour(0xff383a46));
    pinButton.repaint();

    if (auto* top = getTopLevelComponent())
    {
        top->setAlwaysOnTop(alwaysOnTopState);
       #if JUCE_LINUX
        setLinuxWindowAlwaysOnTop(top->getWindowHandle(), alwaysOnTopState);
       #endif
    }
}

void OpenSynth1AudioProcessorEditor::openAudioSettings(bool showDirectDialog)
{
   #if JUCE_STANDALONE_APPLICATION
    if (auto* h = juce::StandalonePluginHolder::getInstance())
    {
        if (showDirectDialog)
        {
            h->showAudioSettingsDialog();
            return;
        }

        juce::PopupMenu m;
        m.addItem (1, "Audio / MIDI Device Settings...");
        m.addSeparator();
        m.addItem (2, "Save Current State...");
        m.addItem (3, "Load a Saved State...");
        m.addSeparator();
        m.addItem (4, "Reset to Default State");

        m.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&audioButton),
                         [h, this](int result) {
            if (result == 1)
            {
                h->showAudioSettingsDialog();
            }
            else if (result == 2)
            {
                h->askUserToSaveState();
            }
            else if (result == 3)
            {
                h->askUserToLoadState();
            }
            else if (result == 4)
            {
                audioProcessor.resetToInitPreset();
                updateAllSectionsFromParameters();
            }
        });
        return;
    }
   #endif

    juce::AlertWindow::showMessageBoxAsync (
        juce::AlertWindow::InfoIcon,
        "Audio Settings",
        "OpenSynth1 is currently running as a VST3 plugin inside a DAW host.\n\n"
        "Audio hardware devices, sample rates, and buffer sizes are managed directly by your DAW (Bitwig, Reaper, etc.).\n\n"
        "To change your audio setup, open your DAW's Audio Preferences.");
}

void OpenSynth1AudioProcessorEditor::updateAllSectionsFromParameters()
{
    if (oscSec) { oscSec->updateRadioGroups(); oscSec->updateFromParameters(audioProcessor.apvts); }
    if (effectSec) { effectSec->updateRadioGroups(); effectSec->updateFromParameters(audioProcessor.apvts); }
    if (delaySec) { delaySec->updateRadioGroups(); delaySec->updateFromParameters(audioProcessor.apvts); }
    if (chorusSec) { chorusSec->updateRadioGroups(); chorusSec->updateFromParameters(audioProcessor.apvts); }
    if (voiceSec) { voiceSec->updateRadioGroups(); voiceSec->updateFromParameters(audioProcessor.apvts); }
    if (arpSec) { arpSec->updateRadioGroups(); arpSec->updateFromParameters(audioProcessor.apvts); }
    if (lfoSec) { lfoSec->updateRadioGroups(); lfoSec->updateFromParameters(audioProcessor.apvts); }
    if (wheelSec) { wheelSec->updateRadioGroups(); wheelSec->updateFromParameters(audioProcessor.apvts); }

    if (programLcd) {
        juce::String top = juce::String(audioProcessor.getCurrentProgramNumber()) + ":" + audioProcessor.getCurrentProgramName();
        juce::String btm = juce::String::formatted("%02d:00 ", audioProcessor.getCurrentBankNumber()) + audioProcessor.getCurrentBankName();
        programLcd->setTexts(top, btm);
    }
    mainContainer.repaint();
}

void OpenSynth1AudioProcessorEditor::timerCallback()
{
    if (audioProcessor.consumePresetLoaded()) {
        updateAllSectionsFromParameters();
        mainContainer.repaint();
    }

    // Update active voice meter
    if (voiceMeter) {
        voiceMeter->setActiveMask(audioProcessor.getActiveVoicesMask());
    }

    // Update Program LCD text
    if (programLcd) {
        juce::String top = juce::String(audioProcessor.getCurrentProgramNumber()) + ":" + audioProcessor.getCurrentProgramName();
        juce::String btm = juce::String::formatted("%02d:00 ", audioProcessor.getCurrentBankNumber()) + audioProcessor.getCurrentBankName();
        programLcd->setTexts(top, btm);
    }

    // Regular periodic updates (LCDs, rate/speed texts, radio groups)
    if (oscSec) oscSec->updateFromParameters(audioProcessor.apvts);
    if (effectSec) effectSec->updateFromParameters(audioProcessor.apvts);
    if (delaySec) delaySec->updateFromParameters(audioProcessor.apvts);
    if (chorusSec) chorusSec->updateFromParameters(audioProcessor.apvts);
    if (voiceSec) voiceSec->updateFromParameters(audioProcessor.apvts);
    if (arpSec) arpSec->updateFromParameters(audioProcessor.apvts);
    if (lfoSec) lfoSec->updateFromParameters(audioProcessor.apvts);
    if (wheelSec) wheelSec->updateFromParameters(audioProcessor.apvts);

    // Check learn toast
    if (learnToastTimer > 0) {
        --learnToastTimer;
        if (learnToastTimer == 0) mainContainer.repaint();
    }
}

void OpenSynth1AudioProcessorEditor::toggleMidiLearn()
{
    if (audioProcessor.isMidiLearnActive()) {
        audioProcessor.stopMidiLearn();
        learnToastMessage = "MIDI Learn Cancelled";
    } else {
        audioProcessor.startMidiLearn(19); // Default to Filter Freq (19) or last selected
        learnToastMessage = "★ MIDI LEARN ACTIVE: Move a controller knob on your keyboard! ([Shift]+opt to exit)";
    }
    learnToastTimer = 90;
    mainContainer.repaint();
}

void OpenSynth1AudioProcessorEditor::showPresetBrowser()
{
    if (!presetBrowser) {
        presetBrowser = std::make_unique<PresetBrowserOverlay>(audioProcessor, [this]() { hidePresetBrowser(); });
        mainContainer.addAndMakeVisible(*presetBrowser);
        presetBrowser->setBounds(0, 0, 1020, 620);
    }
}

void OpenSynth1AudioProcessorEditor::hidePresetBrowser()
{
    if (presetBrowser) {
        mainContainer.removeChildComponent(presetBrowser.get());
        presetBrowser = nullptr;
    }
}

void OpenSynth1AudioProcessorEditor::showWriteDialog()
{
    if (!writeDialog) {
        writeDialog = std::make_unique<WriteProgramDialog>(audioProcessor, [this]() { hideWriteDialog(); });
        mainContainer.addAndMakeVisible(*writeDialog);
        writeDialog->setBounds(0, 0, 1020, 620);
    }
}

void OpenSynth1AudioProcessorEditor::hideWriteDialog()
{
    if (writeDialog) {
        mainContainer.removeChildComponent(writeDialog.get());
        writeDialog = nullptr;
    }
}

void OpenSynth1AudioProcessorEditor::showOptionsDialog()
{
    if (!optionsDialog) {
        optionsDialog = std::make_unique<OptionsDialog>(audioProcessor, this, [this]() { hideOptionsDialog(); });
        mainContainer.addAndMakeVisible(*optionsDialog);
        optionsDialog->setBounds(0, 0, 1020, 620);
    }
}

void OpenSynth1AudioProcessorEditor::hideOptionsDialog()
{
    if (optionsDialog) {
        mainContainer.removeChildComponent(optionsDialog.get());
        optionsDialog = nullptr;
    }
}

void OpenSynth1AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff121318));
}

void OpenSynth1AudioProcessorEditor::resized()
{
    float scale = juce::jmin((float)getWidth() / 1020.0f, (float)getHeight() / 620.0f);
    if (scale <= 0.01f) scale = 1.0f;

    float scaledW = 1020.0f * scale;
    float scaledH = 620.0f * scale;
    float offsetX = ((float)getWidth() - scaledW) * 0.5f;
    float offsetY = ((float)getHeight() - scaledH) * 0.5f;

    mainContainer.setBounds(0, 0, 1020, 620);
    mainContainer.setTransform(juce::AffineTransform::scale(scale).translated(offsetX, offsetY));
}

void OpenSynth1AudioProcessorEditor::paintMain (juce::Graphics& g)
{
    // Main Panel Background
    juce::Colour bg = audioProcessor.isPanelCustomColourEnabled() 
                      ? audioProcessor.getPanelCustomColour() 
                      : juce::Colour(0xff22242e);
    g.fillAll(bg);

    // Top Title Bar: OpenSynth1 virtual Analog
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(17.0f).withStyle("Bold")));
    g.drawText("OpenSynth1", 12, 3, 120, 22, juce::Justification::centredLeft);

    g.setColour(juce::Colour(0xffb0b4c2));
    g.setFont(juce::Font(juce::FontOptions(12.5f).withStyle("Italic")));
    g.drawText("virtual Analog", 136, 5, 120, 20, juce::Justification::centredLeft);

    // Red Version Badge: V1.1 - 2026.9
    g.setColour(juce::Colour(0xff9e1616));
    g.fillRect(1020 - 170, 4, 160, 18);
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    g.drawText("V1.1 - 2026.9", 1020 - 170, 4, 160, 18, juce::Justification::centred);

    // MIDI Learn Banner
    if (audioProcessor.isMidiLearnActive() || learnToastTimer > 0)
    {
        g.setColour(juce::Colour(0xffff9900));
        g.fillRect(260, 4, 340, 18);
        g.setColour(juce::Colours::black);
        g.setFont(juce::Font(juce::FontOptions(10.5f).withStyle("Bold")));
        juce::String banner = audioProcessor.isMidiLearnActive() 
                              ? "★ MIDI LEARN: Move a controller knob to bind! [Shift]+opt"
                              : learnToastMessage;
        g.drawText(banner, 260, 4, 340, 18, juce::Justification::centred);
    }
}

void OpenSynth1AudioProcessorEditor::resizedMain()
{
    if (oscSec == nullptr)
        return;

    // Top Bar Buttons: AUDIO and PIN TOP
    pinButton.setBounds(1020 - 170 - 86, 4, 80, 18);
    audioButton.setBounds(1020 - 170 - 86 - 86, 4, 80, 18);

    // Generous, spacious layout across 1020 x 620 with healthy 8px gutters
    // Left Column: 420 px wide (x = 8)
    oscSec->setBounds(8, 32, 420, 220);
    lfoSec->setBounds(8, 260, 420, 144);

    // Center Column: 280 px wide (x = 436)
    ampSec->setBounds(436, 32, 280, 84);
    filterSec->setBounds(436, 124, 280, 156);
    arpSec->setBounds(436, 288, 280, 116);

    // Wheel / MIDI Section: spans across Left + Center columns (width = 708, x = 8)
    wheelSec->setBounds(8, 412, 708, 102);

    // Right Column: 288 px wide (x = 724)
    effectSec->setBounds(724, 32, 288, 64);
    eqSec->setBounds(724, 104, 288, 68);
    delaySec->setBounds(724, 180, 288, 88);
    chorusSec->setBounds(724, 276, 288, 88);
    voiceSec->setBounds(724, 372, 288, 142);

    // Bottom Bar (y = 522 to 614)
    masterVolSlider.setBounds(14, 526, 60, 60);
    volLabel.setBounds(14, 588, 60, 16);

    progUpBtn->setBounds(86, 532, 24, 24);
    progDownBtn->setBounds(86, 562, 24, 24);
    programLcd->setBounds(118, 526, 480, 60);

    writeButton.setBounds(608, 526, 56, 28);
    initButton.setBounds(668, 526, 56, 28);
    optButton.setBounds(608, 558, 116, 28);

    voiceMeter->setBounds(732, 542, 280, 28);

    // Rescale overlays if active inside mainContainer
    if (presetBrowser) presetBrowser->setBounds(0, 0, 1020, 620);
    if (writeDialog) writeDialog->setBounds(0, 0, 1020, 620);
    if (optionsDialog) optionsDialog->setBounds(0, 0, 1020, 620);
}
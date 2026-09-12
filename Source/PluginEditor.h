#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "QuickOptionPicker.h"
#include <vector>
#include <memory>

// ==============================================================================
// Custom LookAndFeel for EXACT Synth1 v1.12 Match
// ==============================================================================
class Synth1LookAndFeel : public juce::LookAndFeel_V4
{
public:
    Synth1LookAndFeel()
    {
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffff1818));
        setColour(juce::Label::textColourId, juce::Colour(0xffc8c8c8));
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff121418));
        setColour(juce::ComboBox::textColourId, juce::Colour(0xff00ffff));
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff444855));
        setColour(juce::TextButton::buttonColourId, juce::Colour(0xff383a44));
        setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
        float minDim = juce::jmin(bounds.getWidth(), bounds.getHeight());
        float radius = minDim * 0.5f - 2.5f;
        float centreX = bounds.getCentreX();
        float centreY = bounds.getCentreY();
        float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Surrounding tick dots arc (~270 degrees sweep)
        const int numDots = 15;
        float dotRadius = radius + 1.8f;
        for (int i = 0; i < numDots; ++i)
        {
            float dotAngle = rotaryStartAngle + (float)i / (float)(numDots - 1) * (rotaryEndAngle - rotaryStartAngle);
            float dx = centreX + std::cos(dotAngle - juce::MathConstants<float>::halfPi) * dotRadius;
            float dy = centreY + std::sin(dotAngle - juce::MathConstants<float>::halfPi) * dotRadius;
            g.setColour(juce::Colour(0xff7a8094));
            g.fillEllipse(dx - 0.9f, dy - 0.9f, 1.8f, 1.8f);
        }

        // Highlight if MIDI learn target
        bool isLearnTarget = slider.getProperties().contains("isMidiLearn");
        if (isLearnTarget)
        {
            g.setColour(juce::Colour(0xffffcc00).withAlpha(0.65f));
            g.fillEllipse(centreX - radius - 2.0f, centreY - radius - 2.0f, (radius + 2.0f) * 2.0f, (radius + 2.0f) * 2.0f);
        }

        // Dark outer knob body
        g.setColour(juce::Colour(0xff121318));
        g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

        // Bevel outer ring
        g.setColour(juce::Colour(0xff3a3e4c));
        g.drawEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.4f);

        // Inner cap
        float innerRadius = radius * 0.74f;
        juce::ColourGradient grad(juce::Colour(0xff2d313c), centreX, centreY - innerRadius,
                                  juce::Colour(0xff14151a), centreX, centreY + innerRadius, false);
        g.setGradientFill(grad);
        g.fillEllipse(centreX - innerRadius, centreY - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);

        // Red indicator line needle
        juce::Path p;
        float needleLen = radius * 0.90f;
        p.addRectangle(-1.5f, -needleLen, 3.0f, needleLen * 0.92f);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        g.setColour(juce::Colour(0xffff1818));
        g.fillPath(p);
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        bool isGreen = button.getProperties().contains("isGreen");
        bool isCheckbox = button.getProperties().contains("isCheckbox")
                          || button.isColourSpecified(juce::ToggleButton::textColourId);
        bool on = button.getToggleState();

        if (isCheckbox)
        {
            float boxSize = 13.0f;
            float boxX = 2.0f;
            float boxY = (bounds.getHeight() - boxSize) * 0.5f;

            // White box with border
            g.setColour(juce::Colours::white);
            g.fillRect(boxX, boxY, boxSize, boxSize);
            g.setColour(juce::Colour(0xff7f9db9));
            g.drawRect(boxX, boxY, boxSize, boxSize, 1.2f);

            if (on)
            {
                // Crisp green checkmark
                g.setColour(juce::Colour(0xff0e7e12));
                juce::Path p;
                p.startNewSubPath(boxX + 2.5f, boxY + 6.5f);
                p.lineTo(boxX + 5.0f, boxY + 10.0f);
                p.lineTo(boxX + 10.5f, boxY + 3.0f);
                g.strokePath(p, juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::square));
            }

            // Text
            if (button.getButtonText().isNotEmpty())
            {
                auto textCol = button.isColourSpecified(juce::ToggleButton::textColourId)
                                   ? button.findColour(juce::ToggleButton::textColourId)
                                   : juce::Colour(0xff181818);
                g.setColour(textCol);
                g.setFont(juce::Font(juce::FontOptions(11.5f).withStyle("Bold")));
                g.drawText(button.getButtonText(), (int)(boxX + boxSize + 6.0f), 0,
                           (int)(bounds.getWidth() - boxSize - 8.0f), (int)bounds.getHeight(),
                           juce::Justification::centredLeft);
            }
            return;
        }

        float ledSize = 7.0f;
        float ledX = 1.0f;
        float ledY = (bounds.getHeight() - ledSize) * 0.5f;

        // Draw bezel
        g.setColour(juce::Colour(0xff121418));
        g.fillEllipse(ledX - 1.0f, ledY - 1.0f, ledSize + 2.0f, ledSize + 2.0f);
        g.setColour(juce::Colour(0xff404450));
        g.drawEllipse(ledX - 1.0f, ledY - 1.0f, ledSize + 2.0f, ledSize + 2.0f, 1.0f);

        // LED color
        if (isGreen)
        {
            if (on) {
                g.setColour(juce::Colour(0xff00ff44));
                g.fillEllipse(ledX, ledY, ledSize, ledSize);
                g.setColour(juce::Colours::white.withAlpha(0.6f));
                g.fillEllipse(ledX + 1.5f, ledY + 1.0f, 2.5f, 2.0f);
            } else {
                g.setColour(juce::Colour(0xff0a300a));
                g.fillEllipse(ledX, ledY, ledSize, ledSize);
            }
        }
        else
        {
            if (on) {
                g.setColour(juce::Colour(0xffff2020));
                g.fillEllipse(ledX, ledY, ledSize, ledSize);
                g.setColour(juce::Colours::white.withAlpha(0.6f));
                g.fillEllipse(ledX + 1.5f, ledY + 1.0f, 2.5f, 2.0f);
            } else {
                g.setColour(juce::Colour(0xff3a0808));
                g.fillEllipse(ledX, ledY, ledSize, ledSize);
            }
        }

        // Text
        if (button.getButtonText().isNotEmpty())
        {
            auto textCol = button.isColourSpecified(juce::ToggleButton::textColourId)
                               ? button.findColour(juce::ToggleButton::textColourId)
                               : (on ? juce::Colours::white : juce::Colour(0xffccd0de));
            g.setColour(textCol);
            g.setFont(juce::Font(juce::FontOptions(11.5f).withStyle("Bold")));
            g.drawText(button.getButtonText(), (int)(ledX + ledSize + 4.0f), 0,
                       (int)(bounds.getWidth() - ledSize - 5.0f), (int)bounds.getHeight(),
                       juce::Justification::centredLeft);
        }
    }

    juce::Font getLabelFont(juce::Label&) override {
        return juce::Font(juce::FontOptions(11.5f).withStyle("Bold"));
    }
};

// ==============================================================================
// Helper UI Components (Waveform display, Cyan LCD, Section)
// ==============================================================================
// ==============================================================================
class WaveformIcon : public juce::Component
{
public:
    int type = 0; // 0=saw, 1=tri, 2=square, 3=s&h, 4=random/noise, 5=sine
    std::function<void()> onClick;

    WaveformIcon(int t) : type(t) {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void mouseUp(const juce::MouseEvent&) override {
        if (onClick) onClick();
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colour(0xffc8ccdc));
        juce::Path p;
        float w = (float)getWidth();
        float h = (float)getHeight();
        float midY = h * 0.5f;

        if (type == 0) { // Saw (ramp down)
            p.startNewSubPath(1.0f, h - 1.0f);
            p.lineTo(w - 1.0f, 1.0f);
            p.lineTo(w - 1.0f, h - 1.0f);
        } else if (type == 1) { // Tri
            p.startNewSubPath(1.0f, h - 1.0f);
            p.lineTo(w * 0.5f, 1.0f);
            p.lineTo(w - 1.0f, h - 1.0f);
        } else if (type == 2) { // Square / Pulse
            p.startNewSubPath(1.0f, h - 1.0f);
            p.lineTo(1.0f, 1.0f);
            p.lineTo(w * 0.5f, 1.0f);
            p.lineTo(w * 0.5f, h - 1.0f);
            p.lineTo(w - 1.0f, h - 1.0f);
        } else if (type == 3) { // S&H (stepped)
            p.startNewSubPath(1.0f, h - 2.0f);
            p.lineTo(w * 0.33f, h - 2.0f);
            p.lineTo(w * 0.33f, 2.0f);
            p.lineTo(w * 0.66f, 2.0f);
            p.lineTo(w * 0.66f, midY);
            p.lineTo(w - 1.0f, midY);
        } else if (type == 4) { // Noise / Random Smoothed
            p.startNewSubPath(1.0f, midY);
            p.lineTo(w * 0.2f, 2.0f);
            p.lineTo(w * 0.4f, h - 2.0f);
            p.lineTo(w * 0.6f, 3.0f);
            p.lineTo(w * 0.8f, h - 3.0f);
            p.lineTo(w - 1.0f, midY);
        } else if (type == 5) { // Sine
            p.startNewSubPath(1.0f, midY);
            p.quadraticTo(w * 0.25f, 1.0f, w * 0.5f, midY);
            p.quadraticTo(w * 0.75f, h - 1.0f, w - 1.0f, midY);
        }
        g.strokePath(p, juce::PathStrokeType(1.4f));
    }
};

class CyanLcdBox : public juce::Component
{
public:
    juce::String text;
    float fontSize = 12.0f;
    std::function<void()> onClick;
    std::function<void(int)> onDelta;

    CyanLcdBox(const juce::String& t = "", float fSize = 12.0f) : text(t), fontSize(fSize) {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void setText(const juce::String& newText) {
        if (text != newText) {
            text = newText;
            repaint();
        }
    }

    int lastY = 0;
    bool wasDragged = false;

    void mouseDown(const juce::MouseEvent& e) override {
        lastY = e.y;
        wasDragged = false;
    }

    void mouseDrag(const juce::MouseEvent& e) override {
        if (onDelta) {
            int delta = lastY - e.y;
            if (std::abs(delta) >= 3) {
                wasDragged = true;
                onDelta(delta > 0 ? 1 : -1);
                lastY = e.y;
            }
        }
    }

    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& wheel) override {
        if (onDelta && std::abs(wheel.deltaY) > 0.01f) {
            onDelta(wheel.deltaY > 0 ? 1 : -1);
        }
    }

    void mouseUp(const juce::MouseEvent&) override {
        if (!wasDragged && onClick) onClick();
    }

    void paint(juce::Graphics& g) override
    {
        // Dark teal LCD screen
        g.setColour(juce::Colour(0xff021b1b));
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 2.0f);
        g.setColour(juce::Colour(0xff004444));
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 2.0f, 1.0f);

        // Glowing cyan digital text
        g.setColour(juce::Colour(0xff00f4f4));
        g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fontSize, juce::Font::bold)));
        g.drawText(text, getLocalBounds().reduced(2), juce::Justification::centred);
    }
};

class LfoBadgeButton : public juce::Component
{
public:
    juce::String text;
    bool isOn = true;
    std::function<void()> onClick;

    LfoBadgeButton(const juce::String& t) : text(t) {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void setOn(bool on) {
        if (isOn != on) {
            isOn = on;
            repaint();
        }
    }

    void mouseDown(const juce::MouseEvent&) override {
        if (onClick) onClick();
    }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        if (isOn) {
            // Lit yellow badge
            g.setColour(juce::Colour(0xfffeff99));
            g.fillRect(bounds);
            g.setColour(juce::Colour(0xffb0b400));
            g.drawRect(bounds, 1.0f);
            g.setColour(juce::Colour(0xff14151a));
        } else {
            // Dark / Off badge
            g.setColour(juce::Colour(0xff343628));
            g.fillRect(bounds);
            g.setColour(juce::Colour(0xff22241a));
            g.drawRect(bounds, 1.0f);
            g.setColour(juce::Colour(0xff707460));
        }
        g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
        g.drawText(text, getLocalBounds(), juce::Justification::centred);
    }
};

class Synth1ArrowButton : public juce::Component
{
public:
    bool isUp = true;
    std::function<void()> onClick;

    Synth1ArrowButton(bool up) : isUp(up) {}

    void mouseDown(const juce::MouseEvent&) override {
        if (onClick) onClick();
    }

    void paint(juce::Graphics& g) override {
        auto bounds = getLocalBounds().toFloat();
        g.setColour(juce::Colour(0xff323540));
        g.fillRect(bounds);
        g.setColour(juce::Colour(0xff181a20));
        g.drawRect(bounds, 1.0f);

        juce::Path p;
        float cx = bounds.getCentreX();
        float cy = bounds.getCentreY();
        if (isUp) {
            p.addTriangle(cx, cy - 3.5f, cx - 4.0f, cy + 3.0f, cx + 4.0f, cy + 3.0f);
        } else {
            p.addTriangle(cx, cy + 3.5f, cx - 4.0f, cy - 3.0f, cx + 4.0f, cy - 3.0f);
        }
        g.setColour(juce::Colour(0xffc8ccd8));
        g.fillPath(p);
    }
};

class MainLcdDisplay : public juce::Component
{
public:
    juce::String programText = "1:initial sound";
    juce::String bankText = "00:00 Empty";
    std::function<void()> onClick;

    void setTexts(const juce::String& p, const juce::String& b) {
        if (programText != p || bankText != b) {
            programText = p;
            bankText = b;
            repaint();
        }
    }

    void mouseDown(const juce::MouseEvent&) override {
        if (onClick) onClick();
    }

    void mouseUp(const juce::MouseEvent&) override {
        if (onClick) onClick();
    }

    void paint(juce::Graphics& g) override {
        auto bounds = getLocalBounds().toFloat();
        // LCD glass
        g.setColour(juce::Colour(0xff021b1b));
        g.fillRoundedRectangle(bounds, 2.0f);
        g.setColour(juce::Colour(0xff004848));
        g.drawRoundedRectangle(bounds, 2.0f, 1.0f);

        // Row 1: Program name (bright cyan)
        g.setColour(juce::Colour(0xff00f8f8));
        g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), 13.5f, juce::Font::bold)));
        g.drawText(programText, 8, 2, getWidth() - 16, 18, juce::Justification::centredLeft);

        // Row 2: Bank name (teal cyan)
        g.setColour(juce::Colour(0xff00c4c4));
        g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), 11.5f, juce::Font::bold)));
        g.drawText(bankText, 8, 20, getWidth() - 16, 18, juce::Justification::centredLeft);
    }
};

// ==============================================================================
// Radio Group Helper for Parameter Values (0, 1, 2, 3...)
// ==============================================================================
class Synth1RadioGroup : public juce::AudioProcessorValueTreeState::Listener
{
public:
    Synth1RadioGroup(juce::AudioProcessorValueTreeState& state, const juce::String& paramId, int radioGroupId)
        : apvts(state), paramID(paramId), groupId(radioGroupId)
    {
        apvts.addParameterListener(paramID, this);
    }

    ~Synth1RadioGroup() override
    {
        apvts.removeParameterListener(paramID, this);
    }

    const juce::String& getParamID() const { return paramID; }

    void addButton(juce::ToggleButton* btn, int valueIndex)
    {
        btn->setRadioGroupId(groupId);
        buttons.push_back(btn);
        values.push_back(valueIndex);

        btn->onClick = [this, btn, valueIndex]() {
            if (btn->getToggleState()) {
                if (auto* p = apvts.getParameter(paramID))
                    p->setValueNotifyingHost(p->getNormalisableRange().convertTo0to1((float)valueIndex));
            }
        };

        // Initial state
        if (auto* p = apvts.getParameter(paramID)) {
            int curVal = (int)std::round(p->getNormalisableRange().convertFrom0to1(p->getValue()));
            btn->setToggleState(curVal == valueIndex, juce::dontSendNotification);
        }
    }

    void parameterChanged(const juce::String& parameterID, float newValue) override
    {
        if (parameterID == paramID) {
            juce::MessageManager::callAsync([this, newValue]() {
                int curVal = (int)std::round(newValue);
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i] != nullptr) {
                        buttons[i]->setToggleState(values[i] == curVal, juce::dontSendNotification);
                    }
                }
            });
        }
    }

    void updateFromParameter()
    {
        if (auto* p = apvts.getParameter(paramID)) {
            int curVal = (int)std::round(p->getNormalisableRange().convertFrom0to1(p->getValue()));
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i] != nullptr) {
                    bool shouldBeOn = (values[i] == curVal);
                    if (buttons[i]->getToggleState() != shouldBeOn) {
                        buttons[i]->setToggleState(shouldBeOn, juce::dontSendNotification);
                    }
                }
            }
        }
    }

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::String paramID;
    int groupId;
    std::vector<juce::ToggleButton*> buttons;
    std::vector<int> values;
};

// -------------------------------------------------------------
// Note Division String Helpers (Musical values for rate/speed)
// -------------------------------------------------------------
inline juce::String getArpBeatString(int val)
{
    static const char* beats[] = {
        "1", "1/2.", "1/2", "1/2T", "1/4.", "1/4", "1/4T",
        "1/8.", "1/8", "1/8T", "1/16.", "1/16", "1/16T",
        "1/32.", "1/32", "1/32T", "1/64", "1/64T", "1/128"
    };
    int idx = (val <= 18) ? juce::jlimit(0, 18, val) : juce::jlimit(0, 18, (int)std::round((float)val / 127.0f * 18.0f));
    return beats[idx];
}

inline juce::String getLfoSpeedString(int val, bool isSynced)
{
    if (isSynced) {
        static const char* syncs[] = {
            "16", "8", "4", "2", "1", "1/2.", "1/2", "1/2T",
            "1/4.", "1/4", "1/4T", "1/8.", "1/8", "1/8T", "1/16.", "1/16", "1/32"
        };
        int idx = juce::jlimit(0, 16, (int)std::round((float)val / 127.0f * 16.0f));
        return syncs[idx];
    }
    float hz = 0.1f * std::pow(300.0f, (float)val / 127.0f);
    if (hz < 10.0f) return juce::String(hz, 1) + "Hz";
    return juce::String((int)std::round(hz)) + "Hz";
}

inline juce::String getDelayTimeString(int val)
{
    static const char* delays[] = {
        "0.1ms", "1/32T", "1/32", "1/16T", "1/32.", "1/16", "1/8T", "1/16.",
        "1/8", "1/4T", "1/8.", "1/4", "1/2T", "1/4.", "1/2", "1T", "1/2.", "1", "2", "4"
    };
    int idx = juce::jlimit(0, 19, val);
    return delays[idx];
}

// ==============================================================================
// Section Base Class
// ==============================================================================
class Synth1Section : public juce::Component
{
public:
    Synth1Section(const juce::String& title) : sectionTitle(title) {}

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        // Dark section panel background
        g.setColour(customPanelBg.getAlpha() > 0 ? customPanelBg : juce::Colour(0xff21232c));
        g.fillRect(bounds);

        // Silver-grey header bar (exact Synth1 style)
        juce::ColourGradient headerGrad(juce::Colour(0xffdcdfe8), 0.0f, 0.0f,
                                        juce::Colour(0xffb2b6c2), 0.0f, 18.0f, false);
        g.setGradientFill(headerGrad);
        g.fillRect(0, 0, getWidth(), 18);

        // Header Title in bold black text
        g.setColour(juce::Colour(0xff16171c));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText(sectionTitle, 7, 0, getWidth() - 10, 18, juce::Justification::centredLeft);

        // Outer border
        g.setColour(juce::Colour(0xff121318));
        g.drawRect(bounds, 1.0f);
        g.setColour(juce::Colour(0xff3c404e));
        g.drawRect(bounds.reduced(1.0f), 1.0f);
    }

    void setCustomBgColour(juce::Colour c) {
        customPanelBg = c;
        repaint();
    }

    void updateRadioGroups()
    {
        for (auto& rg : radioGroups) {
            if (rg) rg->updateFromParameter();
        }
    }

protected:
    juce::String sectionTitle;
    juce::Colour customPanelBg;

    std::vector<std::unique_ptr<juce::Component>> controls;
    std::vector<std::unique_ptr<juce::Label>> labels;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;
    std::vector<std::unique_ptr<Synth1RadioGroup>> radioGroups;
    std::vector<std::unique_ptr<WaveformIcon>> waveformIcons;

    juce::Slider* addKnob(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID,
                          const juce::String& labelText, int x, int y, int size = 32, juce::Label** outLabel = nullptr)
    {
        auto slider = std::make_unique<juce::Slider>(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox);
        slider->setBounds(x, y, size, size);
        slider->setDoubleClickReturnValue(true, 64.0);
        addAndMakeVisible(*slider);

        if (labelText.isNotEmpty()) {
            auto label = std::make_unique<juce::Label>("", labelText);
            label->setJustificationType(juce::Justification::centred);
            label->setBounds(x - 10, y + size - 1, size + 20, 15);
            label->setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
            label->setColour(juce::Label::textColourId, juce::Colour(0xffdcdfe8));
            addAndMakeVisible(*label);
            if (outLabel != nullptr) *outLabel = label.get();
            labels.push_back(std::move(label));
        }

        sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, *slider));

        auto* s = slider.get();
        controls.push_back(std::move(slider));
        return s;
    }

    juce::ToggleButton* addLedToggle(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID,
                                     const juce::String& labelText, int x, int y, int w, int h, bool isGreen = false)
    {
        auto btn = std::make_unique<juce::ToggleButton>(labelText);
        btn->setBounds(x, y, w, h);
        if (isGreen) btn->getProperties().set("isGreen", true);
        addAndMakeVisible(*btn);

        buttonAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, paramID, *btn));

        auto* b = btn.get();
        controls.push_back(std::move(btn));
        return b;
    }

    Synth1RadioGroup* getOrCreateRadioGroup(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, int groupId)
    {
        for (auto& rg : radioGroups) {
            if (rg->getParamID() == paramID) return rg.get();
        }
        radioGroups.push_back(std::make_unique<Synth1RadioGroup>(apvts, paramID, groupId));
        return radioGroups.back().get();
    }

    juce::ToggleButton* addRadioToggle(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID,
                                       int valueIndex, int groupId, const juce::String& labelText,
                                       int x, int y, int w, int h, bool isGreen = false)
    {
        auto btn = std::make_unique<juce::ToggleButton>(labelText);
        btn->setBounds(x, y, w, h);
        if (isGreen) btn->getProperties().set("isGreen", true);
        btn->getProperties().set("isRadio", true);
        addAndMakeVisible(*btn);

        auto* rg = getOrCreateRadioGroup(apvts, paramID, groupId);
        rg->addButton(btn.get(), valueIndex);

        auto* b = btn.get();
        controls.push_back(std::move(btn));
        return b;
    }

    void addWfIcon(int x, int y, int type)
    {
        auto icon = std::make_unique<WaveformIcon>(type);
        icon->setBounds(x, y, 12, 9);
        addAndMakeVisible(*icon);
        waveformIcons.push_back(std::move(icon));
    }
};

// ==============================================================================
// Concrete Sections (Oscillators, Filter, Amp, FX, EQ, Delay, Chorus, Voice, Arp, LFO, Wheel)
// ==============================================================================
class OscillatorsSection : public Synth1Section
{
public:
    OscillatorsSection(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void updateFromParameters(juce::AudioProcessorValueTreeState& apvts);
private:
    std::unique_ptr<CyanLcdBox> pitchLcd;
};

class FilterSection : public Synth1Section
{
public:
    FilterSection(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
};

class AmpSection : public Synth1Section
{
public:
    AmpSection(juce::AudioProcessorValueTreeState& apvts);
};

class EffectSection : public Synth1Section
{
public:
    EffectSection(juce::AudioProcessorValueTreeState& apvts);
    void updateFromParameters(juce::AudioProcessorValueTreeState& apvts);
private:
    std::unique_ptr<CyanLcdBox> typeLcd;
};

class EqSection : public Synth1Section
{
public:
    EqSection(juce::AudioProcessorValueTreeState& apvts);
};

class DelaySection : public Synth1Section
{
public:
    DelaySection(juce::AudioProcessorValueTreeState& apvts);
    void updateFromParameters(juce::AudioProcessorValueTreeState& apvts);
private:
    std::unique_ptr<CyanLcdBox> typeLcd;
    juce::Label* timeLabel = nullptr;
};

class ChorusSection : public Synth1Section
{
public:
    ChorusSection(juce::AudioProcessorValueTreeState& apvts);
    void updateFromParameters(juce::AudioProcessorValueTreeState& apvts);
private:
    std::unique_ptr<CyanLcdBox> typeLcd;
};

class VoiceSection : public Synth1Section
{
public:
    VoiceSection(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void updateFromParameters(juce::AudioProcessorValueTreeState& apvts);
private:
    std::unique_ptr<CyanLcdBox> polyLcd;
    std::unique_ptr<CyanLcdBox> unisonNumLcd;
};

class ArpSection : public Synth1Section
{
public:
    ArpSection(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void updateFromParameters(juce::AudioProcessorValueTreeState& apvts);
private:
    juce::Label* beatLabel = nullptr;
};

class LfoSection : public Synth1Section
{
public:
    LfoSection(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void updateFromParameters(juce::AudioProcessorValueTreeState& apvts);
private:
    std::unique_ptr<LfoBadgeButton> lfo1BadgeBtn;
    std::unique_ptr<LfoBadgeButton> lfo2BadgeBtn;
    std::unique_ptr<WaveformIcon> lfo1Icon;
    std::unique_ptr<WaveformIcon> lfo2Icon;
    juce::Label* lfo1SpdLabel = nullptr;
    juce::Label* lfo2SpdLabel = nullptr;
};

class WheelMidiSection : public Synth1Section
{
public:
    WheelMidiSection(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void updateFromParameters(juce::AudioProcessorValueTreeState& apvts);
private:
    std::unique_ptr<CyanLcdBox> pbLcd;
    std::unique_ptr<CyanLcdBox> src1Lcd;
    std::unique_ptr<CyanLcdBox> dst1Lcd;
    std::unique_ptr<CyanLcdBox> src2Lcd;
    std::unique_ptr<CyanLcdBox> dst2Lcd;
};

// ==============================================================================
// Voice Activity 16-LED Meter Component
// ==============================================================================
class VoiceActivityMeter : public juce::Component
{
public:
    void setActiveMask(uint32_t mask) {
        if (activeMask != mask) {
            activeMask = mask;
            repaint();
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xff101216));
        g.setColour(juce::Colour(0xff2d303a));
        g.drawRect(getLocalBounds(), 1);

        float segW = (float)getWidth() / 16.0f;
        float segH = (float)getHeight() - 4.0f;

        for (int i = 0; i < 16; ++i)
        {
            bool on = (activeMask & (1u << i)) != 0;
            float x = (float)i * segW + 1.5f;
            float y = 2.0f;

            if (on) {
                g.setColour(juce::Colour(0xffff2020));
                g.fillRect(x, y, segW - 2.0f, segH);
                g.setColour(juce::Colours::white.withAlpha(0.6f));
                g.fillRect(x + 1.0f, y + 1.0f, segW - 4.0f, 2.0f);
            } else {
                g.setColour(juce::Colour(0xff280a0a));
                g.fillRect(x, y, segW - 2.0f, segH);
            }
        }
    }

private:
    uint32_t activeMask = 0;
};

// ==============================================================================
// Subwindow: Preset Browser Overlay (PDF page 73)
// ==============================================================================
class PresetBrowserOverlay : public juce::Component
{
public:
    PresetBrowserOverlay(OpenSynth1AudioProcessor& proc, std::function<void()> onClose);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void refresh();

private:
    OpenSynth1AudioProcessor& processor;
    std::function<void()> closeCallback;

    juce::ListBox bankListBox;
    juce::TextButton closeButton;
    juce::TextButton listInfoTab;

    // Bank list model
    class BankListModel : public juce::ListBoxModel
    {
    public:
        BankListModel(PresetBrowserOverlay& o) : owner(o) {}
        int getNumRows() override;
        void paintListBoxItem(int row, juce::Graphics& g, int w, int h, bool rowIsSelected) override;
        void selectedRowsChanged(int lastRowSelected) override;
    private:
        PresetBrowserOverlay& owner;
    } bankListModel;

    // Grid of 128 programs (4 columns x 32 rows)
    class ProgramGridComponent : public juce::Component
    {
    public:
        ProgramGridComponent(PresetBrowserOverlay& o) : owner(o) {}
        void paint(juce::Graphics& g) override;
        void mouseDown(const juce::MouseEvent& e) override;
    private:
        PresetBrowserOverlay& owner;
    } programGrid;
};

// ==============================================================================
// Subwindow: Over Write Program Dialog (PDF page 68)
// ==============================================================================
class WriteProgramDialog : public juce::Component
{
public:
    WriteProgramDialog(OpenSynth1AudioProcessor& proc, std::function<void()> onClose);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    OpenSynth1AudioProcessor& processor;
    std::function<void()> closeCallback;

    juce::ComboBox bankCombo;
    juce::TextButton programPickerBtn;  // opens a QuickOptionPicker over all 128 slots
    juce::Label programSlotLabel;       // shows what's currently in the chosen slot
    int selectedProgramIndex = 1;       // 1-based, matches Synth1ProgramInfo::index
    juce::TextEditor nameEditor;
    juce::ComboBox colorCombo;
    juce::TextButton okButton;
    juce::TextButton cancelButton;
    juce::TextButton closeButton;

    // Rebuilds the 128 program-slot names for whichever bank is currently
    // selected in bankCombo, e.g. "014: Fat Bass" or "014: (empty)".
    juce::StringArray buildProgramSlotItems() const;
    void refreshProgramSlotUi();
};

class OpenSynth1AudioProcessorEditor;

// ==============================================================================
// Subwindow: Options Dialog (PDF pages 69 & 72)
// ==============================================================================
class OptionsDialog : public juce::Component
{
public:
    OptionsDialog(OpenSynth1AudioProcessor& proc, OpenSynth1AudioProcessorEditor* ed, std::function<void()> onClose);
    void paint(juce::Graphics& g) override;
    void resized() override;

    // General Tab
    class GeneralTab : public juce::Component
    {
    public:
        GeneralTab(OpenSynth1AudioProcessor& proc, OpenSynth1AudioProcessorEditor* ed);
        void paint(juce::Graphics& g) override;
        void resized() override;
    private:
        OpenSynth1AudioProcessor& processor;
        OpenSynth1AudioProcessorEditor* editor = nullptr;
        juce::TextEditor bankPathEditor;
        juce::TextButton copyPathBtn;
        juce::TextButton browseBankBtn;
        juce::Slider tuneSlider;
        juce::TextButton minusTuneBtn;
        juce::TextButton plusTuneBtn;
        juce::TextButton resetTuneBtn;
        juce::Label tuneReadoutLabel;

        juce::ComboBox sizeCombo;
        juce::ToggleButton alwaysOnTopToggle;
        juce::ToggleButton sliderWinToggle;
        juce::ToggleButton selectProgToggle;
        juce::ToggleButton panelColToggle;
        juce::TextEditor panelColEditor;
        juce::ToggleButton textColToggle;
        juce::TextEditor textColEditor;
        juce::ToggleButton transpToggle;
        juce::ComboBox themePresetCombo;
        std::unique_ptr<juce::FileChooser> fileChooser;
    };

    // MIDI Tab
    class MidiTab : public juce::Component
    {
    public:
        MidiTab(OpenSynth1AudioProcessor& proc);
        void paint(juce::Graphics& g) override;
        void resized() override;
    private:
        OpenSynth1AudioProcessor& processor;
        juce::ListBox ccListBox;
        juce::TextButton updateCcBtn;
        juce::TextEditor ccInputEditor;
        juce::ToggleButton rxBankToggle;
        juce::ToggleButton rxProgToggle;
        juce::TextButton loadCcBtn;
        juce::TextButton saveCcBtn;

        class CcListModel : public juce::ListBoxModel
        {
        public:
            CcListModel(OpenSynth1AudioProcessor& p) : proc(p) {}
            int getNumRows() override;
            void paintListBoxItem(int row, juce::Graphics& g, int w, int h, bool rowIsSelected) override;
        private:
            OpenSynth1AudioProcessor& proc;
        } ccModel;
    };

    // Audio Tab (Standalone audio/midi hardware setup)
    class AudioTab : public juce::Component
    {
    public:
        AudioTab(OpenSynth1AudioProcessor& proc, OpenSynth1AudioProcessorEditor* ed);
        void paint(juce::Graphics& g) override;
        void resized() override;
    private:
        OpenSynth1AudioProcessor& processor;
        OpenSynth1AudioProcessorEditor* editor = nullptr;
        juce::TextButton openAudioSettingsBtn;
        juce::Label statusLabel;
    };

private:
    OpenSynth1AudioProcessor& processor;
    OpenSynth1AudioProcessorEditor* editor = nullptr;
    std::function<void()> closeCallback;

    juce::TabbedComponent tabs;
    juce::TextButton okButton;
    juce::TextButton cancelButton;
    juce::TextButton closeButton;
};

// ==============================================================================
// Main Plugin Editor Component
// ==============================================================================
class OpenSynth1AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Timer
{
public:
    OpenSynth1AudioProcessorEditor (OpenSynth1AudioProcessor&);
    ~OpenSynth1AudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    void showPresetBrowser();
    void hidePresetBrowser();
    void showWriteDialog();
    void hideWriteDialog();
    void showOptionsDialog();
    void hideOptionsDialog();
    void toggleMidiLearn();
    void updateAllSectionsFromParameters();

    void parentHierarchyChanged() override;
    void setUiScale(float s);
    bool isAlwaysOnTop() const { return alwaysOnTopState; }
    void setAlwaysOnTopState(bool onTop);
    void openAudioSettings(bool showDirectDialog = false);

    void paintMain(juce::Graphics& g);
    void resizedMain();

private:
    class MainContainer : public juce::Component
    {
    public:
        MainContainer (OpenSynth1AudioProcessorEditor& ed) : editor (ed) {}
        void paint (juce::Graphics& g) override { editor.paintMain (g); }
        void resized() override { editor.resizedMain(); }
    private:
        OpenSynth1AudioProcessorEditor& editor;
    };

    MainContainer mainContainer { *this };
    juce::TextButton pinButton;
    juce::TextButton audioButton;
    bool alwaysOnTopState = false;

    OpenSynth1AudioProcessor& audioProcessor;
    Synth1LookAndFeel synth1LookAndFeel;

    // Sections
    std::unique_ptr<OscillatorsSection> oscSec;
    std::unique_ptr<FilterSection> filterSec;
    std::unique_ptr<AmpSection> ampSec;
    std::unique_ptr<EffectSection> effectSec;
    std::unique_ptr<EqSection> eqSec;
    std::unique_ptr<DelaySection> delaySec;
    std::unique_ptr<ChorusSection> chorusSec;
    std::unique_ptr<VoiceSection> voiceSec;
    std::unique_ptr<ArpSection> arpSec;
    std::unique_ptr<LfoSection> lfoSec;
    std::unique_ptr<WheelMidiSection> wheelSec;

    // Bottom Bar Controls
    juce::Slider masterVolSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volAttachment;
    juce::Label volLabel;

    std::unique_ptr<Synth1ArrowButton> progUpBtn;
    std::unique_ptr<Synth1ArrowButton> progDownBtn;
    std::unique_ptr<MainLcdDisplay> programLcd;

    juce::TextButton writeButton;
    juce::TextButton initButton;
    juce::TextButton optButton;
    std::unique_ptr<VoiceActivityMeter> voiceMeter;

    // Active subwindows / overlays
    std::unique_ptr<PresetBrowserOverlay> presetBrowser;
    std::unique_ptr<WriteProgramDialog> writeDialog;
    std::unique_ptr<OptionsDialog> optionsDialog;

    // Learn Banner
    juce::String learnToastMessage;
    int learnToastTimer = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenSynth1AudioProcessorEditor)
};

// Backward compatibility alias
using Synth1CloneAudioProcessorEditor = OpenSynth1AudioProcessorEditor;

#pragma once
#include <JuceHeader.h>
#include <functional>

/**
 * QuickOptionPicker
 * ------------------
 * A small, fast, searchable list popup used anywhere the plugin needs the
 * user to choose one option out of many (program slots, effect/delay/chorus
 * types, MIDI destinations, voice counts, etc.).
 *
 * Why it exists: several places in the UI used a plain juce::ComboBox or a
 * juce::PopupMenu for lists that can be long (e.g. 128 program slots). Both
 * force the user to scroll through everything with the mouse. This widget
 * adds a text box at the top - start typing and the list filters instantly,
 * press Enter to confirm the top match, Up/Down to move the highlight,
 * Escape to cancel. The full option text is always shown at readable size
 * (no truncated abbreviations), and the currently-selected item is
 * highlighted and scrolled into view when the picker opens.
 *
 * Usage (from any click handler):
 *
 *   QuickOptionPicker::launch(triggerComponent, itemNames, currentIndex,
 *       [this](int chosenIndex)
 *       {
 *           // chosenIndex is -1 if the user cancelled
 *           if (chosenIndex >= 0) { ... }
 *       });
 *
 * The picker owns itself via the CallOutBox it is launched in, so callers
 * don't need to keep it alive.
 */
class QuickOptionPicker : public juce::Component
{
public:
    using ChoiceCallback = std::function<void(int chosenIndex)>;

    QuickOptionPicker(juce::StringArray itemsToShow, int currentIndex,
                       ChoiceCallback callback);
    ~QuickOptionPicker() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    bool keyPressed(const juce::KeyPress& key) override;

    /** Builds and shows a QuickOptionPicker anchored to `anchor`, inside a
        juce::CallOutBox. `items` is the full readable text for every choice,
        in parameter/index order (item N corresponds to choice value N).
        `currentIndex` (may be -1) is pre-highlighted and scrolled into view.
        `callback` fires exactly once, with -1 if the user cancels. */
    static void launch(juce::Component& anchor, const juce::StringArray& items,
                        int currentIndex, ChoiceCallback callback,
                        int width = 260, int maxVisibleRows = 9);

private:
    // Text box that forwards navigation keys to the owning picker instead of
    // consuming them, so the user can keep typing while using Up/Down/Enter.
    class FilterEditor : public juce::TextEditor
    {
    public:
        explicit FilterEditor(QuickOptionPicker& o) : owner(o) {}
        bool keyPressed(const juce::KeyPress& key) override
        {
            if (key.isKeyCode(juce::KeyPress::upKey) ||
                key.isKeyCode(juce::KeyPress::downKey) ||
                key.isKeyCode(juce::KeyPress::returnKey) ||
                key.isKeyCode(juce::KeyPress::escapeKey))
                return owner.keyPressed(key);
            return juce::TextEditor::keyPressed(key);
        }

    private:
        QuickOptionPicker& owner;
    };

    class ListModel : public juce::ListBoxModel
    {
    public:
        explicit ListModel(QuickOptionPicker& o) : owner(o) {}
        int getNumRows() override { return owner.filteredIndices.size(); }
        void paintListBoxItem(int row, juce::Graphics& g, int w, int h,
                               bool rowIsSelected) override;
        void listBoxItemClicked(int row, const juce::MouseEvent&) override;
        void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;

    private:
        QuickOptionPicker& owner;
    };

    void applyFilter();
    void commitRow(int filteredRow);
    void cancel();

    juce::StringArray allItems;
    juce::Array<int> filteredIndices; // maps visible row -> index into allItems
    int currentIndex = -1;
    ChoiceCallback onChoice;
    bool choiceMade = false;

    ListModel listModel { *this };
    FilterEditor searchBox { *this };
    juce::ListBox listBox { "quickOptionList", &listModel };
    juce::Label hintLabel;
};

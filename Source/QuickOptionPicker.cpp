#include "QuickOptionPicker.h"

QuickOptionPicker::QuickOptionPicker(juce::StringArray itemsToShow,
                                     int startIndex, ChoiceCallback callback)
    : allItems(std::move(itemsToShow)), currentIndex(startIndex),
      onChoice(std::move(callback)) {
  for (int i = 0; i < allItems.size(); ++i)
    filteredIndices.add(i);

  searchBox.setTextToShowWhenEmpty("Type to filter\u2026",
                                   juce::Colours::grey);
  searchBox.setColour(juce::TextEditor::textColourId, juce::Colours::black);
  searchBox.setColour(juce::TextEditor::backgroundColourId,
                      juce::Colours::white);
  searchBox.setColour(juce::TextEditor::outlineColourId,
                      juce::Colour(0xff7f9db9));
  searchBox.setColour(juce::TextEditor::focusedOutlineColourId,
                      juce::Colour(0xff316ac5));
  searchBox.setColour(juce::TextEditor::highlightColourId,
                      juce::Colour(0xff316ac5));
  searchBox.setColour(juce::TextEditor::highlightedTextColourId,
                      juce::Colours::white);
  searchBox.setFont(juce::Font(juce::FontOptions(13.0f)));
  // Up/Down/Enter/Escape are intercepted by FilterEditor::keyPressed above
  // and forwarded straight to this picker's keyPressed(), so only the plain
  // text-changed callback is needed here.
  searchBox.onTextChange = [this] { applyFilter(); };
  addAndMakeVisible(searchBox);

  listBox.setModel(&listModel);
  listBox.setRowHeight(20);
  listBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::white);
  listBox.setColour(juce::ListBox::outlineColourId, juce::Colour(0xff7f9db9));
  listBox.setOutlineThickness(1);
  addAndMakeVisible(listBox);

  hintLabel.setText("\u2191\u2193 move   Enter select   Esc cancel",
                    juce::dontSendNotification);
  hintLabel.setFont(juce::Font(juce::FontOptions(9.5f)));
  hintLabel.setColour(juce::Label::textColourId, juce::Colour(0xff707070));
  hintLabel.setJustificationType(juce::Justification::centredLeft);
  addAndMakeVisible(hintLabel);

  applyFilter();
  if (currentIndex >= 0)
    listBox.selectRow(filteredIndices.indexOf(currentIndex));

  setWantsKeyboardFocus(true);
}

void QuickOptionPicker::resized() {
  auto b = getLocalBounds().reduced(4);
  searchBox.setBounds(b.removeFromTop(24));
  b.removeFromTop(3);
  hintLabel.setBounds(b.removeFromBottom(14));
  listBox.setBounds(b);
}

void QuickOptionPicker::paint(juce::Graphics& g) {
  g.fillAll(juce::Colours::white);
  g.setColour(juce::Colour(0xff7f9db9));
  g.drawRect(getLocalBounds(), 1);
}

void QuickOptionPicker::applyFilter() {
  auto query = searchBox.getText().trim();
  filteredIndices.clear();

  if (query.isEmpty()) {
    for (int i = 0; i < allItems.size(); ++i)
      filteredIndices.add(i);
  } else {
    for (int i = 0; i < allItems.size(); ++i)
      if (allItems[i].containsIgnoreCase(query))
        filteredIndices.add(i);
  }

  listBox.updateContent();

  int rowToSelect = filteredIndices.indexOf(currentIndex);
  if (rowToSelect < 0 && filteredIndices.size() > 0)
    rowToSelect = 0;

  if (rowToSelect >= 0) {
    listBox.selectRow(rowToSelect, false, true);
  }
}

void QuickOptionPicker::commitRow(int filteredRow) {
  if (filteredRow < 0 || filteredRow >= filteredIndices.size())
    return;

  int actualIndex = filteredIndices[filteredRow];
  choiceMade = true;
  if (onChoice)
    onChoice(actualIndex);

  if (auto* callOut = findParentComponentOfClass<juce::CallOutBox>())
    callOut->dismiss();
}

void QuickOptionPicker::cancel() {
  if (auto* callOut = findParentComponentOfClass<juce::CallOutBox>())
    callOut->dismiss();
}

bool QuickOptionPicker::keyPressed(const juce::KeyPress& key) {
  if (key.isKeyCode(juce::KeyPress::downKey)) {
    int next = juce::jlimit(0, juce::jmax(0, filteredIndices.size() - 1),
                            listBox.getSelectedRow() + 1);
    listBox.selectRow(next, false, true);
    return true;
  }
  if (key.isKeyCode(juce::KeyPress::upKey)) {
    int prev = juce::jlimit(0, juce::jmax(0, filteredIndices.size() - 1),
                            listBox.getSelectedRow() - 1);
    listBox.selectRow(prev, false, true);
    return true;
  }
  if (key.isKeyCode(juce::KeyPress::returnKey)) {
    commitRow(listBox.getSelectedRow() >= 0 ? listBox.getSelectedRow() : 0);
    return true;
  }
  if (key.isKeyCode(juce::KeyPress::escapeKey)) {
    cancel();
    return true;
  }
  return false;
}

void QuickOptionPicker::launch(juce::Component& anchor,
                               const juce::StringArray& items,
                               int currentIndex, ChoiceCallback callback,
                               int width, int maxVisibleRows) {
  if (items.isEmpty())
    return;

  auto content =
      std::make_unique<QuickOptionPicker>(items, currentIndex, callback);

  const int rowH = 20;
  int visibleRows = juce::jlimit(1, maxVisibleRows, items.size());
  int height = 24 + 3 + 14 + visibleRows * rowH + 10;
  content->setSize(width, height);

  auto* picker = content.get();
  auto& box = juce::CallOutBox::launchAsynchronously(
      std::move(content), anchor.getScreenBounds(),
      anchor.getTopLevelComponent());
  box.setDismissalMouseClicksAreAlwaysConsumed(false);

  picker->searchBox.grabKeyboardFocus();
}

QuickOptionPicker::~QuickOptionPicker() {
  // Guarantees the callback always fires exactly once: if the box was
  // dismissed by clicking outside it (rather than through commitRow()),
  // report a cancellation so callers never hang waiting for a choice.
  if (!choiceMade && onChoice)
    onChoice(-1);
}

// ---------------------------------------------------------------------
// ListModel
// ---------------------------------------------------------------------
void QuickOptionPicker::ListModel::paintListBoxItem(int row, juce::Graphics& g,
                                                     int w, int h,
                                                     bool rowIsSelected) {
  if (row < 0 || row >= owner.filteredIndices.size())
    return;

  int actualIndex = owner.filteredIndices[row];

  if (rowIsSelected)
    g.fillAll(juce::Colour(0xff316ac5));
  else if (actualIndex == owner.currentIndex)
    g.fillAll(juce::Colour(0xffe4ecf7));

  g.setColour(rowIsSelected ? juce::Colours::white : juce::Colours::black);
  g.setFont(juce::Font(juce::FontOptions(12.5f).withStyle(
      actualIndex == owner.currentIndex ? "Bold" : "Plain")));
  g.drawText(owner.allItems[actualIndex], 6, 0, w - 10, h,
             juce::Justification::centredLeft, true);
}

void QuickOptionPicker::ListModel::listBoxItemClicked(int row,
                                                       const juce::MouseEvent&) {
  owner.commitRow(row);
}

void QuickOptionPicker::ListModel::listBoxItemDoubleClicked(
    int row, const juce::MouseEvent&) {
  owner.commitRow(row);
}

/*
  ==============================================================================

    OscilloscopeDisplay.cpp
    Created: 12 Sep 2022 6:48:51pm
    Author:  pwmag

  ==============================================================================
*/

#include "OscilloscopeDisplay.h"

OscilloscopeDisplay::OscilloscopeDisplay(xynth::GuiData& g, int chNum) : guiData(g), id(chNum) {
    
    startTimerHz(60);

    setInterceptsMouseClicks(false, false);
}

void OscilloscopeDisplay::paint(juce::Graphics& g) {
    auto rect = getLocalBounds();
    //guiData.getLnf().drawGraphBackground(g, rect.toFloat(), 1);

    auto fixedBuffer = guiData.audioProcessor.getFixedBuffer(id);
    const auto oscLength = fixedBuffer->getWidth();

    juce::Path line;
    line.clear();
    line.startNewSubPath(rect.getX(), rect.getCentreY() - juce::jlimit<float>(-0.5f, 0.5f, fixedBuffer->getSample(0) * 0.5 * rect.getHeight()));
    for (int i = 1; i < oscLength; i += 10) {
        line.lineTo(rect.getX() + i * rect.getWidth() / oscLength, rect.getCentreY() - juce::jlimit<float>(-0.5f * rect.getHeight(), 0.5f * rect.getHeight(), fixedBuffer->getSample(i) * 0.5 * rect.getHeight()));
    }

    g.setColour(guiData.getTextColor());
    g.strokePath(line, juce::PathStrokeType(1.f, juce::PathStrokeType::curved, juce::PathStrokeType::EndCapStyle::rounded));
}

void OscilloscopeDisplay::timerCallback() {
    repaint();
}
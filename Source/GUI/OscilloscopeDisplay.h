/*
  ==============================================================================

    OscilloscopeDisplay.h
    Created: 12 Sep 2022 6:48:51pm
    Author:  pwmag

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GuiData.h"
#include "../Common/FixedWidthBuffer.h"

class OscilloscopeDisplay : public juce::Component, juce::Timer {
public:
    OscilloscopeDisplay(xynth::GuiData& g, int chNum);
    void paint(juce::Graphics& g);

private:
    xynth::GuiData& guiData;
    int id;

    void timerCallback() override;
};


class A {
public:
    A();
    ~A();

};
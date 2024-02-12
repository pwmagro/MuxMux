/*
  ==============================================================================

    WindowLayout.h
    Created: 28 May 2022 11:46:38am
    Author:  thesp

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GuiData.h"
#include "BlockButton.h"
#include "OscilloscopeDisplay.h"

class WindowLayout : public juce::Component
{
public:
    WindowLayout(xynth::GuiData& g);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    xynth::GuiData& guiData;

    std::unique_ptr<BlockButton> buttons[NUM_STATES][NUM_OUTPUTS];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachments[NUM_STATES][NUM_OUTPUTS];

    std::unique_ptr<juce::Slider> select;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> selectAttach;

    std::unique_ptr<juce::Label> channelLabels[NUM_OUTPUTS];

    // Contains oscilloscope and VU meter
    std::unique_ptr<OscilloscopeDisplay> oscilloscopeDisplays[NUM_OUTPUTS];
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WindowLayout)
};
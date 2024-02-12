/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/GuiData.h"
#include "GUI/WindowLayout.h"

//==============================================================================
/**
*/
class MuxMuxAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MuxMuxAudioProcessorEditor (MuxMuxAudioProcessor&);
    ~MuxMuxAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;

private:
    xynth::GuiData guiData;
    WindowLayout windowLayout;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MuxMuxAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuxMuxAudioProcessorEditor)
};

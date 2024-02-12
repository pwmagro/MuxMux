/*
  ==============================================================================

    GuiData.h
    Created: 5 Sep 2022 3:56:01pm
    Author:  thesp

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"

namespace xynth
{

struct GuiData
{
    GuiData(MuxMuxAudioProcessor& ap, juce::AudioProcessorValueTreeState& apvts) :
        audioProcessor(ap), apvts(apvts)
    {
        textColor = juce::Colour::fromRGB(0xdd, 0xd7, 0xc6);
        backgroundColor = juce::Colour::fromRGB(0x49, 0x00, 0x45);
        highlightColor = juce::Colour::fromRGB(0x00, 0xc8, 0x93);
        altColor = juce::Colour::fromRGB(0xbd, 0x72, 0xb4);
    }

    juce::Colour& getBackgroundColor() { return backgroundColor; }
    juce::Colour& getTextColor()       { return textColor; }
    juce::Colour& getHighlightColor()  { return highlightColor; }
    juce::Colour& getAltColor()        { return altColor; }

    juce::AudioProcessorValueTreeState& apvts;
    MuxMuxAudioProcessor& audioProcessor;

private:
    juce::Colour backgroundColor, textColor, highlightColor, altColor;
};
} //namespace xynth
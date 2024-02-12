/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MuxMuxAudioProcessorEditor::MuxMuxAudioProcessorEditor (MuxMuxAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), guiData(p, p.apvts), windowLayout(guiData)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (720, 360);
    addAndMakeVisible(windowLayout);
}

MuxMuxAudioProcessorEditor::~MuxMuxAudioProcessorEditor()
{
}

void MuxMuxAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    windowLayout.setBounds(getLocalBounds());
}

/*
  ==============================================================================

	WindowLayout.cpp
	Created: 28 May 2022 11:46:38am
	Author:  thesp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WindowLayout.h"

WindowLayout::WindowLayout(xynth::GuiData& g) :
	guiData(g)
{
	// not using NUM_OUTPUTS macro here in case the host didn't give us all the channels we asked for
	auto numOutputs = (g.audioProcessor.getNumOutputChannels() / 2) - 1;

	for (int selVal = 0; selVal < NUM_STATES; selVal++) {
		for (int outChannel = 0; outChannel < NUM_OUTPUTS_VARIABLE; outChannel++) {
			juce::String id, name;
			id << "SEL_" << (selVal + 1) << "_OUT_" << (outChannel + 1) << "_EN";
			buttons[selVal][outChannel] = std::make_unique<BlockButton>(id, guiData, selVal + 1);
			attachments[selVal][outChannel] = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(guiData.apvts, id, *buttons[selVal][outChannel]);
			addAndMakeVisible(buttons[selVal][outChannel].get());
		}
	}

	for (int outChannel = 0; outChannel < NUM_OUTPUTS_VARIABLE; outChannel++) {
		channelLabels[outChannel] = std::make_unique<juce::Label>();
		juce::String label;
		label << "Channel " << outChannel;
		channelLabels[outChannel]->setText(label, juce::dontSendNotification);
		channelLabels[outChannel]->setColour(juce::Label::ColourIds::textColourId, guiData.getTextColor());
		channelLabels[outChannel]->setJustificationType(juce::Justification::centredRight);
		channelLabels[outChannel]->setFont(juce::Font(14.f));
		addAndMakeVisible(channelLabels[outChannel].get());

		oscilloscopeDisplays[outChannel] = std::make_unique<OscilloscopeDisplay>(guiData, outChannel);
		addAndMakeVisible(oscilloscopeDisplays[outChannel].get());
	}

	select = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
	select->setColour(juce::Slider::backgroundColourId, guiData.getAltColor());
	select->setColour(juce::Slider::trackColourId,		juce::Colour::fromRGBA(0, 0, 0, 0));
	select->setColour(juce::Slider::thumbColourId,		guiData.getTextColor());


	select->onDragEnd = [this] {
		for (int selVal = 0; selVal < NUM_STATES; selVal++) {
			for (auto& b : buttons[selVal]) {
				b->repaint();
			}
		}
	};

	selectAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(guiData.apvts, "OUTPUT_ROUTE", *select);
	addAndMakeVisible(select.get());
}

void WindowLayout::paint(juce::Graphics& g)
{
	g.setColour(guiData.getBackgroundColor());
	g.fillAll();

}

void WindowLayout::resized()
{
	auto numOutputs = (guiData.audioProcessor.getNumOutputChannels() / 2) - 1;

	auto rect = getLocalBounds().reduced(5);
	auto subRect = rect.removeFromBottom(300);
	auto labelRect = subRect.removeFromLeft(100);
	
	auto h = subRect.proportionOfHeight(1.f / NUM_OUTPUTS_VARIABLE);
	auto w = subRect.proportionOfWidth(1.f / NUM_STATES);

	for (int outChannel = 0; outChannel < NUM_OUTPUTS_VARIABLE; outChannel++) {

		channelLabels[outChannel]->setBounds(labelRect.getX(), h * outChannel + labelRect.getY(), labelRect.getWidth(), h);
		oscilloscopeDisplays[outChannel]->setBounds(subRect.getX(), h * outChannel + subRect.getY(), subRect.getWidth(), h);

		for (int selVal = 0; selVal < NUM_STATES; selVal++) {
			juce::String id, name;
			id << "SEL_" << (selVal + 1) << "_OUT_" << (outChannel + 1) << "_EN";
			name << "Rack " << (selVal + 1) << " to ch " << (outChannel + 1);
			buttons[selVal][outChannel]->setBounds(w * selVal + subRect.getX(), h * outChannel + subRect.getY(), w, h);
		}
	}

	auto sliderRect = rect.withTrimmedLeft(100).reduced((w / 2) - 10, 0);
	select->setBounds(sliderRect);
}
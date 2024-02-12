/*
  ==============================================================================

    BlockButton.h
    Created: 11 Feb 2024 1:13:33pm
    Author:  pwmag

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GuiData.h"

class BlockButton : public juce::ToggleButton {
public:
    BlockButton(const juce::String& name, xynth::GuiData& g, int channel) :
        juce::ToggleButton(name),
        guiData(g),
        selectChannel(channel)
    {
        setToggleable(true);
        color = guiData.getTextColor().darker();

        onStateChange = [this] {

            if (getToggleState()) {
                color = guiData.getHighlightColor();
            }
            else if (isMouseOver()) {
                color = guiData.getTextColor();
            }
            else {
                color = guiData.getTextColor().darker();
            }

            this->repaint();
        };
    }
    void paint(juce::Graphics& g) {
        auto rect = getLocalBounds().reduced(0, 2.f);

        //auto trueColor = (int(guiData.apvts.getParameterAsValue("OUTPUT_ROUTE").getValue()) == selectChannel) ? color.brighter() : color.darker();
        auto trueColor = color;

        g.setColour(trueColor);
        g.fillRect(rect);
        g.setColour(trueColor.brighter());
        g.drawRect(rect, 2.f);
    }

private:
    xynth::GuiData& guiData;
    juce::Colour color;
    const int selectChannel;
};
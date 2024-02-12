/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Common/FixedWidthBuffer.h"

#define NUM_STATES      8
#define NUM_OUTPUTS     8

#ifdef DEBUG
    #define NUM_OUTPUTS_VARIABLE NUM_OUTPUTS
#else
    #define NUM_OUTPUTS_VARIABLE numOutputs
#endif

//==============================================================================
/**
*/
class MuxMuxAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    MuxMuxAudioProcessor();
    ~MuxMuxAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    WDYM::FixedWidthBuffer* getFixedBuffer(int id) { return fixedBuffers[id].get(); }

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::unique_ptr<WDYM::FixedWidthBuffer> fixedBuffers[NUM_OUTPUTS];
    juce::LinearSmoothedValue<float> selValSmoother[NUM_OUTPUTS] {};
        
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MuxMuxAudioProcessor)
    
};

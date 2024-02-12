/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MuxMuxAudioProcessor::MuxMuxAudioProcessor()
	: AudioProcessor(BusesProperties()
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
		.withOutput("Main Out", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 1", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 2", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 3", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 4", juce::AudioChannelSet::stereo(), true)
#if NUM_OUTPUTS >= 8
		.withOutput("Select 5", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 6", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 7", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 8", juce::AudioChannelSet::stereo(), true)
#if NUM_OUTPUTS == 16
		.withOutput("Select 9", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 10", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 11", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 12", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 13", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 14", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 15", juce::AudioChannelSet::stereo(), true)
		.withOutput("Select 16", juce::AudioChannelSet::stereo(), true)
#endif
#endif
	),
	apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
{
	for (int ch = 0; ch < NUM_OUTPUTS; ch++) {
		fixedBuffers[ch] = std::make_unique<WDYM::FixedWidthBuffer>();
	}
}

MuxMuxAudioProcessor::~MuxMuxAudioProcessor()
{
}

//==============================================================================
const juce::String MuxMuxAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MuxMuxAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MuxMuxAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MuxMuxAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MuxMuxAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MuxMuxAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int MuxMuxAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MuxMuxAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MuxMuxAudioProcessor::getProgramName(int index)
{
	return {};
}

void MuxMuxAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MuxMuxAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	for (auto& s : selValSmoother) {
		s.setCurrentAndTargetValue(1.f);
		s.reset(sampleRate, 0.05f);
	}
}

void MuxMuxAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

bool MuxMuxAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.

#ifdef DEBUG
	return true;
#endif

	auto numOutputs = layouts.getMainOutputChannels();
	return numOutputs >= 10;
}

void MuxMuxAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	auto chrl = buffer.getReadPointer(0);
	auto chrr = buffer.getReadPointer(1);

	auto samples = buffer.getNumSamples();

	auto numOutputs = (getTotalNumOutputChannels() / 2) - 1;

	for (auto& s : selValSmoother)
		s.setTargetValue(int(apvts.getParameterAsValue("OUTPUT_ROUTE").getValue()));

	for (int selValScan = 1; selValScan <= NUM_STATES; selValScan++) {
		for (int outChannel = 1; outChannel <= NUM_OUTPUTS_VARIABLE; outChannel++) {
			juce::String id, name;
			id << "SEL_" << selValScan << "_OUT_" << outChannel << "_EN";
			bool en = apvts.getParameter(id)->getValue();

			if (en)
			{
				auto chNum = outChannel * 2;

				auto chwl = buffer.getWritePointer(chNum);
				auto chwr = buffer.getWritePointer(chNum + 1);

				for (int s = 0; s < samples; s++) {
					// this is why we have 8 smoothers. the cache would hate us otherwise
					auto smoothSelVal = selValSmoother[outChannel-1].getNextValue();
					float gain = juce::jlimit<float>(0.f, 1.f, 1 - abs(smoothSelVal - selValScan));

					chwl[s] += chrl[s] * gain;
					chwr[s] += chrr[s] * gain;
				}
			}
		}
	}

	juce::dsp::AudioBlock<float> mainBlock(buffer);
	for (int outChannel = 1; outChannel <= NUM_OUTPUTS_VARIABLE; outChannel++) {
		auto chNum = outChannel * 2;
		auto subBlock = mainBlock.getSubsetChannelBlock(chNum, 2);
		fixedBuffers[outChannel - 1]->process(subBlock);
	}
}

//==============================================================================
bool MuxMuxAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MuxMuxAudioProcessor::createEditor()
{
	return new MuxMuxAudioProcessorEditor(*this);
	//return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void MuxMuxAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void MuxMuxAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MuxMuxAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MuxMuxAudioProcessor::createParameterLayout() {
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(std::make_unique<juce::AudioParameterInt>("OUTPUT_ROUTE", "Routing State", 1, NUM_STATES, 1));

	// not using NUM_OUTPUTS macro here in case the host didn't give us all the channels we asked for
	auto numOutputs = (getTotalNumOutputChannels() / 2) - 1;

	for (int selVal = 1; selVal <= NUM_STATES; selVal++) {
		for (int outChannel = 1; outChannel <= numOutputs; outChannel++) {
			juce::String id, name;
			id << "SEL_" << selVal << "_OUT_" << outChannel << "_EN";
			name << "Rack " << selVal << " to ch " << outChannel;
			layout.add(std::make_unique<juce::AudioParameterBool>(id, name, false));
		}
	}

	layout.add(std::make_unique<juce::AudioParameterFloat>("SMOOTH_VAL", "Smoothing Constant", 0.01f, 1.f, 0.9f));

	return layout;
}
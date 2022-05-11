/*
  ==============================================================================

    PluginProcessor.h 
    Class derived from AudioProcessor class. It handles the audio processing.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"
#include "Data/FilterData.h"


class SubSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SubSynthAudioProcessor();
    ~SubSynthAudioProcessor() override;

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

    
    //AudioProcessorValueTreeState object for implementing the communication between Editor and Processor.
    //It is used to manage an AudioProcessor's entire state.
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioVisualiserComponent waveViewer; //wave viewer
    
private:

    juce::Synthesiser synth; //Defining the synthesiser
    juce::AudioProcessorValueTreeState::ParameterLayout createParams(); //function used to add parameters to the AudioProcessorValueTreeState

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubSynthAudioProcessor)
};

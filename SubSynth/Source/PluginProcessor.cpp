/*
  ==============================================================================

    PluginProcessor.cpp

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSynthAudioProcessor::SubSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), 
                       //initialization of the value tree state using the function createParams()
                       apvts(*this, nullptr, "Parameters", createParams()),
                       //initialization of the waveViewer
                       waveViewer(1)
#endif
{
    synth.addSound(new SynthSound()); //adding syhtnsound to the synthesiser
    for (auto i = 0; i < 4; ++i) //adding 4 synthvoices to the synthesiser
        synth.addVoice(new SynthVoice());
    waveViewer.setRepaintRate(30);
    waveViewer.setBufferSize(128);
}

SubSynthAudioProcessor::~SubSynthAudioProcessor()
{
}

//==============================================================================
const juce::String SubSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SubSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SubSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SubSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SubSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SubSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SubSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SubSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SubSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SubSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SubSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate); //setting sample rate on the synth

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels()); // Setting up all voices
        }
    }
    waveViewer.clear();
}

void SubSynthAudioProcessor::releaseResources()
{
    waveViewer.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SubSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//Reders the next block
void SubSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //for each voice we get the parameters from the value tree state
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");

            auto& oscWaveChoice = *apvts.getRawParameterValue("OSCWAVETYPE");
          
            // Applying Filter
            auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
            auto& filterCutOff = *apvts.getRawParameterValue("FILTERCUTOFF");
            auto& filterRes = *apvts.getRawParameterValue("FILTERRES");

            auto& filterAttack = *apvts.getRawParameterValue("FILTERATTACK");
            auto& filterDecay = *apvts.getRawParameterValue("FILTERDECAY");
            auto& filterSustain = *apvts.getRawParameterValue("FILTERSUSTAIN");
            auto& filterRelease = *apvts.getRawParameterValue("FILTERRELEASE");

            //setting of the parameters of the oscillator, filter and envelope according to the values retrieved
            voice->updateAdsr(attack, decay, sustain, release);
            voice->getOscillator().setWaveType(oscWaveChoice);
            voice->updateFilter(filterType, filterCutOff, filterRes);
            voice->updateFilterAdsr(filterAttack, filterDecay, filterSustain, filterRelease);
        }
    }
    
    //Creates the next block of audio output
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    //wave viewer
    waveViewer.pushBuffer(buffer);
}

//==============================================================================
bool SubSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SubSynthAudioProcessor::createEditor()
{
    return new SubSynthAudioProcessorEditor (*this);
}

//==============================================================================
void SubSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SubSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubSynthAudioProcessor();
}

//function createParams() that adds parameters to the value tree state
juce::AudioProcessorValueTreeState::ParameterLayout SubSynthAudioProcessor::createParams()
{
    //vector of unique pointers to objects of classes RangedAudioParameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params; 

    // ADSR 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.9f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.2f));

    // Filter ADSR 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERATTACK", "Filter Attack", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERDECAY", "Filter Decay", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERSUSTAIN", "Filter Sustain", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.9f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRELEASE", "Filter Release", juce::NormalisableRange<float> {0.1f, 3.0f, 0.1f}, 0.4f));

    //OSC type
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSCWAVETYPE", "Osc Wave Type", juce::StringArray{ "Sine", "Sawtooth", "Square", "Triangle", "White Noise" }, 0));

    // Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{ "Lowpass", "Bandpass", "Highpass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float> {20.0f, 20000.0f, 1.0f, 0.6f}, 15000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float> {1.0f, 10.0f, 0.1f}, 1.0f));
   
    return { params.begin(), params.end() };
}

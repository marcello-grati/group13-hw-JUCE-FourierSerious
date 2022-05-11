/*
  ==============================================================================

    FilterData.cpp

  ==============================================================================
*/

#include "FilterData.h"

//Initialises the filter
void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
    filter.reset();

    //structure that contains information about various aspects of the context
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;    
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;

    filter.prepare(spec); 

    isPrepared = true;
}

//Processes the input and output samples supplied in the processing context
void FilterData::process(juce::AudioBuffer<float>& buffer) 
{
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> block{ buffer };

    filter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//setting of the parameters of the filter
void FilterData::updateParameters(const int filterType, const float frequency, const float resonance, const float modulator)
{
    //choosing the type of filter
    switch (filterType)
    {
        case 0: //lowpass filter
            filter.setType (juce::dsp::StateVariableTPTFilterType::lowpass);
            break;
        case 1: //bandpass filter
            filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            break;
        case 2: //highpass filter
            filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            break;
    }
    
    //setting the cutoff frequency and the resonance of the filter
    float modFreq = frequency * modulator;
    modFreq = std::fmax(20.0f, modFreq);
    modFreq = std::fmin(20000.0f, modFreq);
    filter.setCutoffFrequency(modFreq);
    filter.setResonance(resonance);
}

void FilterData::reset()
{
    filter.reset();
}

/*
  ==============================================================================

    FilterData.h
    Filter class.
    The filter is an IIR filter that can perform low, band and high-pass 
    filtering on an audio signal, with 12 dB of attenuation per octave.
    
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class FilterData
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const int filterType, const float frequency, const float resonance, const float modulator = 1.0f);
    void reset();

private:
    //state variable filter
    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared = false;
};

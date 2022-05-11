/*
  ==============================================================================

    SynthVoice.cpp

  ==============================================================================
*/

#include "SynthVoice.h"


//return true if the voice can play a sound
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) 
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

//function called by the synthesiser when a voice is started
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
     //set the frequency of the wave according to the MIDI note
    osc.setWaveFrequency(midiNoteNumber);
    //starts the attack phase of the envelope of the amplitude and of the filter
    adsr.noteOn(); 
    filterAdsr.noteOn();
}

//function called to stop a note
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    //starts the release phase of the envelope of the amplitude and of the filter
    adsr.noteOff();
    filterAdsr.noteOff();
    
    //if allowTailOff is false or the adsr is not active it stops the sound immediately
    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

//Renders the next block of data for this voice. 
void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    
    //If the voice is currently silent, it just returns without doing anything.
    if (!isVoiceActive())
        return;

    //setting the audio buffer
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    filterAdsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);
    synthBuffer.clear();
    
    //creating the AudioBlock from the audio buffer
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    //oscillator
    osc.getNextAudioBlock(audioBlock);
    //envelope
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    //filter
    filter.process(synthBuffer);
    //gain
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));


    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        //The output audio data is added to the current contents of the buffer provided
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        //If the sound that the voice is playing finishes during the course of this rendered block, 
        //clearCurrentNote() is called to tell the synthesiser that it has finished
        if (!adsr.isActive())
            clearCurrentNote();
    }
}

//setting of the parameters of a voice
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    //structure that contains information about various aspects of the context
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    //setting of the parameters
    osc.prepareToPlay(spec);
    adsr.setSampleRate(sampleRate);
    filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
    filterAdsr.setSampleRate(sampleRate);
    gain.prepare(spec);
    gain.setGainLinear(0.2f);    

    isPrepared = true;
}

//function that updates the parameters of the filter
void SynthVoice::updateFilter(const int filterType, const float cutoff, const float resonance)
{
    float modulator = filterAdsr.getNextSample();
    filter.updateParameters(filterType, cutoff, resonance, modulator);
}

//function that sets the values of attack, decay, sustain and release of the amplitude envelope
void SynthVoice::updateAdsr(const float attack, const float decay, const float sustain, const float release)
{
    adsr.updateADSR(attack, decay, sustain, release);
}

//function that sets the value of attack, decay, sustain and release of the filter envelope
void SynthVoice::updateFilterAdsr(const float attack, const float decay, const float sustain, const float release)
{
    filterAdsr.updateADSR(attack, decay, sustain, release);
}

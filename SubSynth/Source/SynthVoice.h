/*
  ==============================================================================

    SynthVoice.h
    Represents a voice that a Synthesiser can use to play a SynthesiserSound.
    It maintains the state of one of the voices of the synthesiser.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/AdsrData.h"
#include "Data/OscData.h"
#include "Data/FilterData.h"

class SynthVoice : public juce::SynthesiserVoice 
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay(double sampleRate, int SamplesPerBlock, int outputChannels);

    void updateAdsr(const float attack, const float decay, const float sustain, const float release);
    void updateFilter(const int filterType, const float cutoff, const float resonance);
    void updateFilterAdsr(const float attack, const float decay, const float sustain, const float release);

    OscData& getOscillator() { return osc; } //returns the oscillator

private:
    //data members
    juce::AudioBuffer<float> synthBuffer; //audio buffer
    OscData osc; //oscillator
    AdsrData adsr; //ADSR envelope
    FilterData filter; //filter
    AdsrData filterAdsr; //ADSR envelope applied to the filter
    juce::dsp::Gain<float> gain; //gain 
    bool isPrepared = false;
};

/*
  ==============================================================================

    AdsrData.h
    ADSR envelope class

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class AdsrData : public juce::ADSR {
public:

    void updateADSR(const float attack, const float decay, const float sustain, const float release);

private:
    juce::ADSR::Parameters adsrParameters; //parameters of the ADSR envelope
};

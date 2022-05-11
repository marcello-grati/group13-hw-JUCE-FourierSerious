/*
  ==============================================================================

    AdsrData.cpp

  ==============================================================================
*/

#include "AdsrData.h"

//function that sets the parameters of the ADSR envelope 
void AdsrData::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    adsrParameters.attack = attack;
    adsrParameters.decay = decay;
    adsrParameters.sustain = sustain;
    adsrParameters.release = release;

    setParameters(adsrParameters);
}

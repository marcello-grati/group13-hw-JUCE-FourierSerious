/*
  ==============================================================================

    SynthSound.h
    This class describes one of the sounds that a Synthesiser can play and 
    can be used to store data that is needed to create the sound. 
    A synthesiser can contain one or more sounds, and a sound can choose 
    which midi notes and channels can trigger it.
    In our case, it just returns true for both the appliesToNote() 
    and appliesToChannel() functions.    
    The actual audio rendering for a sound is done by the SynthesiserVoice class. 
    This allows more than one SynthesiserVoice to play the same sound at the same time.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};

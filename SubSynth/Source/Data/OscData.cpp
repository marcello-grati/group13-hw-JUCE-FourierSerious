/*
  ==============================================================================

    OscData.cpp

  ==============================================================================
*/

#include "OscData.h"

//function called before processing starts
void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    prepare(spec);
}

//function that allows to choose the type of waveform
void OscData::setWaveType(const int choice)
{
    switch (choice)
    {
    case 0: //sine wave
        initialise([](float x) {return std::sin(x); }); 
        break;
    case 1: // sawtooth wave
        initialise([](float x) {return x / juce::MathConstants<float>::pi; });
        break;
    case 2: //square wave
        initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; });
        break;
    case 3: //triangle wave
        initialise([](float x) {return (2 / juce::MathConstants<double>::pi) * asin(sin(x));});
        break;
    case 4: //white noise
        initialise([](float x) {return juce::Random().nextFloat()*0.25-0.125;});
        break;
    default:
        jassertfalse;
        break;
    }
}

//setting of the frequency according to the MIDI note
void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

//processes the input and output buffers supplied in the processing context
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

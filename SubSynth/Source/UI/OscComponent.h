/*
  ==============================================================================

    OscComponent.h
    Class that handles the GUI component of the oscillator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    //selector of the type of wave
    juce::ComboBox oscWaveSelector;
    //attachment
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
    //label
    juce::Label waveSelectorLabel{ "Choose wave type:", "Choose wave type:" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};

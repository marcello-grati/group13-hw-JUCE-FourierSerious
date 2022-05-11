/*
  ==============================================================================

    OscComponent.cpp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
{
    //selection of the type of waveform
    juce::StringArray choices{ "Sine", "Sawtooth", "Square", "Triangle", "White Noise" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);

    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    waveSelectorLabel.setFont(15.0f);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(waveSelectorLabel);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f, 1));
    g.drawText("Wave Type", labelSpace.withX(5), juce::Justification::left);
    g.setColour(juce::Colour(0xff121719));
    g.fillRoundedRectangle(bounds.toFloat(), 5.0f);
}

void OscComponent::resized()
{
    const auto startY = 40;
    const auto labelHeigth = 20;

    oscWaveSelector.setBounds(10, startY + 20, 160, 30);
    waveSelectorLabel.setBounds(10, startY, 160, labelHeigth);
}

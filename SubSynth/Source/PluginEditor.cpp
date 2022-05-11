/*
  ==============================================================================

    PluginEditor.cpp

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSynthAudioProcessorEditor::SubSynthAudioProcessorEditor (SubSynthAudioProcessor& p)
    : AudioProcessorEditor (&p),
    //initialization of the data members of the Editor 
    audioProcessor (p), 
    osc (audioProcessor.apvts, "OSCWAVETYPE"),
    adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"), 
    filter (audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES"),
    filterAdsr ("Filter Envelope", audioProcessor.apvts, "FILTERATTACK", "FILTERDECAY", "FILTERSUSTAIN", "FILTERRELEASE")
{
    setSize (670, 600);
    
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::rebeccapurple);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::lightgrey);
    addAndMakeVisible(osc);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::rebeccapurple);
    addAndMakeVisible(osc);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::mediumpurple);
    addAndMakeVisible(osc);
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colours::rebeccapurple);
    
    //show the components
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filterAdsr);
    addAndMakeVisible(filter);
    
    //wave viewer
    audioProcessor.waveViewer.setColours(juce::Colour(0xff121719), juce::Colours::rebeccapurple);
    addAndMakeVisible(audioProcessor.waveViewer);
}

SubSynthAudioProcessorEditor::~SubSynthAudioProcessorEditor()
{
}

//==============================================================================
void SubSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void SubSynthAudioProcessorEditor::resized()
{
    const auto padX = 5;
    const auto padY = 30;
    const auto block_height = 180;
    const auto block_width = 250;

    osc.setBounds(padX, padY, block_width, block_height);
 
    adsr.setBounds(osc.getRight() , padY , getRight() - osc.getRight() - padX, block_height);

    filter.setBounds(padX, block_height + 2 * padY - 20, block_width, block_height);
  
    filterAdsr.setBounds(filter.getRight(), block_height + 2 * padY - 20, getRight() - filter.getRight() - padX, block_height);
    
    audioProcessor.waveViewer.setBounds(3* padX, 2 * block_height + 3 * padY - 20, getLocalBounds().getRight() - 6 * padX, block_height);
}

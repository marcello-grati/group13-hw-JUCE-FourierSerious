/*
  ==============================================================================

    FilterComponent.cpp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterCutoffId, juce::String filterResId)
{
    //selection of the type of filter
    juce::StringArray choices{ "Lowpass", "Bandpass", "Highpass" };
    filterTypeSel.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSel);

    filterTypeSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorId, filterTypeSel);

    filterSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterSelectorLabel.setFont(15.0f);
    filterSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(filterSelectorLabel);

    //setting of the sliders
    setLinearSliderWithLabel(filterCutoffSlider, filterCutoffLabel, apvts, filterCutoffId, filterCutoffAttachment);
    setLinearSliderWithLabel(filterResSlider, filterResLabel, apvts, filterResId, filterResAttachment);
    
    filterCutoffSlider.setTextValueSuffix(" Hz");
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f, 1));
    g.drawText("Filter", labelSpace.withX(5), juce::Justification::left);
    g.setColour(juce::Colour(0xff121719));
    g.fillRoundedRectangle(bounds.toFloat(), 5.0f);
}

void FilterComponent::resized()
{
    const auto startY = 40;
    const auto sliderWidth = 220;
    const auto sliderHeight = 30;
    const auto labelYOffset = 10;
    const auto labelHeigth = 10;

    filterTypeSel.setBounds(10, startY, 90, 20);
    
    filterCutoffSlider.setBounds(10, startY + 20 + 30, sliderWidth, sliderHeight);
    filterCutoffLabel.setBounds(filterCutoffSlider.getX(), filterCutoffSlider.getY() - labelYOffset, 150, labelHeigth);

    filterResSlider.setBounds(10, startY + 70 + sliderHeight, sliderWidth, sliderHeight);
    filterResLabel.setBounds(filterResSlider.getX(), filterResSlider.getY() - labelYOffset, 150, labelHeigth);
}

void FilterComponent::setLinearSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 20);
    addAndMakeVisible(slider);
 
    //slider attachment
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramId, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(10.0f);
    label.setJustificationType(juce::Justification::left);
    addAndMakeVisible(label);
}

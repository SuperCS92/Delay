/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayKAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DelayKAudioProcessorEditor (DelayKAudioProcessor&);
    ~DelayKAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    
    Slider mDryWetSlider;
    Slider mFeedbackSlider;
    Slider mDelayTimeSlider;
    
    Label mDryWetLabel;
    Label mFeedbackLabel;
    Label mDelayTimeLabel;
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayKAudioProcessor& processor;
    
    
public:
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> DryWetValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> FeedbackValue;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayKAudioProcessorEditor)
};

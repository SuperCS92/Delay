/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayKAudioProcessorEditor::DelayKAudioProcessorEditor (DelayKAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    setSize (300, 200);
    
    auto& params = processor.getParameters();
    
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*) processor.treeState.getParameter(DRYWET_ID);
    
    mDryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end);
    mDryWetSlider.setValue(*dryWetParameter);
    addAndMakeVisible(mDryWetSlider);
    mDryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 50);
    mDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = mDryWetSlider.getValue();};
    //mDryWetSlider.onDragStart = [dryWetParameter] { dryWetParameter->beginChangeGesture(); };
    //mDryWetSlider.onDragEnd = [dryWetParameter] { dryWetParameter->endChangeGesture(); };
    mDryWetSlider.setNumDecimalPlacesToDisplay(2);
    
    mDryWetLabel.attachToComponent(&mDryWetSlider, false);
    mDryWetLabel.setText("   Dry/Wet", dontSendNotification);
    
    
    AudioParameterFloat* feedbackParameter = (AudioParameterFloat*) processor.treeState.getParameter(FEEDBACK_ID);
    
    mFeedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackSlider.setRange(Decibels::gainToDecibels(feedbackParameter->range.start), Decibels::gainToDecibels(feedbackParameter->range.end));
    mFeedbackSlider.setValue(*feedbackParameter);
    addAndMakeVisible(mFeedbackSlider);
    mFeedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 50);
    mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = Decibels::decibelsToGain( mFeedbackSlider.getValue());};
    //mFeedbackSlider.onDragStart = [feedbackParameter] { feedbackParameter->beginChangeGesture(); };
    //mFeedbackSlider.onDragEnd = [feedbackParameter] { feedbackParameter->endChangeGesture(); };
    mFeedbackSlider.setNumDecimalPlacesToDisplay(1);
    mFeedbackSlider.setTextValueSuffix("DBs");
    mFeedbackSlider.setSkewFactorFromMidPoint(-16.0);
    
    mFeedbackLabel.attachToComponent(&mFeedbackSlider, false);
    mFeedbackLabel.setText("   Feedback", dontSendNotification);
    
    AudioParameterFloat* delayTimeParameter = (AudioParameterFloat*) params.getUnchecked(2);
    
    mDelayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDelayTimeSlider.setRange(delayTimeParameter->range.start, delayTimeParameter->range.end);
    mDelayTimeSlider.setValue(*delayTimeParameter);
    addAndMakeVisible(mDelayTimeSlider);
    mDelayTimeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 50);
    mDelayTimeSlider.onValueChange = [this, delayTimeParameter] { *delayTimeParameter = mDelayTimeSlider.getValue();};
    mDelayTimeSlider.onDragStart = [delayTimeParameter] { delayTimeParameter->beginChangeGesture(); };
    mDelayTimeSlider.onDragEnd = [delayTimeParameter] { delayTimeParameter->endChangeGesture(); };
    mDelayTimeSlider.setNumDecimalPlacesToDisplay(1);
    mDelayTimeSlider.setTextValueSuffix("Seg");
    
    mDelayTimeLabel.attachToComponent(&mDelayTimeSlider, false);
    mDelayTimeLabel.setText("      Delay", dontSendNotification);
    
    
    DryWetValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (processor.treeState, DRYWET_ID, mDryWetSlider);
    FeedbackValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (processor.treeState, FEEDBACK_ID, mFeedbackSlider);
    

}

DelayKAudioProcessorEditor::~DelayKAudioProcessorEditor()
{
}

//==============================================================================
void DelayKAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
   
}

void DelayKAudioProcessorEditor::resized()
{
    Rectangle<int> area = getLocalBounds();
    
    Rectangle<int> DryWetBox = area.removeFromLeft(100);
    Rectangle<int> FeedbackBox = area.removeFromLeft(100);
    Rectangle<int> DelayTimeBox = area.removeFromLeft(100);
    
    mDryWetLabel.setBounds(DryWetBox.removeFromTop(30));
    mDryWetSlider.setBounds(DryWetBox);
    
    mFeedbackLabel.setBounds(FeedbackBox.removeFromTop(30));
    mFeedbackSlider.setBounds(FeedbackBox);
    
    mDelayTimeLabel.setBounds(DelayTimeBox.removeFromTop(30));
    mDelayTimeSlider.setBounds(DelayTimeBox.removeFromLeft(100));
}

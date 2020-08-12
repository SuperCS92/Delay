/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define MAX_DELAY_TIME 2

//==============================================================================
/**
*/
class DelayKAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayKAudioProcessor();
    ~DelayKAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    float lin_interp (float sample_x, float sample_x1, float inPhase);

private:
    
    AudioParameterFloat* mDryWetParameter;
    AudioParameterFloat* mFeedbackParameter;
    AudioParameterFloat* mDelayTimeParameter;
    
    float mDelayTimeSmothed{0};
    
    float mDryWet{0};
    
    float mFeedbackLeft{0};
    float mFeedbackRight{0};
    
    int mCircularBufferLength{0};
    int mCircularBufferWriteHead{0};
    
    float mCircularBufferReadHead{0};
    float mDelayTimeInSamples{0};
    
    std::unique_ptr<float[]> mCircularBufferLeft;
    std::unique_ptr<float []> mCircularBufferRight;

    //float* mCircularBufferLeft;
    //float* mCircularBufferRight;

    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayKAudioProcessor)
};

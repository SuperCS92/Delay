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
    void updateTimeDelay ();
    void writeInTheBuffer(float* left, float* right, int i);
    void updateReadHead();
    void writeDelayedSample(AudioBuffer<float>& buffer, int i, float& left, float& right);
    void updateFeedbackSample(const float& left,const float& right);

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
    
    int readHead_x{0};
    int readHead_x1{0};
    float readHeadFloat{0.0f};

    float delay_sample_left {0};
    float delay_sample_right {0};
    
    //float* mCircularBufferLeft;
    //float* mCircularBufferRight;

    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayKAudioProcessor)
};

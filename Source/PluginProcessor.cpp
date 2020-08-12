/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayKAudioProcessor::DelayKAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    
    addParameter(mDryWetParameter = new  AudioParameterFloat("drywet",
                                                             "Dry Wet",
                                                             0.0,
                                                             1.0,
                                                             0.5));
    
    addParameter(mFeedbackParameter = new  AudioParameterFloat("feedback",
                                                             "Feedback",
                                                             0.0,
                                                             0.98,
                                                             0.5));
    
    addParameter(mDelayTimeParameter = new  AudioParameterFloat("delaytime",
                                                             "Delay Time",
                                                             0.01,
                                                             MAX_DELAY_TIME,
                                                             0.5));
    
    
}

DelayKAudioProcessor::~DelayKAudioProcessor()
{
}

//==============================================================================
const String DelayKAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayKAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayKAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayKAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayKAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayKAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayKAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayKAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayKAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayKAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayKAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mDelayTimeInSamples = sampleRate * *mDelayTimeParameter;
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;

    mCircularBufferLeft.reset(new float [mCircularBufferLength]);
    mCircularBufferRight.reset(new float [mCircularBufferLength]);
    
    //zeromem(mCircularBufferLeft, mCircularBufferLength * sizeof(float));
    //zeromem(&mCircularBufferRight, mCircularBufferLength * sizeof(float));

    
    mCircularBufferWriteHead = 0;
    mCircularBufferReadHead = 0;
    
    mDryWet = 0.5;
    
    mDelayTimeSmothed = *mDelayTimeParameter;

}

void DelayKAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayKAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayKAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    mDelayTimeInSamples = getSampleRate() * *mDelayTimeParameter;

   
    float* leftChannel = buffer.getWritePointer(0);
    float* RightChannel = buffer.getWritePointer(1);
    
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        mDelayTimeSmothed = mDelayTimeSmothed - 0.001 * (mDelayTimeSmothed - *mDelayTimeParameter);
        mDelayTimeInSamples = getSampleRate() * mDelayTimeSmothed;
        
        
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = RightChannel[i] + mFeedbackRight;
        
        mCircularBufferReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
        
        if (mCircularBufferReadHead < 0) {
            mCircularBufferReadHead += mCircularBufferLength;
        }
        
        int readHead_x = (int) mCircularBufferReadHead;
        int readHead_x1 = readHead_x + 1;
        
        float readHeadFloat = mCircularBufferReadHead - readHead_x;
        
        if (readHead_x1 >= mCircularBufferLength) {
            readHead_x1 -= mCircularBufferLength;
        }
        
        
        
        float delay_sample_left = lin_interp(mCircularBufferLeft[readHead_x], mCircularBufferLeft[readHead_x1], readHeadFloat);
        float delay_sample_right = lin_interp(mCircularBufferRight[readHead_x], mCircularBufferRight[readHead_x1], readHeadFloat);
        
        mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
        mFeedbackRight = delay_sample_right * *mFeedbackParameter;

        
        
        buffer.setSample(0, i, buffer.getSample(0, i) * (1 - *mDryWetParameter)  + delay_sample_left * *mDryWetParameter );
        buffer.setSample(1, i, buffer.getSample(1, i) * (1 - *mDryWetParameter) + delay_sample_right * *mDryWetParameter);

        mCircularBufferWriteHead++;
        
        if (mCircularBufferWriteHead >= mCircularBufferLength) {
            mCircularBufferWriteHead = 0;
        }
    }
    
    
    
}

//==============================================================================
bool DelayKAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayKAudioProcessor::createEditor()
{
    return new DelayKAudioProcessorEditor (*this);
}

//==============================================================================
void DelayKAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayKAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayKAudioProcessor();
}


float DelayKAudioProcessor::lin_interp (float sample_x, float sample_x1, float inPhase)
{
    return (1 - inPhase) * sample_x + inPhase * sample_x1;
}

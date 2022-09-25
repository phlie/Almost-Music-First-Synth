/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SamplerVoice.h"
#include "SamplerSound.h"

//==============================================================================
/**
*/
class AMFSAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    AMFSAudioProcessor();
    ~AMFSAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Used to connect the front end knobs to the backend
    juce::AudioProcessorValueTreeState apvts;

    // Used to load the file async
    void loadFile();
    void playFile();
    void stopFile();
private:
    // Create a Parameter Layout
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();

    // Create a synth of class Synthesiser.
    juce::Synthesiser synthGranular;

    // The total number of voices for the synth is 5
    const int numVoices{ 7 };

    // Used to choose which file to open
    std::unique_ptr<juce::FileChooser> chooser;

    // Describes the various formats available.
    juce::AudioFormatManager formatManager;
    // Stores the audio data after it has been read
    juce::AudioFormatReader* reader = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMFSAudioProcessor)
};

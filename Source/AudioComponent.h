/*
  ==============================================================================

    AudioComponent.h
    Created: 19 Sep 2022 6:02:16pm
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class AudioComponent  : public juce::Component
{
public:
    AudioComponent(AMFSAudioProcessor& audioProcessor);
    ~AudioComponent() override;

    /*void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;*/

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AMFSAudioProcessor& ap;

    using Attachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    void openButtonClicked();
    void playButtonClicked();

    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    std::unique_ptr<Attachment> openButtonAttachment;
    std::unique_ptr<Attachment> playButtonAttachment;
    std::unique_ptr<Attachment> stopButtonAttachment;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioComponent)
};

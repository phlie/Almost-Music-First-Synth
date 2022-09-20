/*
  ==============================================================================

    AudioComponent.cpp
    Created: 19 Sep 2022 6:02:16pm
    Author:  phlie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioComponent.h"

//==============================================================================
AudioComponent::AudioComponent(juce::AudioProcessorValueTreeState& apvts)
{
    openButton.setButtonText("Open!");
    openButtonAttachment = std::make_unique<Attachment>(apvts, "OPEN", openButton);
    addAndMakeVisible(openButton);

    playButton.setButtonText("Play.");
    playButtonAttachment = std::make_unique<Attachment>(apvts, "PLAY", playButton);
    addAndMakeVisible(playButton);

    stopButton.setButtonText("Stop.");
    stopButtonAttachment = std::make_unique<Attachment>(apvts, "STOP", stopButton);
    addAndMakeVisible(stopButton);
}

AudioComponent::~AudioComponent()
{
}

/* void AudioComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    /*if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);
}

void AudioComponent::releaseResources()
{
} */

void AudioComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("AudioComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AudioComponent::resized()
{
    //openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 30, getWidth() - 20, 20);
    stopButton.setBounds(10, 50, getWidth() - 20, 20);
}

void AudioComponent::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
        juce::File{},
        "*.wav");

    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file != juce::File{})
            {
                auto* reader = formatManager.createReaderFor(file);

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                    //playButton.setEnabled(true);
                    readerSource.reset(newSource.release());

                    
                }
            }
        });
}

void AudioComponent::playButtonClicked()
{
    transportSource.start();
}
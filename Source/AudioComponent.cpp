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
AudioComponent::AudioComponent(AMFSAudioProcessor& audioProcessor) : ap(audioProcessor) // Set this components reference, ap, to audioProcessor.
{
    // The three callbacks for loading, playing, and stopping a file
    openButton.onClick = [&]() { ap.loadFile(); };
    playButton.onClick = [&]() { ap.playFile(); };
    stopButton.onClick = [&]() { ap.stopFile(); };

    // Standard Connections between the Buttons and the AudioProcessor
    openButton.setButtonText("Open!");
    openButtonAttachment = std::make_unique<BA>(ap.apvts, "OPEN", openButton);
    addAndMakeVisible(openButton);

    playButton.setButtonText("Play.");
    playButtonAttachment = std::make_unique<BA>(ap.apvts, "PLAY", playButton);
    addAndMakeVisible(playButton);

    stopButton.setButtonText("Stop.");
    stopButtonAttachment = std::make_unique<BA>(ap.apvts, "STOP", stopButton);
    addAndMakeVisible(stopButton);

    numSlicesSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    numSlicesSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    numSlicesAttachment = std::make_unique<SA>(ap.apvts, "NUM SLICES", numSlicesSlider);
    addAndMakeVisible(numSlicesSlider);

    playLengthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    playLengthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    playLengthAttachment = std::make_unique<SA>(ap.apvts, "PLAY LENGTH", playLengthSlider);
    addAndMakeVisible(playLengthSlider);

    playbackTimeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    playbackTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    playbackTimeAttachment = std::make_unique<SA>(ap.apvts, "PLAYBACK TIME", playbackTimeSlider);
    addAndMakeVisible(playbackTimeSlider);
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
    // The three buttons with the same width and height spaced evenly
    const auto widthIndent = 10;
    const auto height = 50;
    openButton.setBounds(widthIndent, 10, getWidth() - widthIndent * 2, height);
    playButton.setBounds(widthIndent, openButton.getBottom() + 10, getWidth() - widthIndent * 2, height);
    stopButton.setBounds(widthIndent, playButton.getBottom() + 10, getWidth() - widthIndent * 2, height);

    numSlicesSlider.setBounds(widthIndent, stopButton.getBottom() + 20, 125, 125);
    playLengthSlider.setBounds(numSlicesSlider.getRight(), stopButton.getBottom() + 20, 125, 125);
    playbackTimeSlider.setBounds(playLengthSlider.getRight(), stopButton.getBottom() + 20, 125, 125);
}

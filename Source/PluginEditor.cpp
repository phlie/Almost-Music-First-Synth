/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AMFSAudioProcessorEditor::AMFSAudioProcessorEditor (AMFSAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), audioComponent(audioProcessor) // All components while need the audio processor reference.
{
    addAndMakeVisible(audioComponent);
    setSize (400, 300);
}

AMFSAudioProcessorEditor::~AMFSAudioProcessorEditor()
{
}

//==============================================================================
void AMFSAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void AMFSAudioProcessorEditor::resized()
{
    // Give the load button some space.
    //loadButton.setBounds(0, 0, getWidth() - 20, 20);

    // Fill the screen with the audio component.
    audioComponent.setBounds(0, 0, getWidth(), getHeight());
}



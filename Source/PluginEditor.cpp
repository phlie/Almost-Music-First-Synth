/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AMFSAudioProcessorEditor::AMFSAudioProcessorEditor (AMFSAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), audioComponent(audioProcessor.apvts)
{
    // Enable the load button
    loadButton.setButtonText("LOADme.");
    loadButton.onClick = [&]() { audioProcessor.loadFile(); };
    addAndMakeVisible(loadButton);

    // Temporarily no bounds are set
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
    loadButton.setBounds(0, 0, getWidth() - 20, 20);
    //audioComponent.setBounds(0, 0, getWidth(), getHeight());
}



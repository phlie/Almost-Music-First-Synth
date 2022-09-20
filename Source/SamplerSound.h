/*
  ==============================================================================

    SamplerSound.h
    Created: 20 Sep 2022 2:42:08am
    Author:  phlie

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SamplerSound : public::juce::SamplerSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; };
    bool appliesToChannel(int midiChannel) override { return true; };
};
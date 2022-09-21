/*
  ==============================================================================

    SamplerSound.h
    Created: 20 Sep 2022 2:42:08am
    Author:  phlie

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class SamplerSound : public juce::SamplerSound
{
public:
    /*SamplerSound(const juce::String& soundName,
        juce::AudioFormatReader& source,
        const juce::BigInteger& notes,
        int midiNoteForNormalPitch,
        double attackTimeSecs,
        double releaseTimeSecs,
        double maxSampleLengthSeconds) {
        juce::SamplerSound::SamplerSound(soundName, source, notes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds);
    }; */
    bool appliesToNote(int midiNoteNumber) override { return true; };
    bool appliesToChannel(int midiChannel) override { return true; };

    std::unique_ptr<juce::AudioBuffer<float>> data;
private:
    friend class SamplerVoice;

    juce::String name;
    double sourceSampleRate;
    juce::BigInteger midiNotes;
    int length = 0, midiRootNote = 0;
    juce::ADSR::Parameters params;
};
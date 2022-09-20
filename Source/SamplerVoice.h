/*
  ==============================================================================

    SamplerVoice.h
    Created: 20 Sep 2022 2:41:50am
    Author:  phlie

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/*
  ==============================================================================

    SamplerVoice.cpp
    Created: 29 Aug 2022 7:48:17pm
    Author:  phlie

  ==============================================================================
*/

#include "SamplerVoice.h"

class SamplerVoice : public juce::SamplerVoice
{
public:
    SamplerVoice();
    ~SamplerVoice() override;

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannel);

    void update(const float attack, const float decay, const float sustain, const float release);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
};


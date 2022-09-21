/*
  ==============================================================================

    SamplerVoice.h
    Created: 20 Sep 2022 2:41:50am
    Author:  phlie

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SamplerSound.h"

/*
  ==============================================================================

    SamplerVoice.cpp
    Created: 29 Aug 2022 7:48:17pm
    Author:  phlie

  ==============================================================================
*/


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

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    using juce::SynthesiserVoice::renderNextBlock;

    void setKnobParams(int ns, float pl);
private:
    juce::AudioSampleBuffer* buffer;
    int currentStartPosInBuffer = 0;
    int lengthOfBuffer = 0;
    bool alreadyLoadedBuffer = false;
    int currentPlayingSplice = 0;
    int positionInSplice = 0;

    juce::Random rand;

    double pitchRatio = 0;
    double sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;

    juce::ADSR adsr;

    int numSlices = 1;
    float playLength = 1.0f;
};


/*
  ==============================================================================

    SamplerVoice.cpp
    Created: 20 Sep 2022 2:41:50am
    Author:  phlie

  ==============================================================================
*/

#include "SamplerVoice.h"

bool SamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    // If sound can be dynamically casted as a SynthesiserSound, meaning it is derived from SynthesiserSound.
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{

}

void SamplerVoice::stopNote(float velocity, bool allowTailOff)
{

}

void SamplerVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SamplerVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SamplerVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{

}

// A general update method for use in updating variables.
void SamplerVoice::update(const float attack, const float decay, const float sustain, const float release)
{

}

void SamplerVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

}

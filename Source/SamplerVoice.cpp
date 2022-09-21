/*
  ==============================================================================

    SamplerVoice.cpp
    Created: 20 Sep 2022 2:41:50am
    Author:  phlie

  ==============================================================================
*/

#include "SamplerVoice.h"

SamplerVoice::SamplerVoice()
{
    juce::SamplerVoice::SamplerVoice();
}

SamplerVoice::~SamplerVoice()
{
    juce::SamplerVoice::~SamplerVoice();
}

bool SamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    // If sound can be dynamically casted as a SynthesiserSound, meaning it is derived from SynthesiserSound.
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    juce::SamplerVoice::startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
}

void SamplerVoice::stopNote(float velocity, bool allowTailOff)
{
    juce::SamplerVoice::stopNote(velocity, allowTailOff);
}

void SamplerVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SamplerVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SamplerVoice::setKnobParams(int ns, float pl)
{
    numSlices = ns;
    playLength = pl;
}

void SamplerVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

    if (!alreadyLoadedBuffer)
    {
        auto currentSound = dynamic_cast<juce::SamplerSound*> (getCurrentlyPlayingSound().get());
        if (currentSound == nullptr) { return; };
        buffer = currentSound->getAudioData();
        lengthOfBuffer = buffer->getNumSamples();
        alreadyLoadedBuffer = true;
        currentStartPosInBuffer = 0;
    }

    

    if (alreadyLoadedBuffer)
    {
        int lengthOfEachSlice = lengthOfBuffer / numSlices;
        int amountToPlayOfEachSlice = lengthOfEachSlice * playLength;
        int currentChangeSample = 0;
        int startForThisTime = currentPlayingSplice * lengthOfEachSlice + positionInSplice;
        for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
        {
            for (int sample = 0; sample < numSamples; sample++)
            {
                if (positionInSplice + sample > amountToPlayOfEachSlice)
                {
                    positionInSplice = 0;
                    currentChangeSample = sample;
                    currentPlayingSplice += 1;
                    if (currentPlayingSplice >= numSlices)
                    {
                        currentPlayingSplice = 0;
                    }
                    startForThisTime = currentPlayingSplice * lengthOfEachSlice + positionInSplice;
                }
                    /*currentPlayingSplice++;
                    positionInSplice = 0;
                    startForThisTime = currentPlayingSplice * lengthOfEachSlice + positionInSplice;
                    if (currentPlayingSplice > numSlices)
                    {
                        currentPlayingSplice = 0;
                    } */
                

                outputBuffer.addSample(channel, sample, buffer->getSample(0, sample + startForThisTime));
            }
        }
        positionInSplice += numSamples - currentChangeSample;
        //currentStartPosInBuffer += numSamples;
        //currentStartPosInBuffer = currentStartPosInBuffer + lengthOfBuffer / 512;
    }
    //juce::SamplerVoice::renderNextBlock(outputBuffer, startSample, numSamples);
    /*if (!isVoiceActive()) { return; }


    if (SamplerSound* playingSound = static_cast<SamplerSound*> (getCurrentlyPlayingSound().get()))
    {
        auto& data = *playingSound->data;
        const float* const inL = data.getReadPointer(0);
        const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : nullptr;

        float* outL = outputBuffer.getWritePointer(0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

        while (--numSamples >= 0)
        {
            auto pos = (int)sourceSamplePosition;
            auto alpha = (float)(sourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;

            // just using a very simple linear interpolation here..
            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
                : l;

            auto envelopeValue = adsr.getNextSample();

            l *= lgain * envelopeValue;
            r *= rgain * envelopeValue;

            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                *outL++ += (l + r) * 0.5f;
            }

            sourceSamplePosition += pitchRatio;

            //if (sourceSamplePosition > playingSound->getAudioData())
            if (sourceSamplePosition > 10000)
            {
                stopNote(0.0f, false);
                break;
            }
        }
    }*/
}

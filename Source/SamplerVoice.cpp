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
    // Call the parent's default constructor.
    juce::SamplerVoice::SamplerVoice();
}

SamplerVoice::~SamplerVoice()
{
    // [[[ERROR]]] Attempt to stop the leaked data
    buffer->clear();
    // Call the default destructor
    juce::SamplerVoice::~SamplerVoice();
}

bool SamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    // If sound can be dynamically casted as a SynthesiserSound, meaning it is derived from SynthesiserSound.
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int currentPitchWheelPosition)
{
    // If the sound is of type SamplerSound*...
    if (auto* sound = dynamic_cast<const juce::SamplerSound*> (s))
    {
        // Set the pitch ratio to the second power with an octave being 12.0 and middle C being 60
        // Since an increase of an octave is an increase in double the frequency or pitch Hz
        pitchRatio = std::pow(2.0, (midiNoteNumber - 60) / 12.0);

        //sourceSamplePosition = 0.0;
        //lgain = velocity;
        //rgain = velocity;

        //adsr.setSampleRate(sound->sourceSampleRate);
        //adsr.setParameters(sound->params);

        //adsr.noteOn();
        // Set the stopPlayback to be false.
        stopPlaybackNow = false;
    }
    // If we ever end up here we are using something that is not a SamplerSound when all sounds should be.
    else
    {
        jassertfalse; // this object can only play SamplerSounds!
    }
}

// The current stopNote command, could be improved
void SamplerVoice::stopNote(float velocity, bool allowTailOff)
{
    // Clear the current note this voice is playing
    clearCurrentNote();
    
    // Stop the current playback of this note
    stopPlaybackNow = true;
    
    // Set the current playing splice back to 0
    currentPlayingSplice = 0;

    // The position in splice should also be 0
    positionInSplice = 0.0f;
}
void SamplerVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SamplerVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

// The function for getting the values of the DSP knobs
void SamplerVoice::setKnobParams(int ns, float pl, float pt)
{
    numSlices = ns;
    playLength = pl;
    playbackTime = pt;
}

void SamplerVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    // If the note has been stopped, stop rendering
    if (stopPlaybackNow) { return; }

    // If the buffer has never been loaded before.
    if (!alreadyLoadedBuffer)
    {
        // Get the currently playing sound and cast it to of type SamplerSound [[[ERROR]]] Voice could be not a SamplerSound but a SynthVoice
        auto currentSound = dynamic_cast<juce::SamplerSound*> (getCurrentlyPlayingSound().get());
        if (currentSound == nullptr) { return; };

        // Get the entirerty of the sampled sound's audio data into this voice's buffer.
        buffer = currentSound->getAudioData();

        // Calculate the total amount of samples in the sound.
        lengthOfBuffer = buffer->getNumSamples();

        // The buffer has now been loaded.
        alreadyLoadedBuffer = true;
        
        // Not currently used.
        //currentStartPosInBuffer = 0;
    }

    // Once the buffer has been loaded.
    if (alreadyLoadedBuffer)
    {
        // Calculate the length of ecach slice
        int lengthOfEachSlice = lengthOfBuffer / numSlices;

        // Calculate how much to play of each splice
        float amountToPlayOfEachSlice = lengthOfEachSlice * playLength;

        // The point at which the sample should change over [[[ERROR]]]
        float currentChangeSample = 0.0;

        // The start position is the position in the splice + the position of the start of the slice.
        double startForThisTime = currentPlayingSplice * lengthOfEachSlice + positionInSplice;

        // Might not be needed since it is a mono sample or not [[[ERROR]]]
        for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
        {
            // Used to increment the playhead
            double sample = 0.0f;

            // Used to increment the buffer
            int bufferSample = 0;

            // Continue to loop while there is still data to fill
            while (bufferSample < numSamples)
            {
                // If the knob has a value of 0, make it a small value so sound is still played.
                if (playbackTime == 0.000f)
                    playbackTime = 0.01f;

                // In case the positionInSplice goes negative
                if (positionInSplice < 0.0f)
                {
                    positionInSplice = 0.0f;
                }

                // If it is time to change to go onto the next sample [[[DSP ERROR]]]
                if (positionInSplice + sample > amountToPlayOfEachSlice)
                {
                    // position in splice is reset.
                    positionInSplice = 0.0f;

                    // The current change sample is set to sample.
                    currentChangeSample = sample;

                    // Increment the current splice.
                    currentPlayingSplice++;

                    // If it is the last splice, reset
                    if (currentPlayingSplice >= numSlices)
                    {
                        currentPlayingSplice = 0;
                    }

                    // Calculate the start position for the sample head.
                    startForThisTime = currentPlayingSplice * lengthOfEachSlice + positionInSplice;
                }

                // If the sample buffer is going to overflow, intervene.
                if ((int)(startForThisTime + sample) > lengthOfBuffer - 1)
                {
                    startForThisTime = 0.0f;
                    sample = 0.0;
                }
                // startForThisTime should never be negative
                if (startForThisTime < 0.0f)
                {
                    startForThisTime = 0.0f;
                }
                /*if (positionInSplice < 0.0)
                {
                    positionInSplice = amountToPlayOfEachSlice;
                    currentChangeSample = (int)sample;
                    currentPlayingSplice--;
                    if (currentPlayingSplice <= 0)
                    {
                        currentPlayingSplice = numSlices;
                    }
                }
                    /*currentPlayingSplice++;
                    positionInSplice = 0;
                    startForThisTime = currentPlayingSplice * lengthOfEachSlice + positionInSplice;
                    if (currentPlayingSplice > numSlices)
                    {
                        currentPlayingSplice = 0;
                    } */

                // Add the sample to the outputBuffer.
                outputBuffer.addSample(channel, bufferSample, buffer->getSample(0, (int)(sample + startForThisTime)));
                
                // Increment te buffer count
                bufferSample++;
                //sample = sample + pow(2, (playbackTime - 1.0)) - 1  + pitchRatio - 1;

                // The next sample is the last sample plus the pitch ratio (which Hz to play) plus the pitch shift of the playback time knob, minus 1.0f to play at a normal speed normally.
                sample = sample + pitchRatio + pow(2, (double)(playbackTime/12.0f)) - 1.0f;
            }
        }
        // [[[ERROR]]] Should not be necessary since it falls outside the loop.
        positionInSplice += numSamples - currentChangeSample;
        //currentStartPosInBuffer += numSamples;
        //currentStartPosInBuffer = currentStartPosInBuffer + lengthOfBuffer / 512;
    }
    //juce::SamplerVoice::renderNextBlock(outputBuffer, startSample, numSamples);
    /*if (!isVoiceActive()) { return; }


    //=========================
    // The Code from the parent
    //=========================

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

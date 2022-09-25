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
    
    // Not really correct or needed [[[ERROR]]]
    //using juce::SynthesiserVoice::renderNextBlock;

    // The function called by Audio Processor to set the DSP Knobs internal values
    void setKnobParams(int ns, float pl, float pt);
private:
    // Create a Audio Sampler Buffer pointer.
    juce::AudioSampleBuffer* buffer;

    // Not currently used, depricated
    int currentStartPosInBuffer = 0;

    // Used to store the total number of samples in the currently loaded sample
    int lengthOfBuffer = 0;

    // A bool to determine if the buffer has yet been loaded or not.
    bool alreadyLoadedBuffer = false;

    // Keeps track of which division of the sample it is currently plaing.
    int currentPlayingSplice = 0;

    // Contains the current position the sample between loops, although [[[ERROR]]]
    float positionInSplice = 0.0f;

    // Used to not render if it is true.
    bool stopPlaybackNow = false;

    // Create an instance of the juce::Random class
    juce::Random rand;

    // The current Pitch Ratio
    double pitchRatio = 1.0f;

    // Not currently used
    //double sourceSamplePosition = 0;

    // Not currently used
    //float lgain = 0, rgain = 0;

    // Create an instance of an ADSR
    juce::ADSR adsr;

    // Set the number of slices to be a default of 1
    int numSlices = 1;

    // Set the playback length to be a default of 100%
    float playLength = 1.0f;

    // [[[CHECK]]] Might not be needed since it is the same as changing which note is played.
    int playbackTime = 0;
};


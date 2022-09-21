/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AMFSAudioProcessor::AMFSAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
         // The Default Value Tree State
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    // Register the basic formats
    formatManager.registerBasicFormats();

    // Loop through the total number of voices and add a default SamplerVoice class for each one.
    for (int i = 0; i < numVoices; ++i)
    {
        synthGranular.addVoice(new SamplerVoice());
    }
}

AMFSAudioProcessor::~AMFSAudioProcessor()
{
    reader = nullptr;
}

//==============================================================================
const juce::String AMFSAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AMFSAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AMFSAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AMFSAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AMFSAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AMFSAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AMFSAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AMFSAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AMFSAudioProcessor::getProgramName (int index)
{
    return {};
}

void AMFSAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AMFSAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Needs to be called for any playback to happen.
    synthGranular.setCurrentPlaybackSampleRate(sampleRate);
}

void AMFSAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AMFSAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AMFSAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    // Get the values for the 3 Buttons via apvts
    auto& open = *apvts.getRawParameterValue("OPEN");
    auto& play = *apvts.getRawParameterValue("PLAY");
    auto& stop = *apvts.getParameter("STOP");

    auto& numSlices = *apvts.getRawParameterValue("NUM SLICES");
    auto& playLength = *apvts.getRawParameterValue("PLAY LENGTH");
    auto& playbackTime = *apvts.getRawParameterValue("PLAYBACK TIME");

    if (auto casted = dynamic_cast<SamplerVoice*>(synthGranular.getVoice(0)))
    {
        casted->setKnobParams(numSlices.load(), playLength.load(), playbackTime.load());
    }


    // Do the default renderNextBlock for synthGranular.
    synthGranular.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

/*juce::AudioProcessorValueTreeState::Listener AMFSAudioProcessor::stopButtonClicked()
{
    jassertfalse;
}*/

//==============================================================================
bool AMFSAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AMFSAudioProcessor::createEditor()
{
    return new AMFSAudioProcessorEditor (*this);
}

//==============================================================================
void AMFSAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AMFSAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void AMFSAudioProcessor::loadFile()
{
    // Create a FileChooser pointer that only opens wav files.
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file...", juce::File{}, "*.wav");

    // Set the flags for the chooser.
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    // Launch into async
    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            // Get the results of what was chosen.
            auto file = fc.getResult();

            // If the file is not null.
            if (file != juce::File{})
            {
                // Create a reader format transmission from file to reader.
                reader = formatManager.createReaderFor(file);

                // If the reader ptr is not null
                if (reader != nullptr)
                {
                    // Create a range of all the keys on the keyboard.
                    juce::BigInteger range;
                    range.setRange(0, 128, true);

                    // Use the default method to add the sound to the sampler.
                    synthGranular.addSound(new juce::SamplerSound("Sample", *reader, range, 60, 0.1f, 0.1f, 20.0f));
                }
            }
        });
}

void AMFSAudioProcessor::playFile()
{
    // Turns C3 on with a velocity of 0.9f;
    synthGranular.noteOn(1, 60, 0.9f);
}

void AMFSAudioProcessor::stopFile()
{
    // Calls off the pressed note set to middle C.
    synthGranular.noteOff(1, 60, 1.0f, false);
}

juce::AudioProcessorValueTreeState::ParameterLayout AMFSAudioProcessor::createParams()
{
    // Create the beginnings of the apvts tree
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Add on 3 button parameters.
    params.push_back(std::make_unique<juce::AudioParameterBool>("OPEN", "Open", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("PLAY", "Play", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("STOP", "Stop", false));

    
    params.push_back(std::make_unique<juce::AudioParameterInt>("NUM SLICES", "Num Slices", 1, 32, 4));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PLAY LENGTH", "Play Length", 0.01f, 1.0f, 0.25f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("PLAYBACK TIME", "Playback Time", juce::NormalisableRange(-4.0f, 4.0f, 1.0f/12.0f), 1.0f));

    return { params.begin(), params.end() };
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AMFSAudioProcessor();
}

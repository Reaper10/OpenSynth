/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "TriangleWave.h"
#include "WavetableVoice.h"

//==============================================================================
/** A demo synth sound that's just a basic sine wave.. */
class SineWaveSound : public SynthesiserSound
{
public:
	SineWaveSound() {}

	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */
class SineWaveVoice : public SynthesiserVoice
{
public:
	SineWaveVoice()
		: angleDelta(0.0),
		tailOff(0.0)
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SineWaveSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity,
		SynthesiserSound* /*sound*/,
		int /*currentPitchWheelPosition*/) override
	{
		currentAngle = 0.0;
		level = velocity * 0.15;
		tailOff = 0.0;

		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		double cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = cyclesPerSample * 2.0 * double_Pi;
	}

	void stopNote(float /*velocity*/, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			// start a tail-off by setting this flag. The render callback will pick up on
			// this and do a fade out, calling clearCurrentNote() when it's finished.

			if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
								// stopNote method could be called more than once.
				tailOff = 1.0;
		}
		else
		{
			// we're being told to stop playing immediately, so reset everything..

			clearCurrentNote();
			angleDelta = 0.0;
		}
	}

	void pitchWheelMoved(int /*newValue*/) override
	{
		// can't be bothered implementing this for the demo!
	}

	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{
		// not interested in controllers in this case.
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

	void renderNextBlock(AudioBuffer<double>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

private:

	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
	{
		if (angleDelta != 0.0)
		{
			if (tailOff > 0)
			{
				while (--numSamples >= 0)
				{
					const FloatType currentSample =
						static_cast<FloatType> (std::sin(currentAngle) * level * tailOff);

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;

					tailOff *= 0.99;

					if (tailOff <= 0.005)
					{
						clearCurrentNote();

						angleDelta = 0.0;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0)
				{
					const FloatType currentSample = static_cast<FloatType> (std::sin(currentAngle) * level);

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;
				}
			}
		}
	}

	double currentAngle, angleDelta, level, tailOff;
};

//==============================================================================
NoisemakerAudioProcessor::NoisemakerAudioProcessor() :
	lastUIWidth(600),
	lastUIHeight(260),
	gainParam(nullptr),
	delayParam(nullptr),
	delayPosition(0),
    sawtoothWavetable(SawtoothWavetable(40.0, 4096, getSampleRate())),
    squareWavetable(SquareWavetable(40.0, 4096, getSampleRate()))
{
	lastPosInfo.resetToDefault();

	// This creates our parameters. We'll keep some raw pointers to them in this class,
	// so that we can easily access them later, but the base class will take care of
	// deleting them for us.
	addParameter(gainParam = new AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.9f));
	addParameter(delayParam = new AudioParameterFloat("delay", "Delay Feedback", 0.0f, 1.0f, 0.5f));
	addParameter(filterFrequencyParam = new AudioParameterFloat("filter", "Filter Frequency", 0.0f, 20000.0f, 20000.0f));
	addParameter(envAttackParam = new AudioParameterFloat("attack", "Envelope Attack", 0.0f, 3.0f, 1.0f));
	addParameter(envDecayParam = new AudioParameterFloat("decay", "Envelope Decay", 0.0f, 3.0f, 1.0f));

	ModulationParameter modParamFilter;
	modParamFilter.isModulated = false;
	modParamFilter.start = 0.0;
	modParamFilter.end = 1.0;
	modParamFilter.modulatorId = 1;
	modulationMatrix.set(ParameterTypeFilterFrequency, modParamFilter);

	ModulationParameter modParamGain;
	modParamGain.isModulated = false;
	modParamGain.start = 0.0;
	modParamGain.end = 1.0;
	modParamGain.modulatorId = 1;
	modulationMatrix.set(ParameterTypeGain, modParamGain);
    
	initialiseLowPassFilter();
	initialiseSynthForWaveform(WaveformSine, 8);
	keyboardState.addListener(this);
}

NoisemakerAudioProcessor::~NoisemakerAudioProcessor()
{
	removeAllModulators();
}

void NoisemakerAudioProcessor::initialiseSynthForWaveform(const Waveform waveform, const int numVoices)
{
	synth.clearSounds();
	synth.clearVoices();

	// Add some voices...
	for (int i = numVoices; --i >= 0;)
	{
		switch (waveform)
		{
		case WaveformSine:
			synth.addVoice(new SineWaveVoice());
			break;
		case WaveformSquare:
            {
                WavetableVoice *squareWaveVoice = new WavetableVoice(squareWavetable);
                synth.addVoice(squareWaveVoice);
            }
                break;
		case WaveformSawtooth:
            {
                WavetableVoice *sawtoothVoice = new WavetableVoice(sawtoothWavetable);
                sawtoothVoice->setReleaseLength(synth.getSampleRate() * 1);
                synth.addVoice(sawtoothVoice);
            }
			break;
        case WaveformTriangle:
            synth.addVoice(new TriangleWaveVoice());
            break;
		default:
			synth.addVoice(new SineWaveVoice());
		}
	}

	// ..and give the synth a sound to play
	switch (waveform)
	{
	case WaveformSine:
		synth.addSound(new SineWaveSound());
		break;
	case WaveformSquare:
		synth.addSound(new WavetableSound());
		break;
	case WaveformSawtooth:
		synth.addSound(new WavetableSound());
		break;
    case WaveformTriangle:
        synth.addSound(new TriangleWaveSound());
        break;
	default:
		synth.addSound(new SineWaveSound());
	}
}

void NoisemakerAudioProcessor::initialiseLowPassFilter()
{
	initialiseLowPassFilter(filterFrequencyParam->get());
}

void NoisemakerAudioProcessor::initialiseLowPassFilter(double frequency)
{
	filters.clear();
	for (int filter = 0; filter < 2; filter++)
	{ 
		Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 1> f(1024);
		Dsp::Params params;
		params[0] = 44100; // sample rate
		params[1] = 500; // cutoff frequency
		params[2] = 1.25; // Q
		f.setParams(params);
		filters.push_back(f);
	}
};

//==============================================================================
const String NoisemakerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoisemakerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NoisemakerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double NoisemakerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoisemakerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoisemakerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoisemakerAudioProcessor::setCurrentProgram (int index)
{
}

const String NoisemakerAudioProcessor::getProgramName (int index)
{
    return String();
}

void NoisemakerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NoisemakerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    sawtoothWavetable.setSampleRate(sampleRate);
    squareWavetable.setSampleRate(sampleRate);
	synth.setCurrentPlaybackSampleRate(sampleRate);
	currentSampleRate = sampleRate;
	keyboardState.reset();
	initialiseLowPassFilter();

	if (isUsingDoublePrecision())
	{
		delayBufferDouble.setSize(2, 12000);
		delayBufferFloat.setSize(1, 1);
	}
	else
	{
		delayBufferFloat.setSize(2, 12000);
		delayBufferDouble.setSize(1, 1);
	}

	reset();
}

void NoisemakerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	keyboardState.reset();
}

void NoisemakerAudioProcessor::reset()
{
	// Use this method as the place to clear any delay lines, buffers, etc, as it
	// means there's been a break in the audio's continuity.
	delayBufferFloat.clear();
	delayBufferDouble.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoisemakerAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

template <typename FloatType>
void NoisemakerAudioProcessor::process(AudioBuffer<FloatType>& buffer,
	MidiBuffer& midiMessages,
	AudioBuffer<FloatType>& delayBuffer)
{
	const int numSamples = buffer.getNumSamples();

	// In case we have more outputs than inputs, we'll clear any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		buffer.clear(i, 0, numSamples);

	// Now pass any incoming midi messages to our keyboard state object, and let it
	// add messages to the buffer if the user is clicking on the on-screen keys
	keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

	for (int idx = 0; idx < modulatorIDs.size(); idx++)
	{
		EnvelopeGenerator* env = modulatorsById[modulatorIDs[idx]];
		env->calculateEnvelopeBuffer(numSamples);
	}

	// Update filter frequency
	for (int filterId = 0; filterId < filters.size(); ++filterId)
	{
		double filterModulation = 1.0;
		ModulationParameter modParam = modulationMatrix[ParameterTypeFilterFrequency];
		uint32 modId = modParam.modulatorId;
		EnvelopeGenerator* envGenerator = nullptr;
		if (modulatorsById.contains(modId))
		{
			envGenerator = modulatorsById[modId];
		}

		bool isModulated = modParam.isModulated && envGenerator != nullptr;
		if (isModulated)
		{
			filterModulation = envGenerator->envelopeBuffer[0];
		}
		filters[filterId].setParam(Dsp::ParamID::idFrequency, filterFrequencyParam->get() * filterModulation);
	}

	// and now get our synth to process these midi events and generate its output.
	synth.renderNextBlock(buffer, midiMessages, 0, numSamples);

	// Apply filter
	applyFilter(buffer, delayBuffer);

	// apply our gain-change to the incoming data..
	applyGain(buffer, delayBuffer);

	// Apply our delay effect to the new output..
	applyDelay(buffer, delayBuffer);

	// Now ask the host for the current time so we can store it to be displayed later...
	updateCurrentTimeInfoFromHost();
}

template <typename FloatType>
void NoisemakerAudioProcessor::applyGain(AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer)
{
	ignoreUnused(delayBuffer);
	const float gainLevel = gainParam->get();
	ModulationParameter modParam = modulationMatrix[ParameterTypeGain];
	uint32 modId = modParam.modulatorId;
	EnvelopeGenerator* envGenerator = nullptr;
	if (modulatorsById.contains(modId))
	{
		envGenerator = modulatorsById[modId];
	}
	
	bool isModulated = modParam.isModulated && envGenerator != nullptr;

	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		FloatType* const channelData = buffer.getWritePointer(channel);
		isModulated = isModulated && envGenerator->envelopeBuffer.size() >= buffer.getNumSamples();
		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			channelData[sample] *= (gainLevel * (isModulated ? envGenerator->envelopeBuffer[sample] : 1.0));
		}
	}
}

template <typename FloatType>
void NoisemakerAudioProcessor::applyDelay(AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer)
{
	const int numSamples = buffer.getNumSamples();
	const float delayLevel = delayParam->get();

	int delayPos = 0;

	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		FloatType* const channelData = buffer.getWritePointer(channel);
		FloatType* const delayData = delayBuffer.getWritePointer(jmin(channel, delayBuffer.getNumChannels() - 1));
		delayPos = delayPosition;

		for (int i = 0; i < numSamples; ++i)
		{
			const FloatType in = channelData[i];
			channelData[i] += delayData[delayPos];
			delayData[delayPos] = (delayData[delayPos] + in) * delayLevel;

			if (++delayPos >= delayBuffer.getNumSamples())
				delayPos = 0;
		}
	}

	delayPosition = delayPos;
}

template <typename FloatType>
void NoisemakerAudioProcessor::applyFilter(AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer)
{
	ignoreUnused(delayBuffer);
	for (int channel = 0; channel < buffer.getNumChannels(); channel++)
	{
		FloatType* writePointer = buffer.getWritePointer(channel);
		FloatType* audioData[1];
		audioData[0] = writePointer;
		filters[channel].process(buffer.getNumSamples(), audioData);
	}
}

void NoisemakerAudioProcessor::updateCurrentTimeInfoFromHost()
{
	if (AudioPlayHead* ph = getPlayHead())
	{
		AudioPlayHead::CurrentPositionInfo newTime;

		if (ph->getCurrentPosition(newTime))
		{
			lastPosInfo = newTime;  // Successfully got the current time from the host..
			return;
		}
	}

	// If the host fails to provide the current time, we'll just reset our copy to a default..
	lastPosInfo.resetToDefault();
}

//==============================================================================
bool NoisemakerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NoisemakerAudioProcessor::createEditor()
{
    return new NoisemakerAudioProcessorEditor (*this);
}

//==============================================================================
void NoisemakerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// Here's an example of how you can use XML to make it easy and more robust:

	// Create an outer XML element..
	XmlElement xml("MYPLUGINSETTINGS");

	// add some attributes to it..
	xml.setAttribute("uiWidth", lastUIWidth);
	xml.setAttribute("uiHeight", lastUIHeight);

	// Store the values of all our parameters, using their param ID as the XML attribute
	for (int i = 0; i < getNumParameters(); ++i)
		if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
			xml.setAttribute(p->paramID, p->getValue());

	// then use this helper function to stuff it into the binary blob and return it..
	copyXmlToBinary(xml, destData);
}

void NoisemakerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	// This getXmlFromBinary() helper function retrieves our XML from the binary blob..
	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState != nullptr)
	{
		// make sure that it's actually our type of XML object..
		if (xmlState->hasTagName("MYPLUGINSETTINGS"))
		{
			// ok, now pull out our last window size..
			lastUIWidth = xmlState->getIntAttribute("uiWidth", lastUIWidth);
			lastUIHeight = xmlState->getIntAttribute("uiHeight", lastUIHeight);

			// Now reload our parameters..
			for (int i = 0; i < getNumParameters(); ++i)
				if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
					p->setValueNotifyingHost((float)xmlState->getDoubleAttribute(p->paramID, p->getValue()));
		}
	}
}

void NoisemakerAudioProcessor::setWaveform(Waveform waveform)
{
	initialiseSynthForWaveform(waveform, 8);
}

void NoisemakerAudioProcessor::addModulatorWithId(ModulatorType type, uint32 id)
{
	if (type == ModulatorTypeEnvelope)
	{
		EnvelopeGenerator* envelope = new EnvelopeGenerator();
		envelope->setSampleRate(currentSampleRate);
		envelope->resetEnvelope();
		modulatorsById.set(id, envelope);
		modulatorIDs.push_back(id);
	}
}

void NoisemakerAudioProcessor::removeModulatorWithId(uint32 id)
{
	if (modulatorsById.contains(id))
	{
		EnvelopeGenerator* modulator = modulatorsById[id];
		modulatorsById.remove(id);
		delete modulator;
	}
}

void NoisemakerAudioProcessor::removeAllModulators()
{
	HashMap<int, EnvelopeGenerator*>::Iterator i(modulatorsById);
	std::vector<int> keys;
	
	while (i.next())
	{
		keys.push_back(i.getKey());
	}
	for (int key : keys)
	{
		removeModulatorWithId(key);
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoisemakerAudioProcessor();
}

//==============================================================================
// MidiKeyboardStateListener
void NoisemakerAudioProcessor::handleNoteOn(MidiKeyboardState* source,
	int midiChannel, int midiNoteNumber, float velocity)
{
	for (int index = 0; index < modulatorIDs.size(); index++)
	{
		EnvelopeGenerator* modulator = modulatorsById[modulatorIDs[index]];
		const double attackRate = envAttackParam->get();
		const double decayRate = envDecayParam->get();

		modulator->setDurationInSec(attackRate + decayRate);
		modulator->setAttackRate(attackRate);
		modulator->setDecayRate(decayRate);
		modulator->resetEnvelope();
	}
}

void NoisemakerAudioProcessor::handleNoteOff(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{
}

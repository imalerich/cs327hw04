#ifndef IFUNCTION_H
#define IFUNCTION_H

#include "AudioFile.h"
#include "flags.h"

/**
 * Base interface for use in sound generation.
 * iFunction represent a generic function where given an input
 * (in this case time) it will produce an output (the audio
 * sample at the given time). A function should be theoretically
 * valid at any given 'time' postive or negative, but in practice
 * only needs to guarantee values from 0 -> 'Length', where
 * 'Length' is the parameter that will be used to generate an 
 * AudioFile from this iFunction.
 */
class iFunction {
public:
	/**
	 * Returns the output of this function at the given input time.
	 * \param time Input time to grab a sample from.
	 * \return The sample value at the given time.
	 */
	virtual long sampleAtTime(double time) = 0;

	/**
	 * The name of this function to use for the generated audio file.
	 */
	virtual string functionName() = 0;

	/**
	 * Generates a new audio file that represents a discrete
	 * version of this function. The generated file will only
	 * have one channel representing this function and will have
	 * samples from time 0 to time length as given by
	 * sampleAtTime(...) for parameter in increments of 1.0 / SampleRate.
	 * \param SampleRate Number of samples per second to take of this function.
	 * \param Length Length (in seconds) of the output AudioFile.
	 * \return Discrete AudioFile representing this function.
	 */
	AudioFile generateAudioFile(size_t SampleRate, double Length) {
		AudioFile f = AudioFile(functionName(), "iFunction", SampleRate, MIN_BIT_RES, 1);
		auto sample_count = Length * SampleRate;

		for (auto i = 0; i < sample_count; i++) {
			auto time = i / (double)SampleRate;
			f[0].push_sample(sampleAtTime(time));
		}

		return f;
	}

private:
}

#endif

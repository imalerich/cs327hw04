#ifndef IFUNCTION_H
#define IFUNCTION_H

#include "../AudioFile.h"
#include "../flags.h"

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
	virtual ~iFunction() { }

	/**
	 * Returns the output of this function at the given input time.
	 * \param time Input time to grab a sample from.
	 * \return The sample value at the given time.
	 */
	virtual double sample_at_time(double time) = 0;

	/**
	 * The name of this function to use for the generated audio file.
	 */
	virtual string function_name() = 0;

	/**
	 * Generates a new audio file that represents a discrete
	 * version of this function. The generated file will only
	 * have one channel representing this function and will have
	 * samples from time 0 to time length as given by
	 * sample_at_time(...) for parameter in increments of 1.0 / SampleRate.
	 * \param SampleRate Number of samples per second to take of this function.
	 * \param Length Length (in seconds) of the output AudioFile.
	 * \param BitRes Bit resolution to use in the generated AudioFile.
	 * \return Discrete AudioFile representing this function.
	 */
	AudioFile generate_audio_file(size_t SampleRate, double Length, size_t BitRes) {
		AudioFile f = AudioFile(function_name(), "iFunction", SampleRate, BitRes, 1);
		auto sample_count = Length * SampleRate;

		for (auto i = 0; i < sample_count; i++) {
			auto time = i / (double)SampleRate;
			f[0].push_sample(sample_at_time(time));
		}

		return f;
	}

	/**
	 * Multiplies the values of this continuous function with 
	 * the corresponding samples in the input AudioFile.
	 * The multiplication will be performed across all channels.
	 * \param file File whos samples will be multiplied by this function to generate the result.
	 * \return AudioFile representation of the result.
	 */
	AudioFile operator*(const AudioFile file) {
		AudioFile last = file;

		// go through each channel in this AudioFile
		for (auto c = 0; c < (int)last.get_num_channels(); c++) {
			Channel &channel = last[c];

			// multiply each sample by the func at the given time
			for (auto i = 0; i < (int)channel.size(); i++) {
				auto time = i / (double)last.get_sample_rate();
				channel[i] *= sample_at_time(time);
			}
		}

		return last;
	}

private:
};

#endif

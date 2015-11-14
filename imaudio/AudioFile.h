#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <array>
#include <exception>
#include <stdexcept>

#include "Channel.h"

using namespace std;

/**
 * Generic representation of an audio file.
 * Contains any number of channels to represent
 * the audio files data, as well as the necessary data
 * for playback of those channels.
 */
class AudioFile {
public:
	/**
	 * Constructs a new audio file, thiis file will have the given 
	 * sample rate, bit res, and number of channels. Each channel will have
	 * the same bit res as this audio file. Each channel can then be accessed
	 * using the [] operator.
	 * \param SampleRate Number of samples per second for this audio file.
	 * \param BitRes Number of bits per byte to use for each channel.
	 * \param NumChannels Number of 'Channels' to create.
	 * \param Strict Determines whether or not Channels use 'strict_data'.
	 */
	AudioFile(size_t SampleRate, size_t BitRes, size_t NumChannels, bool Strict = true);

	/**
	 * \return Current number of channels held by this AudioFile.
	 */
	inline size_t get_num_channels() const {
		return channels.size();
	}

	/**
	 * \return The number of samples per second for each channel.
	 */
	inline size_t get_sample_rate() const {
		return sample_rate;
	}

	/**
	 * \return The number of bits per sample for each channel.
	 */
	inline size_t get_bit_res() const {
		return bit_res;
	}

	/**
	 * Determines the number of samples for this AudioFile, this is pulled from
	 * the first Channel, as all channels MUST have the same number of samples.
	 * If there are no channels in this AudioFile, an invalid_argument 
	 * \return The number of samples for this AudioFile.
	 */
	inline size_t get_num_samples() const {
		if (channels.size() > 0) {
			throw out_of_range("Cannot determine the number of samples of with no channels.");
		} else {
			return channels[0].size();
		}
	}

	/**
	 * Byte Rate, as calculated by the sample_rate * num_channels * bit_rest
	 * \return The byte rate (in bytes per second) of this audio file.
	 */
	inline size_t get_byte_rate() const {
		return get_sample_rate() * get_num_channels() * get_bit_res();
	}

	/**
	 * Returns a reference to the channel at the given index.
	 * The input parameter should be within the range [0, num_channels),
	 * this method does no additional checking from the [] operator
	 * of a std::vector.
	 * \param n Index of the channel.
	 * \return The channel at the given index.
	 */
	inline Channel& operator[](size_t n) {
		return channels[n];
	}

private:
	const size_t sample_rate; /**< Number of samples to be played back per second. */
	const size_t bit_res; /**< Bit resolution to use for all of this files channels. */
	const size_t num_channels; /**< Number of channels for this audio file. */
	vector<Channel> channels; /**< Array of this files audio channels. */
};

#endif

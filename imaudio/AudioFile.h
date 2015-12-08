#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H 

#include <array>
#include <exception>
#include <stdexcept>

#include "Channel.h" 

using namespace std;

/**
 * Generic representation of an audio AudioFile, independent of file format.
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
	 * If NumChannels is < 1, this function will throw an invalid_argument exception.
	 * \param FileName Name of the file this AudioFile was created from.
	 * \param Extension Representation of the format this file was loaded from.
	 * \param SampleRate Number of samples per second for this audio file.
	 * \param BitRes Number of bits per byte to use for each channel.
	 * \param NumChannels Number of 'Channels' to create.
	 */
	AudioFile(string FileName, string Extension, size_t SampleRate,
			size_t BitRes, size_t NumChannels);
	AudioFile(const AudioFile &other);
	AudioFile(const AudioFile &&other);
	AudioFile& operator=(const AudioFile &other);
	AudioFile& operator=(const AudioFile &&other);
	friend ostream& operator<<(ostream &os, const AudioFile &file);

	/**
	 * Concats the contents of each Channel to this AudioFiles channels.
	 * If strict data is enabled:
	 * 	Each AudioFile must have the same number of channels.
	 * 	Each AudioFile must have the same bit res.
	 * Else
	 * 	Resulting AudioFile will have the larger number of channels.
	 * 	Resulting AudioFile will have the larger bit res.
	 * \param other AudioFile to concat to this AudioFile.
	 * \return New AudioFile with data from both 'this' and 'other'.
	 */
	AudioFile concat(const AudioFile &other);

	/**
	 * Applies the input scalar across every sample
	 * of every channel contained within this AudioFile.
	 * The only potentional error of this method is if
	 * the input scalar causes a sample to leave its 
	 * appropriate bit resolution.
	 * \param scalar Input scalar to apply to channels.
	 * \return AudioFile representation of the result.
	 */
	AudioFile operator*(const double scalar);

	/**
	 * Adds each sample of this AudioFile to the 
	 * corresponding sample in the corresponding Channel
	 * of the other AudioFile.
	 * The rules with regards to strict data for this method
	 * are identical to concat, with the exception of the number
	 * of samples. With this method, if strict data is enabled
	 * the number of samples must match (otherwise 0's will be added).
	 * \param other AudioFile to sum with this AudioFile.
	 * \return AudioFile representation of the result.
	 */
	AudioFile operator+(const AudioFile &other);

	/**
	 * Multiplies each sample of this AudioFile with
	 * the corresponding sample in the corresponding Channel
	 * of the other AudioFile.
	 * The rules with regards to strict data for this method
	 * are identical to operator+(const AudioFile &other).
	 * \param other AudioFile to multiply with this AudioFile.
	 * \return AudioFile representation of the result.
	 */
	AudioFile operator*(const AudioFile &other);

	/**
	 * Takes very sample of the channel at the input index and replaces
	 * the value with '0'.
	 * \param index Index of the channel to mute.
	 */
	void mute_channel(unsigned index);

	/**
	 * \return Current number of channels held by this AudioFile.
	 */
	inline size_t get_num_channels() const {
		return num_channels;
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
	 * \return The number of samples for this AudioFile.
	 */
	inline size_t get_num_samples() const {
		return channels[0].size();
	}

	/**
	 * Byte Rate, as calculated by the sample_rate * num_channels * bit_rest
	 * \return The byte rate (in bytes per second) of this audio file.
	 */
	inline size_t get_byte_rate() const {
		return get_sample_rate() * get_num_channels() * get_bit_res();
	}

	/**
	 * \return The name for this AudioFile.
	 */
	inline string get_file_name() const {
		return file_name;
	}

	/**
	 * \return The format this file was created from.
	 */
	inline string get_extension() const {
		return extension;
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

	/**
	 * Similar to operator[] but returns a copy instead of
	 * a reference, can be accessed as a constant.
	 * \param n Index of the channel.
	 * \return A copy of the channel at the given index.
	 */
	inline Channel get_channel(size_t n) const {
		return channels[n];
	}

	/**
	 * User should always check if this AudioFile's channels are valid.
	 * To be valid, each channel must simply have the same size().
	 * If any channel has more samples than another channels, the user
	 * may call make_valid() to fill the smaller values with '0' values
	 * to match the largest channel.
	 */
	bool are_channels_valid();

	/**
	 * If this AudioFile is valid this method does nothing.
	 * If all channels do not have the same number of samples,
	 * this method will add '0' values to the shorter channels.
	 */
	void make_valid();

private:
	const string file_name; /**< Name of the file (if available) this AudioFile was created from */
	const string extension; /**< File extensions this file was loaded from. */
	const size_t sample_rate; /**< Number of samples to be played back per second. */
	const size_t bit_res; /**< Bit resolution to use for all of this AudioFile's channels. */
	const size_t num_channels; /**< Number of channels for this audio AudioFile. */
	vector<Channel> channels; /**< Array of this AudioFile's channels. */
};

#endif

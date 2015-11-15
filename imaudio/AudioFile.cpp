#include "AudioFile.h"

#include <string>
#include <algorithm>

static const string invalid_msg = "Must have at least 1 Sample to construct an AudioFile.";

AudioFile::AudioFile(size_t SampleRate, size_t BitRes, size_t NumChannels, bool Strict) : 
		sample_rate{SampleRate}, bit_res{BitRes}, num_channels{NumChannels} { 
	if (num_channels < 1) {
		throw invalid_argument(invalid_msg);
	}

	// initalize each channel for this sound file, users of this class will not be able to replace these
	for (auto i = 0; i < (int)num_channels; i++) {
		channels.push_back(Channel(bit_res, Strict));
	}
}

bool AudioFile::are_channels_valid() {
	auto num_channels = get_num_channels();
	for (auto &channel : channels) {
		if (channel.size() != num_channels) {
			return false;
		}
	}

	return true;
}

void AudioFile::make_valid() {
	if (are_channels_valid()) {
		return;
	}

	// find the maximum number of channels across all channels
	auto max_channels = get_num_channels();
	for (auto &channel : channels) {
		max_channels = max(max_channels, channel.size());
	}

	// push '0' values to smaller channels until they match 'max_channels'
	for (auto &channel : channels) {
		while (channel.size() != max_channels) {
			channel.push_sample(0);
		}
	}

	// AudioFile is now valid
}

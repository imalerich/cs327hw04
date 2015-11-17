#include <string>
#include <algorithm>

#include "AudioFile.h"

static const string invalid_num_channels = "Invalid num_channels in constructor.";
static const string invalid_bit_res = "Invalid bit_res in constructor.";
static const string invalid_assign = "Must have matching sample_rate, bit_res, and num_channels.";

AudioFile::AudioFile(string FileName, size_t SampleRate, size_t BitRes, size_t NumChannels, bool Strict) : 
		file_name{FileName}, sample_rate{SampleRate}, bit_res{BitRes}, num_channels{NumChannels} { 
	if (num_channels < 1 || num_channels >= 128) {
		throw invalid_argument(invalid_num_channels);
	}

	if (bit_res != 8 && bit_res != 16 && bit_res != 32) {
		throw invalid_argument(invalid_bit_res);
	}

	// initalize each channel for this sound file, users of this class will not be able to replace these
	for (auto i = 0; i < (int)num_channels; i++) {
		channels.push_back(Channel(bit_res, Strict));
	}
}

AudioFile::AudioFile(const AudioFile &other) :
		file_name{other.file_name}, sample_rate{other.sample_rate}, 
		bit_res{other.bit_res}, num_channels{other.num_channels} { 
	// copy all channels from 'other' to 'this'
	channels = vector<Channel>(channels);
}

AudioFile::AudioFile(const AudioFile &&other) :
		file_name{other.file_name}, sample_rate{other.sample_rate}, 
		bit_res{other.bit_res}, num_channels{other.num_channels} { 
	channels = move(other.channels);
}

AudioFile& AudioFile::operator=(const AudioFile &other) {
	if (other.sample_rate != sample_rate || other.bit_res != bit_res || 
			other.num_channels != num_channels) {
		throw invalid_argument(invalid_assign);
	}

	channels = vector<Channel>(channels);
	return *this;
}

AudioFile& AudioFile::operator=(const AudioFile &&other) {
	if (other.sample_rate != sample_rate || other.bit_res != bit_res || 
			other.num_channels != num_channels) {
		throw invalid_argument(invalid_assign);
	}

	channels = move(other.channels);
	return *this;
}

ostream& operator<<(ostream &os, const AudioFile &file) {
	os << "File Name:\t" << file.get_file_name() << endl;
	os << "File Type:\t" << ".cs229" << endl;
	os << "Sample Rate:\t" << file.sample_rate << endl;
	os << "Bit Depth:\t" << file.bit_res << endl;
	os << "Num Channels:\t" << file.num_channels << endl;
	os << "Num Samples:\t" << file.get_num_samples() << endl;
	os << "Length:\t\t" << (file.get_num_samples() / file.sample_rate) << endl;

	return os;
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

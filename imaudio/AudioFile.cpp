#include <string>
#include <algorithm> 
#include "AudioFile.h"
#include "flags.h"

static const string invalid_num_channels = "Invalid num_channels in constructor.";
static const string invalid_bit_res = "Invalid bit_res in constructor.";
static const string invalid_assign = "Must have matching sample_rate, bit_res, and num_channels.";

AudioFile::AudioFile(string FileName, string Extension, size_t SampleRate, 
		size_t BitRes, size_t NumChannels) : 
		file_name{FileName}, extension{Extension}, sample_rate{SampleRate}, 
		bit_res{BitRes}, num_channels{NumChannels} { 
	if (num_channels < 1 || num_channels >= 128) {
		throw invalid_argument(invalid_num_channels);
	}

	if (bit_res != 8 && bit_res != 16 && bit_res != 32) {
		throw invalid_argument(invalid_bit_res);
	}

	// initalize each channel for this sound file, users of this class will not be able to replace these
	for (auto i = 0; i < (int)num_channels; i++) {
		channels.push_back(Channel(bit_res));
	}
}

AudioFile::AudioFile(const AudioFile &other) :
		file_name{other.file_name}, extension{other.extension}, sample_rate{other.sample_rate}, 
		bit_res{other.bit_res}, num_channels{other.num_channels} { 
	// copy all channels from 'other' to 'this'
	channels = other.channels;
}

AudioFile::AudioFile(const AudioFile &&other) :
		file_name{other.file_name}, extension{other.extension}, sample_rate{other.sample_rate}, 
		bit_res{other.bit_res}, num_channels{other.num_channels} { 
	channels = move(other.channels);
}

AudioFile& AudioFile::operator=(const AudioFile &other) {
	if (strict_data) {
		if (other.sample_rate != sample_rate || other.bit_res != bit_res || 
				other.num_channels != num_channels) {
			throw invalid_argument(invalid_assign);
		}
	}

	// we need to pull constants from 'other', not use our current constants
	*const_cast<string*>(&file_name) = other.file_name;
	*const_cast<string*>(&extension) = other.extension;
	*const_cast<size_t*>(&sample_rate) = other.sample_rate;
	*const_cast<size_t*>(&bit_res) = other.bit_res;
	*const_cast<size_t*>(&num_channels) = other.num_channels;

	channels = other.channels;
	return *this;
}

AudioFile& AudioFile::operator=(const AudioFile &&other) {
	if (strict_data) {
		if (other.sample_rate != sample_rate || other.bit_res != bit_res || 
				other.num_channels != num_channels) {
			throw invalid_argument(invalid_assign);
		}
	}

	// we need to pull constants from 'other', not use our current constants
	*const_cast<string*>(&file_name) = other.file_name;
	*const_cast<string*>(&extension) = other.extension;
	*const_cast<size_t*>(&sample_rate) = other.sample_rate;
	*const_cast<size_t*>(&bit_res) = other.bit_res;
	*const_cast<size_t*>(&num_channels) = other.num_channels;

	channels = move(other.channels);
	return *this;
}

ostream& operator<<(ostream &os, const AudioFile &file) {
	os << "File Name:\t" << file.get_file_name() << endl;
	os << "File Type:\t" << file.get_extension() << endl;
	os << "Sample Rate:\t" << file.sample_rate << endl;
	os << "Bit Depth:\t" << file.bit_res << endl;
	os << "Num Channels:\t" << file.num_channels << endl;
	os << "Num Samples:\t" << file.get_num_samples() << endl;
	os << "Length:\t\t" << file.get_num_samples() / (double)file.sample_rate << " seconds" << endl;

	return os;
}

AudioFile AudioFile::concat(const AudioFile &other) {
	if (strict_data) {
		if (other.bit_res != bit_res) {
			throw invalid_argument("other.bit_res must match this->bit_res");
		}

		if (other.num_channels != num_channels) {
			throw invalid_argument("other.num_channels must match this->num_channels");
		}

		if (other.sample_rate != sample_rate) {
			throw invalid_argument("other.sample_rate must ALWAYS match this->sample_rate");
		}
	}

	const AudioFile &larger = num_channels > other.num_channels ? *this : other;

	AudioFile last = AudioFile(file_name + " + "  + other.file_name, 
			extension + " + " + other.extension, larger.sample_rate, 
			max(bit_res, other.bit_res), larger.num_channels);

	// add this objects channel data first
	for (auto i = 0; i < (int)num_channels; i++) {
		last[i].append(channels[i]);
	}

	// then push the other channels data
	for (auto i = 0; i < (int)other.num_channels; i++) {
		last[i].append(other.channels[i]);
	}

	last.make_valid();

	return last;
}

AudioFile AudioFile::operator*(const double scalar) {
	AudioFile last = *this;
	for (auto i = 0; i < (int)last.num_channels; i++) {
		last[i] = last[i] * scalar;
	}

	return last;
}

AudioFile AudioFile::operator+(const AudioFile &other) {
	if (strict_data) {
		if (other.bit_res != bit_res) {
			throw invalid_argument("other.bit_res must match this->bit_res");
		}

		if (other.num_channels != num_channels) {
			throw invalid_argument("other.num_channels must match this->num_channels");
		}

		if (other.get_num_samples() != get_num_samples()) {
			throw invalid_argument("other.num_samples must match this->num_samples");
		}

		if (other.sample_rate != sample_rate) {
			throw invalid_argument("other.sample_rate must ALWAYS match this->sample_rate");
		}
	}

	const AudioFile &larger = num_channels > other.num_channels ? *this : other;
	const AudioFile &smaller = num_channels > other.num_channels ? other : *this;

	AudioFile last = AudioFile(file_name + " + "  + other.file_name, 
			extension + " + " + other.extension, larger.sample_rate, 
			max(bit_res, other.bit_res), larger.num_channels);

	// add this objects channel data first
	for (auto i = 0; i < (int)larger.num_channels; i++) {
		// get_channel should use the move constructor into last[i]
		last[i].append(larger.get_channel(i));
	}

	// add the data of the smallers channel next
	for (auto i = 0; i < (int)smaller.num_channels; i++) {
		// will automatically increase channel size if necessary
		last[i] = last[i] + smaller.get_channel(i);
	}

	return last;
}

AudioFile AudioFile::operator*(const AudioFile &other) {
	if (strict_data) {
		if (other.bit_res != bit_res) {
			throw invalid_argument("other.bit_res must match this->bit_res");
		}

		if (other.num_channels != num_channels) {
			throw invalid_argument("other.num_channels must match this->num_channels");
		}

		if (other.get_num_samples() != get_num_samples()) {
			throw invalid_argument("other.num_samples must match this->num_samples");
		}

		if (other.sample_rate != sample_rate) {
			throw invalid_argument("other.sample_rate must ALWAYS match this->sample_rate");
		}
	}

	const AudioFile &larger = num_channels > other.num_channels ? *this : other;
	const AudioFile &smaller = num_channels > other.num_channels ? other : *this;

	AudioFile last = AudioFile(file_name + " + "  + other.file_name, 
			extension + " + " + other.extension, larger.sample_rate, 
			max(bit_res, other.bit_res), larger.num_channels);

	// add this objects channel data first
	for (auto i = 0; i < (int)larger.num_channels; i++) {
		// get_channel should use the move constructor into last[i]
		last[i].append(larger.get_channel(i));
	}

	// add the data of the smallers channel next
	for (auto i = 0; i < (int)smaller.num_channels; i++) {
		// will automatically increase channel size if necessary
		last[i] = last[i] * smaller.get_channel(i);
	}

	last.make_valid();

	return last;
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

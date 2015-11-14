#include "AudioFile.h"

AudioFile::AudioFile(size_t SampleRate, size_t BitRes, size_t NumChannels, bool Strict) : 
		sample_rate{SampleRate}, bit_res{BitRes}, num_channels{NumChannels} { 
	// initalize each channel for this sound file, users of this class will not be able to replace these
	for (auto i = 0; i < (int)num_channels; i++) {
		channels.push_back(Channel(bit_res, Strict));
	}
}

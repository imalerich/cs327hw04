#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdint.h>

#include "AudioFile.h"
#include "WavReader.h"

AudioFile WavReader::read_file(istream &is, string filename) {
	// read the header
	char * header = new char[5];
	char * wave = new char[5];
	header[4] = 0;
	wave[4] = 0;

	int32_t num_bytes = 0;

	is.read(header, 4);
	is.read((char *)&num_bytes, 4);
	is.read(wave, 4);

	if (strcasecmp(header, "RIFF") != 0 || strcasecmp(wave, "WAVE") != 0) {
		throw invalid_argument("Input file is not of Wav format.");
	}

	delete header;
	delete wave;

	// next read the format chunk
	char * format = new char[5];
	format[4] = 0;
	int32_t bytes_in_format = 0;
	int16_t audio_format = 0;

	is.read(format, 4);
	is.read((char *)&bytes_in_format, 4);
	is.read((char *)&audio_format, 2);

	if (strcasecmp(format, "fmt ") != 0 || bytes_in_format != 16) {
		throw invalid_argument("expected the fmt chunk first and 16 bytes in format");
	}

	if (audio_format != 1) {
		throw invalid_argument("expected an audio format of '1'");
	}

	delete format;

	is.read((char *)&num_channels, 2);
	is.read((char *)&sample_rate, 4);
	is.read((char *)&byte_rate, 4);
	is.read((char *)&block_align, 2);
	is.read((char *)&bit_res, 2);

	// create the AudioFile we will be returning
	AudioFile ret = AudioFile(filename, ".wav", sample_rate, bit_res, num_channels);

	// read the data chunk
	char * data = new char[5];
	data[4] = 0;
	int32_t bytes_in_data = 0;

	is.read(data, 4);
	is.read((char *)&bytes_in_data, 4);

	if (strcasecmp(data, "data") != 0) {
		throw invalid_argument("Expection data chunk after reading the format.");
	}

	unsigned current_channel = 0;
	unsigned num_samples = bytes_in_data / (bit_res / 8);

	for (unsigned i = 0; i < num_samples; i++) {
		ret[current_channel].push_sample(get_sample(is));
		current_channel = (current_channel + 1) % num_channels;
	}

	return ret;
}

long WavReader::get_sample(istream &is) {
	if (bit_res == 8) {
		// wav files use unsigned values, so convert to a signed value for the rest of the program
		uint8_t data;
		is.read((char *)&data, 1);
		return (long)data - 127;

	} else if (bit_res == 16) {
		int16_t data;
		is.read((char *)&data, 2);
		return (long)data;

	} else if (bit_res == 32) {
		int32_t data;
		is.read((char *)&data, 4);
		return (long)data;

	} else {
		throw invalid_argument("Expected a BitRes of 8, 16, or 32");
	}

	return 0.0;
}

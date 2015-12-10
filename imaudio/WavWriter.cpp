#include <iostream>
#include <fstream>
#include <stdint.h>

#include "WavWriter.h"

void WavWriter::write_file(AudioFile &file, ostream &os) {
	long samples_count = file.get_num_samples() * file.get_num_channels();
	long samples_bytes = samples_count * (file.get_bit_res() / 8);
	const char * header = "RIFF";
	const char * wave = "WAVE";
	os.write(header, 4);
	write_integer((4) + (24) + (samples_bytes + 8), 32, os); // total number of bytes remaining
	os.write(wave, 4);

	// write the format chunk (24 bytes total)
	const char * fmt = "fmt ";
	size_t byte_rate = file.get_sample_rate() * file.get_num_channels() * file.get_bit_res();
	size_t block_align = file.get_num_channels() * file.get_bit_res();
	os.write(fmt, 4);
	write_integer(16, 32, os); // remaining bytes in chunk
	write_integer(1, 16, os); // AudioFormat
	write_integer(file.get_num_channels(), 16, os); // NumChannels
	write_integer(file.get_sample_rate(), 32, os); // SampleRate
	write_integer(byte_rate, 32, os); // ByteRate
	write_integer(block_align, 16, os); // BlockAlign
	write_integer(file.get_bit_res(), 16, os); // BitDepth

	// finally write all the data (samples_bytes + 8 bytes total)
	const char * data = "data";
	os.write(data, 4);
	write_integer(samples_bytes, 32, os); // remaining bytes in chunk
	for (size_t i = 0; i < file.get_num_samples(); i++) {
		for (size_t c = 0; c < file.get_num_channels(); c++) {
			write_integer(file[c][i], file.get_bit_res(), os);
		}
	}
}

void WavWriter::write_integer(long data, size_t bits, ostream &os) {
	if (bits == 8) {
		// .wav files use unsigne data, account for this when outputing data
		uint8_t out = (uint8_t)(data + 127);
		os.write((const char *)&out, 1);
	} else if (bits == 16) {
		int16_t out = (int16_t)data;
		os.write((const char *)&out, 2);
	} else if (bits == 32) {
		int32_t out = (int32_t)data;
		os.write((const char *)&out, 4);
	} else {
		throw invalid_argument("Invalid 'bits' for file output, must be 8, 16, or 32.");
	}
}

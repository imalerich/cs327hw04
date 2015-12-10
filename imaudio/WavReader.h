#ifndef WAVREADER_H
#define WAVREADER_H

#include "iFileReader.h"
#include "AudioFile.h"

using namespace std;

class WavReader : public iFileReader {
public:
	AudioFile read_file(string filename) { return iFileReader::read_file(filename); }
	virtual AudioFile read_file(istream &is, string filename = "std::cin");

private:
	/**
	 * Reads 'bit_res' /  8 bytes of data from the input stream.
	 * The data read is then returned as a long integer.
	 * \param is Input stream to read data from.
	 * \return The input data formatted as a long integer.
	 */
	long get_sample(istream &is);

	int32_t sample_rate; /**< Sample Rate as read from the Wav file. */
	int16_t bit_res; /**< Bit Res as read from the Wav file. */
	int16_t num_channels; /**< Number of Channels as read from the Wav file. */
	int32_t byte_rate; /**< Byte Rate as read from the Wav file. */
	int16_t block_align; /**< Block Align as read from the Wav file. */
};

#endif

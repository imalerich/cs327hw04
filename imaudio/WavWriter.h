#ifndef WAVWRITER_H
#define WAVWRITER_H

#include "iFileWriter.h"

class WavWriter : public iFileWriter {
public:
	void write_file(AudioFile &file, string filename) { iFileWriter::write_file(file, filename); }
	virtual void write_file(AudioFile &file, ostream &os);

private:
	/**
	 * Writes 'data' to the output stream 'os' such that it fits within the 'bits' size.
	 * \param data Data to write to the file.
	 * \param bits Number of bits to output.
	 * \param os Ouput stream to write to.
	 */
	void write_integer(long data, size_t bits, ostream &os);
};

#endif

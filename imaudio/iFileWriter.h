#ifndef I_FILE_WRITER_H
#define I_FILE_WRITER_H

#include <iostream>
#include <string>

#include "AudioFile.h"

using namespace std;

static const string file_write_msg = "Failed to open file for writing.";

/**
 * Interface for a file writer class.
 * Subclasses of iFileWriter should define methods to output
 * an AudioFile to either a file (given a filename) or to an
 * output stream (given an ostream). A subclass of 
 * iFileWriter represents a possible file format that 
 * AudioFile can be written to.
 */
class iFileWriter {
public:
	/**
	 * This method formats the data of the input file in the file
	 * format represented by this interfaces subclasses.
	 * That file data is then saved to a file with the input
	 * filename. If that file already exists, it should be erased.
	 * \param file Input file to write to a file.
	 * \param filename Name of the file to write data to.
	 */
	virtual void writeFile(AudioFile &file, string filename) = 0;

	/**
	 * This method formats the data of of the input file in the file
	 * format represented by this interfaces subclassses.
	 * That file data is then sent to the output stream
	 * given as a parameter to this method.
	 * \param file Input file to write to the output stream.
	 * \param os Output stream to write data to.
	 */
	virtual void writeFile(AudioFile &file, ostream &os) = 0;
};

#endif

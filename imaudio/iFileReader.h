#ifndef I_FILE_READER_H
#define I_FILE_READER_H

#include <iostream>
#include <string>

#include "AudioFile.h"

using namespace std;

static const string file_read_msg = "Failed to open file for reading.";

/**
 * Interface for a file reader class.
 * Subclasses of iFileReader should define methods to
 * read an AudioFile from either a file (given a filename) or
 * from an input stream (given an istream). A subclass of
 * iFileReader represents a possible file format
 * that AudioFile can be read from.
 */
class iFileReader {
public:
	virtual ~iFileReader() { }
	
	/**
	 * This method reads data from the input file in the file format defined
	 * by the subclass. Using that data, a new AudioFile is created, and then
	 * returned to the caller.
	 * \param filename Input filename to read an AudioFile from.
	 * \return AudioFile The AudioFile as parsed from the input file.
	 */
	AudioFile read_file(string filename) {
		ifstream file(filename);
		if (!file.is_open()) {
			throw invalid_argument(file_read_msg);
		}
	
		AudioFile ret = read_file(file, filename);
		file.close();
		return ret;
	}

	/**
	 * This method reads bytes from an input stream in the file format defined
	 * by the subclass. Using that data, a new AudioFile is created, and 
	 * then returned to the caller.
	 * \param is Input stream to read an AudioFile from.
	 * \param filename The name of the file we are reading from ("std::cin" by default).
	 * \return AudioFile The AudioFile as parsed from the input stream.
	 */
	virtual AudioFile read_file(istream &is, string filename = "std::cin") = 0;
};

#endif

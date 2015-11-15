#ifndef FORMAT_CS229_H
#define FORMAT_CS229_H

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "iFileReader.h"
#include "iFileWriter.h"
#include "AudioFile.h"

using namespace std;

class FormatCS229 : iFileWriter, iFileReader {
public:
	// this format supports reading from a file.
	virtual AudioFile readFile(string filename);
	virtual AudioFile readFile(istream &is);

	// This format supports writing to a file.
	virtual void writeFile(const AudioFile &file, string filename) { }
	virtual void writeFile(const AudioFile &file, ostream &os) { }

private:
	/**
	 * The first line of data should contain a word describing the file format.
	 * This method will check that line and validate that the file is specified to
	 * be of type CS229. If not, this method will throw an invalid_argument exception.
	 * \param First line of data, which should contain the word 'CS229' and nothing else.
	 * \return True if the above condition is met.
	 */
	bool check_format(string line);

	/**
	 * Process a line of data from the header.
	 * All valid header data will be stored in his classes 
	 * 'header' map under their appropriate key.
	 * If a key value or unexpected data is found, this
	 * method will throw an invalid_argument exception.
	 * \param line String representation of the line to parse.
	 * \return True if there is still more data to read.
	 */
	bool proc_header_line(string line);

	/**
	 * Determines whether or not the given line should be ignored.
	 * For the CS229 file, ignored lines are lines that are
	 * either entirely empty, or files whose first non white space
	 * character is a '#' to mark a comment line.
	 * \param line String representation of the line to parse.
	 * \return true if the line should be ignored, false if it contains data to parse.
	 */
	bool ignore_line(string line);

	/**
	 * Determines whether the given key is a valid
	 * type that can be stored in this files header.
	 * \param key The input word to be tested.
	 * \return Whether or not is valid to be stored in the 'header' map.
	 */
	bool is_valid_header(string key);

	/**
	 * Map that associates header 'key' values to their
	 * associated data as parsed from the file.
	 * This data is created through the proc_header_line(...) method,
	 * and should not be modified once header processing is completed.
	 */
	unordered_map<string, int> header;
};

#endif

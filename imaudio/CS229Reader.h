#ifndef FORMAT_CS229_H
#define FORMAT_CS229_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "iFileReader.h"
#include "iFileWriter.h"
#include "AudioFile.h"

using namespace std;

/**
 * Implements the necessary methods of iFileReader
 * to support reading from the .cs229 file format.
 * File data can be reader from either an input file name,
 * or from an input stream (such as std::cin).
 */
class CS229Reader : public iFileReader {
public:
	CS229Reader() : current_line{0} { }

	virtual AudioFile readFile(string filename);
	virtual AudioFile readFile(istream &is, string filename = "std::cin");

private:
	/**
	 * Reads the first valid line of data from the input stream.
	 * For this file format that first line must be 'CS229', if 
	 * it is not, this method will throw an exception.
	 * \param stream
	 */
	void check_header(istream &stream);

	/**
	 * Reads lines from the input stream up until 'StartData'.
	 * This method will fill out the 'header' map with any data
	 * that it finds. If an unexpected value is found, this method will
	 * throw an exception to notify the user.
	 * \param stream
	 */
	void get_header_data(istream &stream);

	/**
	 * Reads from the current point in the input stream
	 * to the end of the stream, interpreting data as samples
	 * which are them added to the input AudioFile.
	 * This method will throw an exception when a line of
	 * data has either not enough samples, too many samples, 
	 * or an unexpected character was encountered.
	 * \param file AudioFile to add parsed samples to.
	 * \param stream Input stream to read samples from.
	 */
	void read_channel_data(AudioFile &file, istream &stream);

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
	 * Reads integers from the line and adds a sample to each chanel
	 * to the input AudioFile reference.
	 * \param file Audio file whos channels to add input samples to.
	 * \param line String representation of the line to parse samples from.
	 */
	void read_samples_from_line(AudioFile &file, string line);

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

	unsigned current_line; /**< Useful for printing out errors. */
};

#endif

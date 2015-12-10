#ifndef ABC229READER_H
#define ABC229READER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "iFileReader.h"
#include "AudioFile.h"
#include "Channel.h"

using namespace std;

/**
 * Implements the necessary methods of iFileReader
 * to support reading from the .abc229 file format.
 * File data can be reader from either an input file name,
 * or from an input stream (such as std::cin).
 */
class ABC229Reader : public iFileReader {
public:
	ABC229Reader(size_t SampleRate, size_t BitRes) : 
		sample_rate{SampleRate}, bit_res{BitRes}, current_line{0} { }

	AudioFile read_file(string filename) { return iFileReader::read_file(filename); }
	virtual AudioFile read_file(istream &is, string filename = "std::cin");

private:
	/**
	 * Checks the first line that is not a comment and makes sure that
	 * the format specifier is correct for this file format.
	 * If it is not, this method will throw an exception.
	 * \param stream Input stream to read the format specifier for.
	 */
	void check_header(istream &stream);

	/**
	 * Used by check_header to determine whether or not the input line
	 * is an appropriate format specifier for this file format.
	 * \return True if the the format specifier is correct, false otherwise.
	 */
	bool check_format(string line);

	/**
	 * This methods reads lines until the first non comment or blank line
	 * is bound, then get_tempo(...) will be called on that line.
	 * \param stream Input stream to read the header for.
	 */
	void get_header_data(istream &stream);

	/**
	 * Once the format specifier is validated, this method will
	 * read the header data to find the tempo to use. This will be stored
	 * in the 'tempo' property. If this method fails an exception will be
	 * thrown.
	 * \param line The line that should contain the tempo data.
	 */
	void get_tempo(string line);

	/**
	 * Reads all instrument data for the input stream.
	 * All data found will be added to the the 'channels' property
	 * which can then be used to generate an AudioFile.
	 * This method will throw an exception on failure.
	 * \param stream Input stream to read all Instruments from.
	 */
	void read_instruments(istream &stream);

	/**
	 * Reads the header data for next instrument.
	 * The data will be stored in the temporary variables for this class.
	 * This method will throw an exception on failure.
	 * If there are no more instruments to read, this method returns false.
	 * \param stream Input data stream to read instruments from.
	 * \return Whether there are any more instrumenst to read.
	 */
	bool get_instrument_header(istream &stream);

	/**
	 * Checks a line for an Instrument, and validates that the 
	 * instrument number associated with that is in the correct ordering.
	 * If this method fails, an error is thrown.
	 * \param line The line to parse for the instrument value.
	 */
	void get_instrument_num(string line);

	/**
	 * Converts the array of notes into a channel representing 
	 * the note array.
	 * This method will throw an exception if an invalid note is found.
	 * \param notes The array of notes to use.
	 * \return The generated channel.
	 */
	Channel get_channel_from_notes(vector<string> &notes);

	/**
	 * Determines whether or not the current line being read is a comment.
	 * \param line The line to check to see if it is a comment.
	 * \return true if the line is a comment, false otherwise.
	 */
	bool ignore_line(string line);

	/**
	 * Reads the key and value from a line.
	 * This method will throw an exception if any extra data is found.
	 * \param line The line where the data will be read from.
	 * \param key (return) They key associated with the data.
	 * \param val (return) The data held in the key.
	 */
	void get_key_and_val(string line, string &key, double &val);

	/**
	 * \param key The key to be tested
	 * \return Whether or not this is valid header for an instrument.
	 */
	bool is_valid_header(string key);

	/**
	 * \param note The note to check for the frequency of.
	 * \return The frequency of the input note.
	 */
	unsigned freq_for_note(string note);

	/**
	 * \param note The note to check for length of.
	 * \return The length of the note (relative to the Tempo).
	 */
	double length_for_note(string note);

	/**
	 * \param note The note to check as a rest.
	 * \return Whether or not the input note is a rest.
	 */
	bool is_note_rest(string note);

	/**
	 * \param note The note to be tested.
	 * \return Whether the input note is valid.
	 */
	bool is_valid_note(string note);

	/**
	 * Returns the vaue stored by the key in tmp_data, if the key is not found, 
	 * def is returned.
	 * \param key The key to use to search for data.
	 * \param def The default to use if key is note found.
	 * \return The data to use.
	 */
	double get_tmp_value(string key, double def);

	vector<Channel> channels; /**< Array of channels that will be used to generate the file */
	const size_t sample_rate; /**< Sample Rate as received by the program arguments. */
	const size_t bit_res; /**< Bit Resolution as received by the program arguments. */
	unsigned current_line; /**< Useful for printing out errors. */
	unsigned tempo; /**< Read from the header data for the file. */

	unordered_map<string, double> tmp_data; /**< Temporary hold of adsr and volume data */
	string tmp_wave; /**< Temporary hold of the sound wave data */
};

#endif

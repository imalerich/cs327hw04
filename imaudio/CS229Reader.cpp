#include <stdexcept>
#include <sstream>
#include <ctype.h>
#include <algorithm>
#include <strings.h>

#include "CS229Reader.h"

static const string invalid_format_msg = "Input file is not of type CS229!";
static const string invalid_header_msg = "Invalid header 'key' or 'data'";
static const string missing_data_msg = "Missing required header data";
static const string missing_start_data_msg = "Input file is missing 'StartData' entry";

AudioFile CS229Reader::readFile(string filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		throw invalid_argument(file_read_msg);
	}
	
	AudioFile ret = readFile(file);
	file.close();
	return ret;
}

AudioFile CS229Reader::readFile(istream &is) {
	check_header(is);
	get_header_data(is);

	try {
		return AudioFile(header.at("SAMPLERATE"), header.at("BITRES"), header.at("CHANNELS"));
	} catch (out_of_range e) {
		// only catch the exception caused by the .at()
		// not exceptions thrown by the constructor
		throw out_of_range(missing_data_msg);
	}
}

void CS229Reader::check_header(istream &file) {
	string line;
	while (getline(file, line) && ignore_line(line)) { }

	// the first valid line should be the format specifier
	if (!check_format(line)) {
		throw invalid_argument(invalid_format_msg);
	}
}

void CS229Reader::get_header_data(istream &file) {
	string line;
	while (getline(file, line)) {
		if (ignore_line(line)) {
			continue;
		}

		// return true when we are done reading the header data
		if (!proc_header_line(line)) {
			return;
		}
	}

	// EOF reached before 'StartData' throw an error
	throw invalid_argument(missing_start_data_msg);
}

bool CS229Reader::check_format(string line) {
	// TODO
	return true;
}

bool CS229Reader::proc_header_line(string line) {
	istringstream stream(line);
	string key;
	stream >> key;

	// termination case, we are finished reading data once we hit 'StartData'
	if (strcasecmp(key.c_str(), "StartData") == 0) {
		return false;
	}

	// grab the integer representatino of 'data'
	string data;
	stream >> data;
	size_t next_index; // make sure this points to the end of the string
	auto val = stoi(data, &next_index);

	if (is_valid_header(key) && next_index == data.size()) {
		transform(key.begin(), key.end(), key.begin(), ::toupper);
		header.insert({key, val});
	} else {
		throw invalid_argument(invalid_header_msg);
	}

	return true;
}

bool CS229Reader::ignore_line(string line) {
	auto found_data = false;
	for (auto c : line) {
		if (c == '#' && !found_data) {
			return true;
		} else if (!isspace(c)) {
			found_data = true;
		}
	}

	return !found_data;
}

bool CS229Reader::is_valid_header(string key) {
	static const string keys[] = { "Samples", "Bitres", "SampleRate", "Channels" };
	for (auto KEY : keys) {
		if (strcasecmp(KEY.c_str(), key.c_str()) == 0) {
			return true;
		}
	}

	return false;
}

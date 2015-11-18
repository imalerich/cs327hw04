#include <stdexcept>
#include <sstream>
#include <ctype.h>
#include <algorithm>
#include <strings.h>

#include "CS229Reader.h"

static const string invalid_format_msg = "Input file is not of type CS229!";
static const string invalid_header_msg = "Invalid Key or Data in header.";
static const string missing_data_msg = "Missing required header data";
static const string missing_start_data_msg = "Input file is missing 'StartData' entry";
static const string invalid_int = "Garbage characters found when integer expected";
static const string invalid_num_sample = "Num samples did not match 'Samples' specified in header";

AudioFile CS229Reader::readFile(string filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		throw invalid_argument(file_read_msg);
	}
	
	AudioFile ret = readFile(file, filename);
	file.close();
	return ret;
}

AudioFile CS229Reader::readFile(istream &is, string filename) {
	try {
		check_header(is);
		get_header_data(is);
	} catch (exception e) {
		// intercept the exception for a second to print the line number
		cerr << filename << " : exception occured at line : " << current_line << endl;
		// forward the exception we found
		throw;
	}

	try {
		AudioFile ret = AudioFile(filename, ".cs229",
				header.at("SAMPLERATE"), header.at("BITRES"), header.at("CHANNELS"));
		read_channel_data(ret, is);
		return ret;

	} catch (out_of_range e) {
		// only catch the exception caused by the .at()
		// not exceptions thrown by the constructor
		throw out_of_range(missing_data_msg);
	} catch (exception e) {
		// intercept any other exception
		cerr << filename << ": exception occured at line : " << current_line << endl;
		// forward the exception we found
		throw;
	}
}

void CS229Reader::check_header(istream &stream) {
	string line;
	while (getline(stream, line) && ignore_line(line)) { current_line++; }

	// the first valid line should be the format specifier
	current_line++;
	if (!check_format(line)) {
		throw invalid_argument(invalid_format_msg);
	}
}

void CS229Reader::get_header_data(istream &stream) {
	string line;
	while (getline(stream, line)) {
		current_line++;

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

void CS229Reader::read_channel_data(AudioFile &file, istream &stream) {
	string line;
	while (getline(stream, line)) {
		current_line++;

		if (ignore_line(line)) {
			continue;
		}

		read_samples_from_line(file, line);
	}

	try {
		// check if the input file expected a particular number of samples
		auto samples = header.at("SAMPLES");
		if (file.get_num_samples() != (unsigned)samples) {
			throw invalid_argument(invalid_num_sample);
		}

	} catch (out_of_range e) {
		// do nothing, the input file did not specify how many samples were expected
	}
}

bool CS229Reader::check_format(string line) {
	istringstream stream(line);
	string key;
	stream >> key;

	return strcasecmp(key.c_str(), "CS229") == 0;
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

	if (next_index != data.length() || !data.length()) {
		throw invalid_argument(invalid_int);
	}

	if (is_valid_header(key)) {
		transform(key.begin(), key.end(), key.begin(), ::toupper);
		header.insert({key, val});
	} else {
		throw invalid_argument(invalid_header_msg);
	}

	string extra;
	stream >> extra;
	if (extra.length() > 0 && extra[0] != '#') {
		throw invalid_argument(invalid_header_msg + " -> " + extra);
	}

	return true;
}

void CS229Reader::read_samples_from_line(AudioFile &file, string line) {
	istringstream stream(line);
	string extra;
	size_t next_index; // make sure this points to the end of the string

	// read a sample for each channel in the AudioFile
	for (auto i = 0; i < (int)file.get_num_channels(); i++) {
		string data;
		stream >> data;
		auto val = stoi(data, &next_index);

		if (next_index != data.length() || !data.length()) {
			throw invalid_argument(invalid_int);
		}

		file[i].push_sample(val);
	}

	// make sure there isn't any extra garbage data
	stream >> extra;
	if (extra.length() > 0 && extra[0] != '#') {
		throw invalid_argument(invalid_int + " -> " + extra);
	}
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

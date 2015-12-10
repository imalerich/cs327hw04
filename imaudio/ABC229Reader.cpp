#include <stdexcept>
#include <sstream>
#include <ctype.h>
#include <algorithm>
#include <strings.h>
#include "func/SinWave.h"
#include "func/PulseWave.h"
#include "func/SawToothWave.h"
#include "func/TriangleWave.h"
#include "func/AdsrEnvelope.h"

#include "ABC229Reader.h"

AudioFile ABC229Reader::read_file(istream &is, string filename) {
	check_header(is);
	get_header_data(is);
	read_instruments(is);
	
	AudioFile ret = AudioFile(filename, ".abc229", sample_rate, bit_res, channels.size());
	for (auto i = 0; i < (int)channels.size(); i++) {
		ret[i].append(channels[i]);
	}

	ret.make_valid();
	return ret;
}

void ABC229Reader::check_header(istream &stream) {
	string line;
	while (getline(stream, line) && ignore_line(line)) { current_line++; }

	// the first valid line should be the format specifier
	current_line++;
	if (!check_format(line)) {
		throw invalid_argument("Input file is not of type ABC229!");
	}
}

bool ABC229Reader::check_format(string line) {
	istringstream stream(line);
	string key;
	stream >> key;

	string extra;
	stream >> extra;
	if (extra.length() > 0 && extra[0] != '%') {
		throw invalid_argument("Extra data found after header");
	}

	return strcasecmp(key.c_str(), "ABC229") == 0;
}

void ABC229Reader::get_header_data(istream &stream) {
	string line;
	while (getline(stream, line) && ignore_line(line)) { current_line++; }

	current_line++;
	get_tempo(line);
}

void ABC229Reader::get_tempo(string line) {
	string key;
	double val;

	get_key_and_val(line, key, val);

	if (strcasecmp(key.c_str(), "Tempo") != 0) {
		// invalid key found
		throw invalid_argument("Invalid specifier found where 'Tempo' was expected");
	}

	tempo = (unsigned)val;
}

bool ABC229Reader::ignore_line(string line) {
	auto found_data = false;
	for (auto c : line) {
		if (c == '%' && !found_data) {
			return true;
		} else if (!isspace(c)) {
			found_data = true;
		}
	}

	return !found_data;
}

void ABC229Reader::read_instruments(istream &stream) {
	while (get_instrument_header(stream)) { }
}

bool ABC229Reader::get_instrument_header(istream &stream) {
	string line;
	string key;
	double val;
	bool end_found = true;
	bool data_found = false;

	tmp_data.clear();
	tmp_wave = "";

	while (true) {
		if (!getline(stream, line)) { return false; }
		current_line++;
		if (!ignore_line(line)) { break; }
	}

	get_instrument_num(line);

	// read all the header data we need
	while (!(end_found = !getline(stream, line))) {
		current_line++;

		if (ignore_line(line)) {
			continue;
		}

		get_key_and_val(line, key, val);
		data_found = true;
		
		// terminate once we find "Score"
		if (strcasecmp(key.c_str(), "Score") == 0) {
			break;
		}

		// check the key we got back and insert the data to our temporary header array
		if (is_valid_header(key)) {
			try {
				tmp_data.at(key);
				throw invalid_argument("Data already found for header: " + key);
			} catch (out_of_range e) {
				tmp_data.insert({key, val});
			}
		} else {
			throw invalid_argument("Invalid key found while searching for header data");
		}
	}

	// the next line must be [
	if ((end_found && data_found) || (!end_found && !getline(stream, line))) {
		throw invalid_argument("Unexpected termination of file.");
	}
	
	if (end_found) {
		return true;
	}

	current_line++;
	if (strcasecmp(line.c_str(), "[") != 0) {
		throw invalid_argument("Excpected '[' after 'Score'.");
	}

	// the next few lines are the note data
	vector<string> notes;
	while (getline(stream, line)) {
		current_line++;

		if (ignore_line(line)) {
			continue;
		}

		if (strcasecmp(line.c_str(), "]") == 0) {
			channels.push_back(get_channel_from_notes(notes));
			return true;
		}

		// read each value
		istringstream data(line);
		string word;

		while (data >> word) {
			if (!is_valid_note(word)) {
				throw invalid_argument("Attempting to read invalid note: " + word);
			}

			notes.push_back(word);
		}
	}

	return false;
}

bool ABC229Reader::is_valid_note(string note) {
	// check for a valid letter
	char val = toupper(note[0]);
	if (!((val >= 'A' && val <= 'G') || (val == 'Z'))) {
		return false;
	} else {
		// remove the note
		note = note.substr(1, note.size() - 1);
	}

	// check for a sharp and remove it
	if (note.front() == '#') {
		// B, C, and Z do not allow sharps
		if (val == 'B' || val == 'E' || val == 'Z') {
			return false;
		}

		note = note.substr(1, note.size() - 1);
	}

	// check for a length value and remove it
	int length_chars = 0;
	for (int i = note.length() - 1; i >= 0; i-- ) {
		if ((note[i] >= '0' && note[i] <= '9') || note[i] == '.') {
			length_chars++;
		} else {
			break;
		}
	}

	if (length_chars > 0) {
		note = note.substr(0, note.length() - length_chars);
	}

	auto up_octave_count = 0;
	auto down_octave_count = 0;

	for (auto i = 0; i < (int)note.length(); i++) {
		if (note[i] == ',') {
			down_octave_count++;
		} else if (note[i] == '\'') {
			up_octave_count++;
		} else {
			return false;
		}
	}

	// only one direction of octave jumps is allowed
	if (up_octave_count > 0 && down_octave_count > 0) {
		return false;
	}

	// octave jumps are not allowed on rests
	if ((up_octave_count > 0 || down_octave_count > 0) && val == 'Z') {
		return false;
	}
	
	return (up_octave_count < 3 && down_octave_count < 3);
}

unsigned ABC229Reader::freq_for_note(string note) {
	if (toupper(note[0]) == 'Z') return 0;

	string note_str;
	note_str += toupper(note[0]);
	if (note[1] == '#') note_str += '#';
	unsigned freq = 440;

	auto up_octave_count = 0;
	auto down_octave_count = 0;

	for (auto c : note) {
		if (c == ',') { down_octave_count++; }
		else if (c == '\'') { up_octave_count++; }
	}

	// modify the octave relative to the frequency
	freq *= pow(2, up_octave_count - down_octave_count);

	// get the 0 based index for the note
	unsigned index;
	static const string keys[] = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
	for (index = 0; strcasecmp(keys[index].c_str(), note_str.c_str()) != 0; index++);

	// apply the offset from the root 'A'
	double s = pow(2, 1.0/12);
	freq *= pow(s, index);

	return freq;
}

double ABC229Reader::length_for_note(string note) {
	string length;
	for (auto i = 0; i < (int)note.length(); i++ ) {
		if ((note[i] >= '0' && note[i] <= '9') || note[i] == '.') {
			length += note[i];
		}
	}

	if (!length.size()) {
		return 1;
	}
	
	double l = stod(length);
	return l;
}

bool ABC229Reader::is_note_rest(string note) {
	return toupper(note[0]) == 'Z';
}

Channel ABC229Reader::get_channel_from_notes(vector<string> &notes) {
	Channel ret = Channel(bit_res);
	double amplitude = ((int)pow(2, bit_res) / 2) - 1;
	unsigned sample_per_note = sample_rate / (tempo / 60.0);
	double volume = get_tmp_value("Volume", 1.0);
	double attack = get_tmp_value("Attack", 0.0);
	double decay = get_tmp_value("Decay", 0.0);
	double sustain = get_tmp_value("Sustain", 1.0);
	double release = get_tmp_value("Release", 0.0);
	double pulsefrac = get_tmp_value("PulseFrac", 0.5);
	double octave = get_tmp_value("Octave", 0.0);

	for (auto note : notes) {
		double freq = freq_for_note(note);
		freq *= pow(2, octave);
		SinWave s = SinWave(amplitude, freq);
		iWaveform * wave = NULL;

		// create the waveform for this note
		if (strcasecmp("Triangle", tmp_wave.c_str()) == 0) {
			wave = new TriangleWave(amplitude, freq);
		} else if (strcasecmp("Sine", tmp_wave.c_str()) == 0) {
			wave = new SinWave(amplitude, freq);
		} else if (strcasecmp("Sawtooth", tmp_wave.c_str()) == 0) {
			wave = new SawToothWave(amplitude, freq);
		} else if (strcasecmp("PulseWave", tmp_wave.c_str()) == 0) {
			wave = new PulseWave(amplitude, freq, pulsefrac);
		} else {
			throw invalid_argument("Expected a waveform when generating wave");
		}

		// push the samples for this note
		for (auto i = 0; i < (int)(sample_per_note * length_for_note(note)); i++) {
			AdsrEnvelope env = AdsrEnvelope(attack, decay, sustain, release, length_for_note(note) * (tempo / 60.0));

			if (is_note_rest(note)) {
				ret.push_sample(0);
			} else {
				double time = i / (double)sample_rate;
				double adsr = env.sample_at_time(time);
				double sample = wave->sample_at_time(time);
				ret.push_sample(volume * sample * adsr);
			}
		}

		delete wave;
	}

	return ret;
}

double ABC229Reader::get_tmp_value(string key, double def) {
	double ret = def;
	try {
		ret = tmp_data.at(key);
	} catch (out_of_range e) { 
		return def;
	}

	return ret;
}

void ABC229Reader::get_instrument_num(string line) {
	string key;
	double val;
	get_key_and_val(line, key, val);

	if (strcasecmp(key.c_str(), "Instrument") != 0) {
		throw invalid_argument("Extra data found where an 'Instrument' was expected.");
	}

	if ((unsigned)val != channels.size()) {
		throw invalid_argument("Instruments numbers are required to be in order.");
	}
}

void ABC229Reader::get_key_and_val(string line, string &key, double &val) {
	istringstream stream(line);
	stream >> key;

	if (strcasecmp(key.c_str(), "Score") == 0) {
		string extra;
		stream >> extra;
		if (extra.length() > 0 && extra[0] != '%') {
			throw invalid_argument("Extra data found after header");
		}

		val = 0.0;
		return;
	}

	if (strcasecmp(key.c_str(), "Waveform") == 0) {
		string wave;
		stream >> wave;
		tmp_wave = wave;

		string extra;
		stream >> extra;
		if (extra.length() > 0 && extra[0] != '%') {
			throw invalid_argument("Extra data found after header");
		}

		return;
	}

	bool isInt = strcasecmp(key.c_str(), "Tempo") == 0 || strcasecmp(key.c_str(), "Instrument") == 0;

	// the next value should be an integer describing the Tempo
	string data;
	stream >> data;
	size_t next_index; // make sure this points to the end of the string
	val = isInt ? (double)stol(data, &next_index) : stod(data, &next_index);

	if (next_index != data.length() || !data.length()) {
		throw invalid_argument("Invalid data where integer expected while reading 'Tempo'");
	}

	string extra;
	stream >> extra;
	if (extra.length() > 0 && extra[0] != '%') {
		throw invalid_argument("Extra data found after header");
	}
}

bool ABC229Reader::is_valid_header(string key) {
	static const string keys[] = { "Waveform", "Volume", "Attack", "Decay", "Sustain", "Release", "PulseFrac", "Octave" };
	for (auto KEY : keys) {
		if (strcasecmp(KEY.c_str(), key.c_str()) == 0) {
			return true;
		}
	}

	return false;
}

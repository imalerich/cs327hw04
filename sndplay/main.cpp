#include <ABC229Reader.h>
#include <CS229Writer.h>
#include <WavWriter.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

void print_help();
long get_long_from_string(string data);

static int output_wav = 0;
static size_t bit_depth = 0;
static size_t sample_rate = 0;
static int mute_index = -1;

int main(int argc, char ** argv) {
	static struct option long_options[] = {
		{ "help", 0, 0, 'h' },
		{ "output", required_argument, 0, 'o' },
		{ "wav", 0, &output_wav, 1 },
		{ "bits", required_argument, 0, 'b' },
		{ "sr", required_argument, 0, 's' },
		{ "mute", required_argument, 0, 'm' },
		{ 0, 0, 0, 0 }
	};

	char c = 0;
	int option_index = 0;
	const char * file_name = NULL;
	while ((c = getopt_long(argc, argv, "ho:wb:s:m:012", long_options, &option_index)) != -1) {
		switch (c) {
		case 'o':
			file_name = optarg;
			break;

		case 'b':
			bit_depth = (size_t)get_long_from_string(string(optarg));
			break;

		case 's':
			sample_rate = (size_t)get_long_from_string(string(optarg));
			break;

		case 'm':
			mute_index = (size_t)get_long_from_string(string(optarg));
			break;

		case 'h':
			print_help();
			return 0;
		}
	}

	unsigned extra_params = argc - optind;
	if (!bit_depth || !sample_rate || extra_params > 1) {
		print_help();
		return 1;
	}

	AudioFile output = extra_params ? ABC229Reader(sample_rate, bit_depth).read_file(string(argv[optind])) :
		ABC229Reader(sample_rate, bit_depth).read_file(cin);

	if (mute_index > -1) {
		output.mute_channel(mute_index);
	}

	iFileWriter * writer = nullptr;
	if (output_wav == 0) {
		writer = new WavWriter();
	} else {
		writer = new CS229Writer();
	}

	if (file_name) {
		writer->write_file(output, file_name);
	} else {
		writer->write_file(output, cout);
	}

	delete writer;
}

long get_long_from_string(string data) {
	try {
		size_t next_index;
		auto val = stol(data, &next_index);

		if (next_index != data.length() || !data.length()) {
			throw invalid_argument("");
		}

		return val;

	} catch (exception e) {
		throw invalid_argument("paremter does not contain a valid long");
	}
}

void print_help() {
	cout << "Usage: sndplay [options] [file]" << endl;
	cout << "Options:" << endl;
	cout << "  -h --help\tDisplay this information" << endl;
	cout << "  -o --output=<file>\tOutput to <file> instead of the standard output" << endl;
	cout << "  -w --wav\tOutput the file in .wav format and not .cs229" << endl;
	cout << "  -s --sr\tSample Rate to use for the output .cs229" << endl;
	cout << "  -b --bits\t Bit Depth to use for the output .cs229" << endl;
	cout << "  -m --mute\tIndex of an Instrument to be muted in output .cs229" << endl;
	cout << endl;
	cout << "This program reads in a file of format .abc229 and converts it to the .cs229 format." << endl;
	cout << "If there is no file specified for input, this program will read from the standard input." << endl;
}

#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include <CS229Reader.h>
#include <CS229Writer.h>
#include <WavWriter.h>
#include <AudioFile.h>
#include <flags.h>

using namespace std;
double get_scalar(char * str);
void print_help();

static int output_wav = 0;

int main(int argc, char ** argv) {
	static struct option long_options[] = {
		{ "help", 0, 0, 'h' },
		{ "output", required_argument, 0, 'o' },
		{ "wav", 0, 0, 'w' },
		{ "nonstrict", 0, 0, 'n' },
		{ 0, 0, 0, 0 }
	};

	char c = 0;
	int option_index = 0;
	const char * file_name = NULL;
	while ((c = getopt_long(argc, argv, "hwo:n012", long_options, &option_index)) != -1) {
		switch (c) {
		case 'o':
			file_name = optarg;
			break;

		case 'w':
			output_wav = 1;
			break;

		case 'n':
			strict_data = false;
			break;

		case 'h':
			print_help();
			return 0;
		}
	}
		
	unsigned arg_count = argc - optind;
	if (arg_count == 0 || arg_count % 2 == 1) {
		print_help();
		return 1;
	}

	auto output = CS229Reader().read_file(string(argv[optind])) * get_scalar(argv[optind + 1]);

	for (auto i = optind + 2; i < argc; i+=2) {
		output = output + (CS229Reader().read_file(string(argv[i])) * get_scalar(argv[i+1]));
	}

	iFileWriter * writer = nullptr;
	if (output_wav == 1) {
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

double get_scalar(char * str) {
	size_t next_index = 0;
	auto data = string(str);
	auto scalar = stod(data, &next_index);
	if (next_index != data.length() || !data.length()) {
		throw invalid_argument("invalid scalar in input");
	}

	if (scalar < -10.0 || scalar > 10.0) {
		throw invalid_argument("scalar must exist within range (-10.0, 10.0)");
	}

	return scalar;
}

void print_help() {
	cout << "Usage: sndmix [options] file mult..." << endl;
	cout << "Options:" << endl;
	cout << "  -h --help\tDisplay this information" << endl;
	cout << "  -o --ouput=<file>\tOutput to <file> instead of standard output" << endl;
	cout << "  -w --wav\tOutput filees to the .wav format instead of .cs229" << endl;
	cout << "  -n --nonstrict\tFile combinations will be much more lenient." << endl;
	cout << endl;
	cout << "This program reads all sound files passed as arguments, and \"mixes\"" << endl; 
	cout << "them into a single sound file." << endl;
}

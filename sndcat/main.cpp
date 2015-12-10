#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <CS229Reader.h>
#include <CS229Writer.h>
#include <WavWriter.h>
#include <AudioFile.h>
#include <flags.h>

using namespace std;
void print_help();

static int output_wav = 0;

int main(int argc, char ** argv) {
	static struct option long_options[] = {
		{ "help", 0, 0, 'h' },
		{ "output", required_argument, 0, 'o' },
		{ "wav", 0, &output_wav, 1 },
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

		case 'n':
			strict_data = false;
			break;

		case 'h':
			print_help();
			return 0;
		}
	}

	if (optind >= argc) {
		print_help();
		return 1;
	}

	AudioFile output = CS229Reader().read_file(string(argv[optind]));

	for (auto i = optind + 1; i < argc; i++) {
		AudioFile add = CS229Reader().read_file(string(argv[i]));
		output = output.concat(add);
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

void print_help() {
	cout << "Usage: sndcat [options] file..." << endl;
	cout << "Options:" << endl;
	cout << "  -h --help\tDisplay this information" << endl;
	cout << "  -o --output=<file>\tOutput to <file> instead of the standard output" << endl;
	cout << "  -w --wav\tOutput filees to the .wav format instead of .cs229" << endl;
	cout << "  -n --nonstrict\tFile combinations will be much more lenient." << endl;
	cout << endl;
	cout << "This program reads all sound files passed as arguments, and writes a single sound file that is" << endl;
	cout << "the concatenation of the inputs. If no files are passed as arguments, then the program should" << endl;
	cout << "read from standard input." << endl;
}

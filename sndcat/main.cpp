#include <iostream>
#include <vector>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <CS229Reader.h>
#include <CS229Writer.h>
#include <AudioFile.h>

using namespace std;
void print_help();

int main(int argc, char ** argv) {
	static struct option long_options[] = {
		{ "help", 0, 0, 'h' },
		{ "output", required_argument, 0, 'o' },
		{ "wav", 0, 0, 'w' },
		{ 0, 0, 0, 0 }
	};

	char c = 0;
	int option_index = 0;
	const char * file_name = NULL;
	while ((c = getopt_long(argc, argv, "hwo:012", long_options, &option_index)) != -1) {
		switch (c) {
		case 'o':
			file_name = optarg;
			break;

		case 'w':
			cout << "wav files currently not supported" << endl;
			return 0;

		case 'h':
			print_help();
			return 0;
		}
	}

	if (optind >= argc) {
		print_help();
		return 1;
	}

	AudioFile output = CS229Reader().readFile(string(argv[optind]));

	for (auto i = optind + 1; i < argc; i++) {
		AudioFile add = CS229Reader().readFile(string(argv[i]));
		output = output.concat(add);
	}

	if (file_name) {
		CS229Writer().writeFile(output, file_name);
	} else {
		CS229Writer().writeFile(output, cout);
	}
}

void print_help() {
	cout << "Usage: sndcat [options] file..." << endl;
	cout << "Options:" << endl;
	cout << "  -h --help\tDisplay this information" << endl;
	cout << "  -o --output<file>\tOutput to <file> instead of the standard output" << endl;
	cout << "  -w --wav\tInput files are in .wav format not .cs229" << endl;
	cout << endl;
	cout << "This program reads all sound files passed as arguments, and writes a single sound file that is" << endl;
	cout << "the concatenation of the inputs. If no files are passed as arguments, then the program should" << endl;
	cout << "read from standard input." << endl;
}

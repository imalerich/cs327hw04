#include <CS229Reader.h>
#include <ABC229Reader.h>
#include <WavReader.h>
#include <CS229Writer.h>
#include <WavWriter.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

#define TMP_FILE ".cin"

void create_tmp_file();
void remove_tmp_file();
void output_file(iFileWriter * writer, AudioFile &file, const char * file_name);
void print_help();

int main(int argc, char ** argv) {
	static struct option long_options[] = {
		{ "help", 0, 0, 'h' },
		{ "output", required_argument, 0, 'o' },
		{ 0, 0, 0, 0 }
	};

	char c = 0;
	const char * file_name = nullptr;
	int option_index = 0;
	while ((c = getopt_long(argc, argv, "ho:012", long_options, &option_index)) != -1) {
		switch (c) {
		case 'o':
			file_name = optarg;
			break;

		case 'h': print_help();
			return 0; }
	}

	// only one extra param is allowed (the file_name)
	unsigned extra_params = argc - optind;
	if (extra_params > 1) {
		print_help();
		return 1;
	}

	if (extra_params == 0) {
		create_tmp_file();
	}

	try {
		AudioFile file = extra_params ? CS229Reader().read_file(string(argv[optind])) :
			CS229Reader().read_file(TMP_FILE);
		WavWriter writer = WavWriter();
		output_file(&writer, file, file_name);
		remove_tmp_file();
		return 0;
	} catch (exception e) { }

	// if that fails try to read it as a .wav
	try {
		AudioFile file = extra_params ? WavReader().read_file(string(argv[optind])) :
			WavReader().read_file(TMP_FILE);
		CS229Writer writer = CS229Writer();
		output_file(&writer, file, file_name);
		remove_tmp_file();
		return 0;
	} catch (exception e) { }

	try {
		AudioFile file = extra_params ? ABC229Reader(48000, 32).read_file(string(argv[optind])) :
			ABC229Reader(48000, 32).read_file(TMP_FILE);
		cerr << "Input file was of type .abc229, using a sample rate of 48000 and bit depth of 32." << endl;
		WavWriter writer = WavWriter();
		output_file(&writer, file, file_name);
		remove_tmp_file();
		return 0;
	} catch (exception e) { }

	// if that also fails print an error
	cerr << "error: Failed to read the input file." << endl;
}

void create_tmp_file() {
	// read all of the input into a file (as we will need to use it more than once)
	string line;
	ofstream output;
	output.open(TMP_FILE);
	while (getline(cin, line)) { output << line << endl; }
	output.close();
}

void remove_tmp_file() {
	remove(TMP_FILE);
}

void output_file(iFileWriter * writer, AudioFile &file, const char * file_name) {
	if (file_name) {
		writer->write_file(file, file_name);
	} else {
		writer->write_file(file, cout);
	}
}

void print_help() {
	cout << "Usage: sndcvt [options] [file]" << endl;
	cout << "Options:" << endl;
	cout << "  -h --help\tDisplay this information" << endl;
	cout << "  -o --output\tSpecifies the name of the file this program should write to (standard output if ommitted)." << endl;
	cout << endl;
	cout << "This program reads the [file] argument (or from standard input if that parameter is ommitted." << endl;
	cout << "The program will then convert that file to a new format depending on its current format." << endl;
	cout << "The new file will be output to the standard output or the file specified by '-o' if available." << endl;
	cout << "The conversion performed is defined as follow: cs229->wav; wav->cs229; abc229->wav." << endl;
}

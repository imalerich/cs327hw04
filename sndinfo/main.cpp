#include <CS229Reader.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;
void print_help();

int main(int argc, char ** argv) {
	static struct option long_options[] = {
		{ "help", 0, 0, 'h' },
		{ 0, 0, 0, 0 }
	};

	char c = 0;
	int option_index = 0;
	while ((c = getopt_long(argc, argv, "h012", long_options, &option_index)) != -1) {
		switch (c) {
		case 'h':
			print_help();
			return 0;
		}
	}

	if (argc == 2) {
		cout << CS229Reader().readFile(string(argv[1])) << endl;
	} else if (argc == 1) {
		cout << CS229Reader().readFile(cin, "std::cin") << endl;
	} else {
		print_help();
	}
}

void print_help() {
	cout << "Usage: sndinfo [options] [file]" << endl;
	cout << "Options:" << endl;
	cout << "  -h --help\tDisplay this information" << endl;
	cout << endl;
	cout << "If a [file] is given, sound information for that file will be written to the standard output." << endl;
	cout << "If a [file] is not given, sound information will be read from the standard intput." << endl;
}

#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include <CS229Reader.h>
#include <CS229Writer.h>
#include <AudioFile.h>

using namespace std;
double get_scalar(char * str);

int main(int argc, char ** argv) {
	if (argc == 1 || argc % 2 == 0) {
		cout << "Please enter args in format <file> <mult>..." << endl;
		return 0;
	}

	cout << "reading file: " << argv[1] << endl;
	auto output = CS229Reader().readFile(string(argv[1])) * get_scalar(argv[2]);

	for (auto i=3; i<argc; i+=2) {
		cout << "reading file: " << argv[i] << endl;
		output = output + (CS229Reader().readFile(string(argv[i])) * get_scalar(argv[i+1]));
	}

	CS229Writer().writeFile(output, cout);
}

double get_scalar(char * str) {
	size_t next_index = 0;
	auto data = string(str);
	auto scalar = stod(data, &next_index);
	if (next_index != data.length() || !data.length() || scalar < -10.0 || scalar > 10.0) {
		throw invalid_argument("invalid scalar in input");
	}

	return scalar;
}

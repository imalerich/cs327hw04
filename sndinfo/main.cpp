#include <iostream>
#include <FormatCS229.h>
#include <string>

using namespace std;

int main(int argc, char ** argv) {
	if (argc > 1) {
		FormatCS229 format;
		format.readFile(string(argv[1]));
	} else {
		cout << "please enter a valid .cs229 file as input\n";
	}
}

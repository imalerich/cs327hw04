#include <iostream>
#include <CS229Reader.h>
#include <string>

using namespace std;

int main(int argc, char ** argv) {
	CS229Reader format;
	if (argc > 1) {
		format.readFile(string(argv[1]));
		cout << "valid" << endl;
	} else {
		format.readFile(cin);
		cout << "valid" << endl;
	}
}

#include <iostream>
#include <vector>

#include <CS229Reader.h>
#include <CS229Writer.h>
#include <AudioFile.h>

using namespace std;

int main(int argc, char ** argv) {
	if (argc == 1) {
		cerr << "Please input a list of audio files" << endl;
		return 1;
	}

	AudioFile output = CS229Reader().readFile(string(argv[1]));

	for (auto i = 2; i < argc; i++) {
		AudioFile add = CS229Reader().readFile(string(argv[i]));
		output = output.concat(add);
	}

	CS229Writer().writeFile(output, cout);
}

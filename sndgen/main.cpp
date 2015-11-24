#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>

#include <CS229Reader.h>
#include <CS229Writer.h>
#include <AudioFile.h>
#include <func/SinWave.h>
#include <func/TriangleWave.h>
#include <func/SawToothWave.h>
#include <func/PulseWave.h>
#include <func/AdsrEnvelope.h>
#include <flags.h>

using namespace std;

void print_help();
double get_double_from_string(string data);
long get_long_from_string(string data);

static int sin;
static int triangle;
static int sawtooth;
static int pulse;
static double pulse_ratio = -1.0;

static size_t bit_res = 0;
static size_t sample_rate = 0;

static double frequency = 1.0;
static double time_duration = 1.0;
static double volume = 1.0;

static bool use_adsr = false;
static double a = -1.0;
static double d = -1.0;
static double s = -1.0;
static double r = -1.0;

int main(int argc, char ** argv) {
	static struct option long_options[] = {
		{ "help",		no_argument,		0,			'h' },
		{ "output",		required_argument,	0,			'o' },
		{ "nonstrict",	no_argument,		0,			'n' },
		{ "bits",		required_argument,	0,			0 },
	 	{ "sr",			required_argument,	0,			0 },
		{ "sin",		no_argument,		&sin,		1 },
		{ "triangle",	no_argument,		&triangle,	1 },
		{ "sawtooth",	no_argument,		&sawtooth,	1 },
		{ "pulse",		no_argument,		&pulse,		1 },
		{ "pf",			required_argument,	0,			'p' },
		{ 0, 0, 0, 0 }
	};

	char c = 0;
	int option_index = 0;
	const char * file_name = NULL;
	const char * name = NULL;

	// parse all arguments
	while ((c = getopt_long(argc, argv, "ho:nf:t:v:a:d:s:r:p:012", long_options, &option_index)) != -1) {
		switch (c) {
		case 0:
			name = long_options[option_index].name;
			if (strcmp(name, "bits") == 0) {
				bit_res = (size_t)get_long_from_string(string(optarg));
			} else if (strcmp(name, "sr") == 0) {
				sample_rate = (size_t)get_long_from_string(string(optarg));
			}

			break;

		case 'o':
			file_name = optarg;
			break;

		case 'n':
			strict_data = false;
			break;

		case 'f':
			frequency = get_double_from_string(string(optarg));
			break;

		case 't':
			time_duration = get_double_from_string(string(optarg));
			break;

		case 'v':
			volume = get_double_from_string(string(optarg));
			break;

		case 'a':
			use_adsr = true;
			a = get_double_from_string(string(optarg));
			break;

		case 'd':
			use_adsr = true;
			d = get_double_from_string(string(optarg));
			break;

		case 's':
			use_adsr = true;
			s = get_double_from_string(string(optarg));
			break;

		case 'r':
			use_adsr = true;
			r = get_double_from_string(string(optarg));
			break;

		case 'p':
			pulse_ratio = get_double_from_string(string(optarg));
			break;

		case 'h':
			print_help();
			return 0;
		}
	}
		
	// check for unwated parameters
	if (argc - optind > 0 || argc == 1) {
		print_help();
		return 1;
	}

	// validate the state of the program
	if (!bit_res || !sample_rate) {
		throw invalid_argument("non-zero bit_res and sample_rate are required to create a new file.");
	}

	if (volume < 0.0 || volume > 1.0) {
		throw invalid_argument("-v requires a value in range [0.0, 1.0]");
	}

	if (pulse && (pulse_ratio < 0 || pulse_ratio > 1.0)) {
		throw invalid_argument("--pulse option required a valid pulse_ratio specified with --pf or -p");
	}

	// check if we are going to use an adsr envelope
	if (use_adsr) {
		if (a < 0 || d < 0 || s < 0 || r < 0) {
			throw invalid_argument("if using an adsr envelope all options ('-a -d -s -r) must be specified");
		}

		if (s < 0 || s > 1.0) {
			throw invalid_argument("sustain time of adsr envelope (option -s) must be within the range [0.0, 1.0]");
		}
	}

	iWaveform * wave = NULL;
	switch (sin * 1000 + triangle * 100 + sawtooth * 10 + pulse * 1) {
	case 1000:
		wave = new SinWave(100, frequency);
		break;

	case 100:
		wave = new TriangleWave(100, frequency);
		break;

	case 10:
		wave = new SawToothWave(100, frequency);
		break;

	case 1:
		wave = new PulseWave(100, frequency, pulse_ratio);
		break;

	default:
		throw invalid_argument("sndgen requires one of the following arguments is allowed: '--sin' '--triangle' '--sawtooth' '--pulse'");
		break;
	}

	// generate the file and output
	AudioFile file = ((iFunction *)wave)->generateAudioFile(sample_rate, time_duration, bit_res);

	if (use_adsr) {
		AdsrEnvelope adsr = AdsrEnvelope(a, d, s, r, time_duration);
		file = adsr * file;
	}

	if (file_name) {
		CS229Writer().writeFile(file, file_name);
	} else {
		CS229Writer().writeFile(file, cout);
	}
	
	// wave is only NULL if the default: exception is thrown
	delete wave;
}

void print_help() {
	cout << "Usage: sndgen [options]" << endl;
	cout << "Options:" << endl;
	cout << "  -h --help\tDisplay this information" << endl;
	cout << "  -o --ouput<file>\tOutput to <file> instead of standard output" << endl;
	cout << "  -n --nonstrict\tFile combinations will be much more lenient." << endl;
	cout << "  --bits=<n>\tUse a bit resolution of <n> in the output file (Required)." << endl;
	cout << "  --sr=<n>\tUse a sample rate of <n> in the output file (Required)." << endl;
	cout << "  -f <n>\tUse a frequency of <n> for the input wave (default of 1.0)." << endl;
	cout << "  -t <n>\tTotal duration of output file (default of 1.0) in seconds." << endl;
	cout << "  -v <n>\tPeak volume of <n> (must be within range of [0.0, 1.0]) (default of 1.0)." << endl;
	cout << "  -a <n>\tAttack time of <n> for input asdr envelope (must be greater than 0.0) (ignored if -a -d -s or -r are never set)." << endl;
	cout << "  -d <n>\tDecay time of <n> for input asdr envelope (must be greater than 0.0) (ignored if -a -d -s or -r are never set)." << endl;
	cout << "  -s <n>\tSustain time of <n> for input asdr envelope (must be within range [0.0, 1.0]) (ignored if -a -d -s or -r are never set)." << endl;
	cout << "  -r <n>\tRelease time of <n> for input asdr envelope (must be greater than 0.0) (ignored if -a -d -s or -r are never set)." << endl;
	cout << "  --sin\tGenerate a sin wave." << endl;
	cout << "  --triangle\tGenerate a triangle wave." << endl;
	cout << "  --pulse\tGenerate a pulse wave (requires --pf)." << endl;
	cout << "  -p --pf=<n>\tFraction of the time the pulse wave is 'up', required for --pulse, ignored otherwise (must be within rage of [0.0, 1.0])." << endl;
	cout << endl;
	cout << "Produces a sound of the specified frequency and waveform, using a simple ADSR envelope." << endl;
}

double get_double_from_string(string data) {
	size_t next_index;
	auto val = stod(data, &next_index);

	if (next_index != data.length() || !data.length()) {
		throw invalid_argument("paremter does not contain a valid double");
	}

	return val;
}

long get_long_from_string(string data) {
	size_t next_index;
	auto val = stol(data, &next_index);

	if (next_index != data.length() || !data.length()) {
		throw invalid_argument("paremter does not contain a valid long");
	}

	return val;
}

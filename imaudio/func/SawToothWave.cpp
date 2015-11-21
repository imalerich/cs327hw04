#include <math.h>
#include "SawToothWave.h"

long SawToothWave::sampleAtTime(double time) {
	double slope = 2 * iWaveform::amplitude;
	time = time * iWaveform::frequency; // adjust time to frequency
	time = time - (int)time; // remap to range [0, 1)
	
	return -iWaveform::amplitude + slope * time;
}

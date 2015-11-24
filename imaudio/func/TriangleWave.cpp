#include <math.h>
#include "TriangleWave.h"

double TriangleWave::sampleAtTime(double time) {
	double slope = 2 * iWaveform::amplitude / 0.5;
	time = time * iWaveform::frequency; // adjust time to frequency
	time = time - (int)time; // remap to range [0, 1)
	
	if (time < 0.5) {
		return -iWaveform::amplitude + slope * time;
	} else {
		return iWaveform::amplitude - slope * time;
	}
}

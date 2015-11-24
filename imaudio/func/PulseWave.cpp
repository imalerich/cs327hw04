#include <math.h>
#include "PulseWave.h"

double PulseWave::sampleAtTime(double time) {
	time = time * iWaveform::frequency; // adjust time to frequency
	time = time - (int)time; // remap to range [0, 1)
	
	// time may be 0 but may never be 1, therefore 
	// I am using an exclusive comparison
	// (ie. if pulse_ratio = 0.5, an equal number of possible
	// instances will be 'up' as will be 'down')
	if (time < pulse_ratio) {
		return iWaveform::amplitude;
	} else {
		return -iWaveform::amplitude;
	}
}

#include <math.h>
#include "SinWave.h"

long SinWave::sampleAtTime(double time) {
	time = (time * 2 * M_PI) * iWaveform::frequency;
	return sin(time) * iWaveform::amplitude;
}

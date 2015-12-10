#include <math.h>
#include "SinWave.h"

double SinWave::sample_at_time(double time) {
	time = (time * 2 * M_PI) * iWaveform::frequency;
	return sin(time) * iWaveform::amplitude;
}

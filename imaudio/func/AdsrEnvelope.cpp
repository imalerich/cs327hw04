#include <math.h>
#include "AdsrEnvelope.h"

AdsrEnvelope::AdsrEnvelope(double Attack, double Decay, double Release, double Sustain, double Length) {
	a = Attack;
	d = Decay;
	s = Sustain;
	r = Release;
	length = Length;
}

long AdsrEnvelope::sampleAtTime(double time) {
	if (time < 0.0) {
		return 0.0;
	}

	if (time < a) {
		return (1.0 / a) * time;
	}

	if (time < d) {
		auto slope = (1.0 - s) / (d - a);
		auto t = (time - a);
		return 1.0 - (slope * t);
	}

	if (time < r) {
		return s;
	}

	if (time < length) {
		auto slope = s / r;
		auto t = time - 4;
		return s - (slope * t);
	}

	return 0.0;
}

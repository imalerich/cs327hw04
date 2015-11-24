#include <math.h>
#include "AdsrEnvelope.h"

AdsrEnvelope::AdsrEnvelope(double Attack, double Decay, double Sustain, double Release, double Length) {
	a = Attack;
	d = Decay;
	s = Sustain;
	r = Release;
	length = Length;
}

double AdsrEnvelope::sampleAtTime(double time) {
	if (time < 0.0) {
		return 0.0;
	}

	if (time < a) {
		return (1.0 / a) * time;
	}

	if (time < a + d) {
		auto slope = (1.0 - s) / d;
		auto t = (time - a);
		return 1.0 - (slope * t);
	}

	cerr << "time: " << time << endl;
	cerr << "length: " << length << endl;
	cerr << "r: " << r << endl;
	cerr << "s: " << s << endl;
	if (time < length - r) {
		return s;
	}

	if (time < length) {
		auto slope = s / r;
		auto t = time - r;
		return s - (slope * t);
	}

	return 0.0;
}

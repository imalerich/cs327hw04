#include <math.h>
#include <algorithm>
#include "AdsrEnvelope.h"

using namespace std;

AdsrEnvelope::AdsrEnvelope(double Attack, double Decay, double Sustain, double Release, double Length) {
	length = max(Length, 0.0);
	a = max(Attack, 0.0);
	d = max(Decay, 0.0);
	s = max(Sustain, 0.0);
	r = max(Release, 0.0);

	if (length < r) {
		// produce no sound when r is greater than the length
		a = d = s = r = 0.0;
	} else if (length < a + r) {
		// fit the attack to length - release
		a = length - r;
		s = 1.0; // wil not be sustained
		d = 0.0; // no sustain or decay

	} else if (length < a + r + d) {
		// decay is too long
		d = length - r - a;
	}

	// otherwise we are good, continue as normal
}

double AdsrEnvelope::sample_at_time(double time) {
	if (time < 0.0 || time > length) {
		return 0.0;
	}

	if (time < a) {
		return max((1.0 / a) * time, 0.0);
	}

	if (time < a + d) {
		auto slope = (1.0 - s) / d;
		auto t = (time - a);
		return max(1.0 - (slope * t), 0.0);
	}

	if (time < length - r) {
		return s;
	}

	if (time < length) {
		auto slope = s / r;
		auto t = time - (length - r);
		return max(s - (slope * t), 0.0);
	}

	return 0.0;
}

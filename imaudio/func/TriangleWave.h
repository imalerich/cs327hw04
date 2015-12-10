#ifndef TRIANGLEWAVE_H
#define TRIANGLEWAVE_H

#include "iWaveform.h"

class TriangleWave : public iWaveform {
public:
	TriangleWave(double Amplitude, double Frequency) : iWaveform(Amplitude, Frequency) { }

	virtual double sample_at_time(double time);

	inline virtual string function_name() {
		return "trianglewave";
	}
};

#endif

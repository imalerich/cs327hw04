#ifndef SINWAVE_H
#define SINWAVE_H

#include "iWaveform.h"

class SinWave : public iWaveform {
public:
	SinWave(double Amplitude, double Frequency) : iWaveform(Amplitude, Frequency) { }

	virtual double sample_at_time(double time);

	inline virtual string function_name() {
		return "sinwave";
	}
};

#endif

#ifndef SAWTOOTHWAVE_H
#define SAWTOOTHWAVE_H

#include "iWaveform.h"

class SawToothWave : public iWaveform {
public:
	SawToothWave(double Amplitude, double Frequency) : iWaveform(Amplitude, Frequency) { }

	virtual double sample_at_time(double time);

	inline virtual string function_name() {
		return "sawtoothwave";
	}
};

#endif

#ifndef SINWAVE_H
#define SINWAVE_H

#include "iWaveform.h"

class SinWave : public iWaveform {
public:
	SinWave(double Amplitude, double Frequency) : iWaveform(Amplitude, Frequency) { }

	virtual long sampleAtTime(double time);

	inline virtual string functionName() {
		return "sinwave";
	}
};

#endif

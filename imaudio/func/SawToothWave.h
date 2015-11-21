#ifndef SAWTOOTHWAVE_H
#define SAWTOOTHWAVE_H

#include "iWaveform.h"

class SawToothWave : public iWaveform {
public:
	SawToothWave(double Amplitude, double Frequency) : iWaveform(Amplitude, Frequency) { }

	virtual long sampleAtTime(double time);

	inline virtual string functionName() {
		return "sawtoothwave";
	}
};

#endif

#ifndef TRIANGLEWAVE_H
#define TRIANGLEWAVE_H

#include "iWaveform.h"

class TriangleWave : public iWaveform {
public:
	TriangleWave(double Amplitude, double Frequency) : iWaveform(Amplitude, Frequency) { }

	virtual long sampleAtTime(double time);

	inline virtual string functionName() {
		return "trianglewave";
	}
};

#endif

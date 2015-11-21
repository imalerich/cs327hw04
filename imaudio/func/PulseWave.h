#ifndef PULSEWAVE_H
#define PULSEWAVE_H

#include "iWaveform.h"

class PulseWave : public iWaveform {
public:
	PulseWave(double Amplitude, double Frequency, double PulseRatio) : 
		iWaveform(Amplitude, Frequency), pulse_ratio{PulseRatio} { }

	virtual long sampleAtTime(double time);

	inline virtual string functionName() {
		return "pulsewave";
	}

private:
	/*
	 * Percentage of time spent in the 'up' state.
	 */
	double pulse_ratio;
};

#endif

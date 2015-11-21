#ifndef IWAVEFORM_H
#define IWAVEFORM_H

#include "iFunction.h"

/**
 * Describes a function that follows a repeting
 * waveform with an 'amplitude' and a 'frequency'.
 * Subclasses of iWaveform shuld override the methods
 * of iFunction and return values for sampleAtTime(...)
 * based on this interfaces 'amplitude' and 'frequency'.
 */
class iWaveform : public iFunction {
public:
	/**
	 * Construct a new waveform. In theory, classes that
	 * adhere to iWaveform will only need to rely on this
	 * method.
	 */
	iWaveform(double Amplitude, double Frequency) {
		amplitude = Amplitude;
		frequency = Frequency;
	}

	virtual ~iWaveform() { }

protected:
	/**
	 * The amplitude of the waveform. This value is half the total height 
	 * of the wave, or alternatively the max value of the function assuming
	 * it is centered at 0.
	 */
	double amplitude;

	/**
	 * The number of repetitions of the waveform per second.
	 * For example, a frequency of 60 for a sin wave will complete
	 * a full cycle 60 times over the course of one second.
	 */
	double frequency;

private:
};

#endif

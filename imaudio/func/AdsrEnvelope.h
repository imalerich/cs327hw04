#ifndef ASDRENVELOPE_H
#define ASDRENVELOPE_H

#include "iFunction.h"

class AdsrEnvelope : public iFunction {
public:
	/**
	 * Constructs an adsr envelope.
	 * \param Attack Attack time this envelope takes to go from 0 to peak.
	 * \param Decay Decay time this envelope takes to go from peak to susain.
	 * \param Sustain Sustain volume.
	 * \param Release Release time to go from sustain to 0 at the end of this envelope.
	 * \param Length Total length of envelope (in seconds).
	 */
	AdsrEnvelope(double Attack, double Decay, double Sustain, double Release, double Length);

	virtual double sampleAtTime(double time);

	/**
	 * \return The current length (in seconds) of this adsr envelope.
	 */
	inline double getLength() const {
		return length;
	}

	/**
	 * Setter for the length property.
	 * Input 'Length' must be greater than 0 or
	 * this method will have no affect.
	 * \param Length New length.
	 */
	inline void setLength(double Length) {
		if (length > 0) {
			length = Length;
		}
	}

	inline virtual string functionName() {
		return "asdrenvelope";
	}

private:
	double a;
	double d;
	double s;
	double r;
	double length;
};

#endif

#include <exception>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <math.h>

#include "Channel.h"
#include "flags.h"

static const string assign_msg = "strict_data enforced during assignment";
static const string overflow_msg = "Sample exceeds this Channels bit resolution!";
static const string length_msg = "strict_data enabled: Channels must have the same number of samples!";
static const string invalid_msg = "strict_data enabled: Channels must have the same bit_res";
static const string invalid_bit_res = "Invalid bit_res in constructor.";

Channel::Channel(size_t BitRes) : bit_res{BitRes} { 
	if (bit_res != 8 && bit_res != 16 && bit_res != 32) {
		throw invalid_argument(invalid_bit_res);
	}
}

Channel::Channel(const Channel &other) : bit_res{other.bit_res} {
	samples = other.samples;
}

Channel::Channel(const Channel &&other) : bit_res{other.bit_res} {
	samples = move(other.samples);
}

Channel& Channel::operator=(const Channel &other) {
	if (strict_data && other.bit_res != bit_res) {
		throw invalid_argument(assign_msg + " " + invalid_msg);
	}

	// we need to pull constants from 'other', not use our current constants
	*const_cast<size_t*>(&bit_res) = other.bit_res;

	samples = other.samples;
	return *this;
}

Channel& Channel::operator=(const Channel &&other) {
	if (strict_data && other.bit_res != bit_res) {
		throw invalid_argument(assign_msg + " " + invalid_msg);
	}

	// we need to pull constants from 'other', not use our current constants
	*const_cast<size_t*>(&bit_res) = other.bit_res;

	samples = move(other.samples);
	return *this;
}

ostream& operator<<(ostream &os, const Channel &channel) {
	for (auto i = 0; i < (int)channel.samples.size(); i++) {
		if (i != (int)channel.samples.size() -1 ) {
			os << channel.samples[i] << ", ";
		} else {
			os << channel.samples[i];
		}
	}

	return os;
}

Channel Channel::operator+(const Channel &other) {
	// check if we should allow Channel addition
	if (strict_data) {
		if (other.samples.size() != samples.size()) {
			throw length_error(length_msg);
		} else if (other.bit_res != bit_res) {
			throw invalid_argument(invalid_msg);
		}
	}

	// all is good, perform the addition
	Channel last = Channel(max(other.bit_res, bit_res));
	for (auto i = 0; i < (int)max(other.samples.size(), samples.size()); i++) {
		auto sample0 = i < (int)other.samples.size() ? other.samples[i] : 0;
		auto sample1 = i < (int)samples.size() ? samples[i] : 0;

		last.push_sample(sample0 + sample1);
	}

	return last;
}

Channel Channel::concat(const Channel &other) {
	// check whether or not we should allow concating Channels
	if (strict_data) {
		if (other.bit_res != bit_res) {
			throw invalid_argument(invalid_msg);
		}
	}
	
	// all is good, concat 'other' Channel to this Channel
	Channel last = Channel(max(other.bit_res, bit_res));
	last.samples = samples;
	for (auto x : other.samples) {
		last.samples.push_back(x);
	}

	return last;
}

void Channel::append(const Channel &other) {
	// check whether or not we should allow appending Channels
	if (strict_data) {
		if (other.bit_res != bit_res) {
			throw invalid_argument(invalid_msg);
		}
	}
	
	// all is good, concat 'other' Channel to this Channel
	for (auto x : other.samples) {
		samples.push_back(x);
	}
}

Channel Channel::operator*(const double &scalar) {
	Channel other = Channel(*this);
	for (auto &sample : other.samples) {
		if (is_valid_sample(sample * scalar)) {
			sample *= scalar;
		} else {
			throw overflow_error(overflow_msg);
		}
	}

	return other;
}

Channel Channel::operator-() {
	Channel other = *this;
	for (auto &sample : other.samples) {
		if (is_valid_sample(-sample)) {
			sample = -sample;
		} else {
			throw overflow_error(overflow_msg);
		}
	}

	return other;
}

void Channel::push_sample(long sample) {
	// check bounds specified by the bit resolution
	if (!is_valid_sample(sample)) {
		throw overflow_error(overflow_msg);
	}

	// all is good, add the sample to our samples vector
	samples.push_back(sample);
}

bool Channel::is_valid_sample(long sample) {
	const long max_val = pow(2, bit_res) / 2;
	const long min_val = -(pow(2, bit_res) / 2 - 1);

	return sample <= max_val && sample >= min_val;
}

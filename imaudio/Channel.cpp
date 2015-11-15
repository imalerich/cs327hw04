#include <exception>
#include <algorithm>
#include <stdexcept>
#include <string>
#include "Channel.h"

static const string assign_msg = "strict_data enforced during assignment";
static const string overflow_msg = "Sample exceeds this Channels bit resolution!";
static const string length_msg = "strict_data enabled: Channels must have the same number of samples!";
static const string invalid_msg = "strict_data enabled: Channels must have the same bit_res";

Channel::Channel(size_t BitRes, bool Strict) : bit_res{BitRes}, strict_data{Strict} { }

Channel::Channel(const Channel &other) : bit_res{other.bit_res}, strict_data{other.strict_data} {
	samples = other.samples;
}

Channel::Channel(const Channel &&other) : bit_res{other.bit_res}, strict_data{other.strict_data} {
	samples = move(other.samples);
}

Channel& Channel::operator=(const Channel &other) {
	if (other.bit_res != bit_res) {
		throw invalid_argument(assign_msg + invalid_msg);
	}

	samples = other.samples;
	return *this;
}

Channel& Channel::operator=(const Channel &&other) {
	if (other.bit_res != bit_res) {
		throw invalid_argument(assign_msg + invalid_msg);
	}

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
	if (other.strict_data || strict_data) {
		if (other.samples.size() != samples.size()) {
			throw length_error(length_msg);
		} else if (other.bit_res != bit_res) {
			throw invalid_argument(invalid_msg);
		}
	}

	// all is good, perform the addition
	Channel last = Channel(max(other.bit_res, bit_res), other.strict_data && strict_data);
	for (auto i = 0; i < (int)max(other.samples.size(), samples.size()); i++) {
		auto sample0 = i < (int)other.samples.size() ? other.samples[i] : 0;
		auto sample1 = i < (int)samples.size() ? samples[i] : 0;

		last.push_sample(sample0 + sample1);
	}

	return last;
}

Channel Channel::append(const Channel &other) {
	// check whether or now we should allow appending Channels
	if (other.strict_data || strict_data) {
		if (other.bit_res != bit_res) {
			throw invalid_argument(invalid_msg);
		}
	}
	
	// all is good, append 'other' Channel to this Channel
	Channel last = Channel(max(other.bit_res, bit_res), other.strict_data && strict_data);
	last.samples = samples;
	for (auto x : other.samples) {
		last.samples.push_back(x);
	}

	return last;
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

void Channel::push_sample(int sample) {
	// check bounds specified by the bit resolution
	if (!is_valid_sample(sample)) {
		throw overflow_error(overflow_msg);
	}

	// all is good, add the sample to our samples vector
	samples.push_back(sample);
}

bool Channel::is_valid_sample(int sample) {
	static const int max_val = pow(2, bit_res) / 2;
	static const int min_val = -(pow(2, bit_res) / 2 - 1);

	return sample <= max_val && sample >= min_val;
}

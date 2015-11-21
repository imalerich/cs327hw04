#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>
#include <iostream>

using namespace std;

/**
 * Representation of a channel. A channel requires that
 * a bit resolution be specified, it will then insure that all
 * data added to it fits within the bit resolution.
 * Internally all data is stored within a vector.
 * A channel has no knowledge of it's sample rate, and
 * therefore could not be 'played' back to the user at all.
 */
class Channel {
public:
	/**
	 * \param BitRes Resolution (in bits) of the data stored in this channel.
	 */
	Channel(size_t BitRes);
	Channel(const Channel &other);
	Channel(const Channel &&other);
	Channel& operator=(const Channel &other);
	Channel& operator=(const Channel &&other);
	friend ostream& operator<<(ostream &os, const Channel &channel);

	/**
	 * Scales each sample of this channel by the input scalar value.
	 * Resulting integers are truncated.
	 * If the scalar were to cause a sample to exceed this channels
	 * bit resolution, this operator will thrown an overflow_exception.
	 * \param scalar Scalar value to apply to each sample of this channel.
	 */
	Channel operator*(const double &scalar);

	/**
	 * Inverts each sample of this Channel.
	 * Because the max_value of the bit_res is 1 greater than the 
	 * absolute value of the min_value of the bit_res, this operator
	 * will throw an overflow_exception when trying to invert
	 * any instances of max_value.
	 */
	Channel operator-();

	/**
	 * Creates a new channel where each sample is the sum of this channels samples
	 * and the samples of the 'other' Channel. 
	 * If the sum of any sample exceeds the resulting bit resolution an overflow_error is thrown.
	 * If 'strict_data' is enabled in 'flags.h':
	 * 	Both channels must have the same bit_resolution (invalid_argument exception).
	 * 	Both channels must have the same size (length_error exception).
	 * Otherwise:
	 * 	The higher 'bit_res' will be used.
	 * 	The larger size will be used.
	 * 	The smaller channel will be treated as if it had 0's beyond its end.
	 */
	Channel operator+(const Channel &other);

	/**
	 * Creates a new Channel with the sample array of 'other' concated to the end
	 * of this Channel's sample array.
	 * \param other The other Channel to concat with this channel.
	 * \return A new channel with the contents of 'other' concated to this channel.
	 */
	Channel concat(const Channel &other);

	/**
	 * Similar to conat(...), but does not create a new channel.
	 * Inplace adds the concat of other to this channel.
	 * \param other The other Channelt to append to this channel.
	 */
	void append(const Channel &other);

	/**
	 * Attempts to push the input sample to the end of this Channels sample vector.
	 * If the sample data will not fit in this Channel's bit resolution, this 
	 * method will throw an overflow_error exception.
	 * is_valid_sample will be used to determine whether or not the sample is valid.
	 * \param Input sample to concat to the samples vector.
	 */
	void push_sample(long sample);

	/**
	 * Determines whether or not the input sample fits within
	 * this Channel's bit resolution or not.
	 * \param sample A sample to be tested.
	 * \return Whether or not the sample is valid for this Channel.
	 */
	bool is_valid_sample(long sample);

	/**
	 * \returns The number of samples stored in this channel.
	 */
	inline size_t size() const {
		return samples.size();
	}

	/**
	 * Forwards the [] operator to the vector. Exceptions generated
	 * by the vector's [] operator are not handled by this method.
	 * \param n Index of the sample to grab.
	 * \return The sample at the given index.
	 */
	inline long& operator[](size_t n) {
		return samples[n];
	}
	
private:
	vector<long> samples; /**< The array of samples representing this channel. */
	const size_t bit_res; /**< Resolution (in bits) of the data for this channel. */
};

#endif

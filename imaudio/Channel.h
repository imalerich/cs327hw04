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
	 * \param Strict Determines whether or not to enforce strict rule swhen modifying multiple channels.
	 */
	Channel(size_t BitRes, bool Strict = true);
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
	 * If either channel has 'strict_data' enabled the following requirements must be met:
	 * 	Both channels must have the same bit_resolution (invalid_argument exception).
	 * 	Both channels must have the same size (length_error exception).
	 * Otherwise:
	 * 	The higher 'bit_res' will be used.
	 * 	The larger size will be used.
	 * 	The smaller channel will be treated as if it had 0's beyond its end.
	 * The resulting channel will have 'strict_data' disabled ONLY if
	 * both input Channels have 'strict_data' disabled.
	 */
	Channel operator+(const Channel &other);

	/**
	 * Creates a new Channel with the sample array of 'other' appended to the end
	 * of this Channel's sample array.
	 * \param other The other Channel to append to this channel.
	 * \return A new channel with the contents of 'other' appended to this channel.
	 */
	Channel append(const Channel &other);

	/**
	 * Attempts to push the input sample to the end of this Channels sample vector.
	 * If the sample data will not fit in this Channel's bit resolution, this 
	 * method will throw an overflow_error exception.
	 * is_valid_sample will be used to determine whether or not the sample is valid.
	 * \param Input sample to append to the samples vector.
	 */
	void push_sample(int sample);

	/**
	 * Determines whether or not the input sample fits within
	 * this Channel's bit resolution or not.
	 * \param sample A sample to be tested.
	 * \return Whether or not the sample is valid for this Channel.
	 */
	bool is_valid_sample(int sample);

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
	inline int& operator[](size_t n) {
		return samples[n];
	}
	
private:
	vector<int> samples; /**< The array of samples representing this channel. */
	const size_t bit_res; /**< Resolution (in bits) of the data for this channel. */
	const bool strict_data; /**< Determines whether to enforce equal channel size when adding. */
};

#endif

/*
 * audiogram.h
 *
 *  Created on: Jun 18, 2022
 *      Author: 16502
 */

#ifndef SRC_AUDIOGRAM_H_
#define SRC_AUDIOGRAM_H_

#include <array>

#include "Implementation_Constants.h"


class Audiogram {
public:
	Audiogram(const float* g, const int* w);
	Audiogram(const float* g, const int w);
	Audiogram();
	~Audiogram();
	void interpolate_gain(int channel, std::array<complex_type,PPN_length>& freq_bins);

private:
	float gain[FilterBank_channels];
	int width[FilterBank_channels];
};



#endif /* SRC_AUDIOGRAM_H_ */

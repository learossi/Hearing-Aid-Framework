/*
 * utilities.h
 *
 *  Created on: Jun 17, 2022
 *      Author: 16502
 */

#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_

#include <Implementation_Constants.h>
#include "arm_const_structs.h"

template <int base>
constexpr int compile_time_pwr_2(int input){
	return ((input > base) ? compile_time_pwr_2<base*2>(input) : base);
}

template <>
constexpr int compile_time_pwr_2<1073741824>(int input){
	return 1073741824;
}

const arm_cfft_instance_f32* get_fft_settings(int length);

#endif /* SRC_UTILITIES_H_ */

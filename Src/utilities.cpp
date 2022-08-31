/*
 * utilities.cpp
 *
 *  Created on: Jun 18, 2022
 *      Author: 16502
 */

#include "utilities.h"

const arm_cfft_instance_f32* get_fft_settings(int length){
	switch(length){
		case 16:
			return &arm_cfft_sR_f32_len16;
		case 32:
			return &arm_cfft_sR_f32_len32;
		case 64:
			return &arm_cfft_sR_f32_len64;
		case 128:
			return &arm_cfft_sR_f32_len128;
		case 256:
			return &arm_cfft_sR_f32_len256;
		case 512:
			return &arm_cfft_sR_f32_len512;
		case 1024:
			return &arm_cfft_sR_f32_len1024;
		case 2048:
			return &arm_cfft_sR_f32_len2048;
		case 4096:
			return &arm_cfft_sR_f32_len4096;
	}
}

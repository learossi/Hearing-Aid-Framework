/*
 * FIRFilter.h
 *
 *  Created on: Jun 16, 2022
 *      Author: 16502
 */

#ifndef SRC_FIRFILTER_H_
#define SRC_FIRFILTER_H_

#include <Implementation_Constants.h>
#include <array>

#include "InputManager.h"
#include "CircBuffer.h"
//this is sloppy w/ the input manager template default so I don't need to rewrite my other code
//I should probably just be using type aliases


using input_manager_type = Input_Manager<FFTLength>;

template<int L>
class FIRFilter {
public:
	FIRFilter() = default;
	~FIRFilter() = default;

	float filter(input_manager_type&, int);
	float operator[](const int index){ return f_coeffs.at(index);}
	float operator[](const int index) const { return f_coeffs.at(index);}
	void operator +(std::array<data_type, L>&);

private:
	std::array<float, L> f_coeffs;
};


template<int L>
float FIRFilter<L>::filter(input_manager_type& input, const int offset){
	float output = 0;
	for(int i = 0; i < L; i++){
		output += f_coeffs.at(i)*input[-i + offset];
	}
	return output;
}

template<int L>
void FIRFilter<L>::operator +(std::array<data_type, L>& a){
	for(int i = 0; i < L; ++i){
		f_coeffs[i] += a[i];
	}
}

#endif /* SRC_FIRFILTER_H_ */

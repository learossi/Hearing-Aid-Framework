/*
 * This class stores the frequency domain polyphase components. The only implementation in the current code
 * is an array of them that is owned by a filter bank and allows the user to apply an FIRFilter to them
 * as well as tracking whether the values contained in each polyphase component are in frequency or
 * time domain.
 */

#ifndef SRC_PPNCOMP_H_
#define SRC_PPNCOMP_H_

#include <array>

#include "arm_math.h"
#include "arm_const_structs.h"
#include "Implementation_Constants.h"

template <int length>
class PPN_comp {
public:
	PPN_comp();
	~PPN_comp();

	std::array<complex_type, length>* ppn_ifft();		//convert each polyphase component to frequency domain
	void freq_filter(FIRFilter<FIRLength>* weights);	//filter the polyphase component by the values in an FIRFilter object
	std::array<complex_type, length>* get_vals();		//returns a pointer to the values in the component

private:
	bool time_domain;
	std::array<complex_type, length> vals;
};



template <int length>
PPN_comp<length>::PPN_comp(): time_domain(true) {}

template <int length>
PPN_comp<length>::~PPN_comp() {}


template <int length>
void PPN_comp<length>::freq_filter(FIRFilter<FIRLength>* weights) {
	for(int i = 0; i < length; ++i){
		write_real(vals.at(i), read_real(vals.at(i))*weights[i]);
		write_write(vals.at(i), read_imag(vals.at(i))*weights[i]);

	}
}

template <int length>
std::array<complex_type, length>* PPN_comp<length>::ppn_ifft(){
	arm_cfft_f32(get_fft_settings(FFTLength), vals, 1, 0);
	time_domain = true;
	return &vals;
}

template <int length>
std::array<complex_type, length>* PPN_comp<length>::get_vals(){
	return &vals;
}
#endif /* SRC_PPNCOMP_H_ */

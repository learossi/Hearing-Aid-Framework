/*
 * adaptivenoisecanceler.h
 *
 *  Created on: Jun 21, 2022
 *      Author: 16502
 */

#ifndef INC_ADAPTIVENOISECANCELER_H_
#define INC_ADAPTIVENOISECANCELER_H_

#include <array>
#include "FIRFilter.h"
#include "CircBuffer.h"

//this needs a function I can call to output the thing

template <int fb_path_order, typename input_manager_type>
class Adaptive_Noise_Canceler {
public:
	Adaptive_Noise_Canceler(float m, int d);
	~Adaptive_Noise_Canceler();
	void update_filter(input_manager_type&);
	void filter_new_vals(input_manager_type&, const int);


private:
	using fb_array = std::array<float, fb_path_order>;

	const int delay; //denoted in number of samples
	FIRFilter<fb_path_order> fb;
	fb_array error_vec;
	fb_array error_vec_prev; //it actually should be one shorter but I kinda wanna to just be able to exchange the pointers
	fb_array filter_output;
	const float mu;

};

template <int fb_path_order, typename input_manager_type>
Adaptive_Noise_Canceler<fb_path_order, input_manager_type>::Adaptive_Noise_Canceler(float m, int d)
: mu(m), delay(d)
{

}

//there's a good chance this should have LMS in the name so ppl are clear on what it does
template <int fb_path_order, typename input_manager_type>
void Adaptive_Noise_Canceler<fb_path_order, input_manager_type>::update_filter(input_manager_type& m){

	CircBuffer<float, fb_path_order> cb = CircBuffer<float, fb_path_order>();
	fb_array update_vec;

	//initialize the circular buffer
	for(int i = 0; i < fb_path_order-1; ++i){
		//add an error check?
		cb.push(m[-fb_path_order+1+i]*error_vec_prev[1+i]);
	}
	//error check?
	cb.push(m[0]*error_vec[0]);

	//first iteration of the update algorithm
	for(int j = 0; j < fb_path_order; ++j){
		if(cb[j]){
			float t_;
			cb[j].get(t_);
			update_vec[j] += mu*t_;
		}else{
			//assert
		}
	}

	//update filter
	for(int i = 1; i < fb_path_order; ++i){
		for(int j = 0; j < fb_path_order; ++j){
			if(cb[j]){
				float t_;
				cb[j].get(t_);
				update_vec[j] += mu*t_;
			}else{
				//assert
			}
		}
		cb.rotate_buf(m[i]*error_vec[i]);
	}

	fb + update_vec; //updates the filter coefficients
}

//I'm not sure I should be updating the error_vec here - it seems a lot more cache efficient
template <int fb_path_order, typename input_manager_type>
void Adaptive_Noise_Canceler<fb_path_order, input_manager_type>::filter_new_vals(input_manager_type& m, const int input_size){
	//this feels like a copy that I should avoid somehow - maybe use pointers
	error_vec_prev = error_vec;

	for(int i = 0; i < input_size; ++i){
		filter_output[i] = fb.filter(m, i - delay); //need to reason about if my delay calculation is correct here
		//am I actually subtracting this delayed version from the output or just using this for filter updating?
		error_vec[i] = filter_output[i] - m[i];
	}
}


#endif /* INC_ADAPTIVENOISECANCELER_H_ */

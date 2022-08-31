#include "Filter_Pipeline.h"

//this is where we call all the ingest functions
void Filter_Pipeline::initialize_filter_bank(){
	f -> set_output_array(data);
	f -> real_ingest_vals();
	f -> FB_fft(); //note that the conversion to frequency domain precedes the filter so we scale well
	f -> analysis_filter();
}

void Filter_Pipeline::terminate_filter_bank(){
	f -> FB_ifft(); //note that the conversion to frequency domain precedes the filter so we scale well
	f -> synthesis_filter();
	f -> real_output_vals();
}

//Setter method for filter bank
void Filter_Pipeline::set_filter_bank(FilterBank<FIRLength, samples_per_cycle, FilterBank_channels>* fb){
	f = fb;
}

void Filter_Pipeline::get_data(std::array<complex_type, FFTLength>* input){
	data = input;
}

void Filter_Pipeline::add_task(const std::function<void()> task, const bool freq_domain_flag){
	if(filter_bank_data){
		pipeline.push_back(task);
		return;
	}

	//if a task is in the frequency domain, but the data is in time-domain, and fft is executed and vice versa
	if(freq_domain_flag != freq_domain){
		if(freq_domain){
			pipeline.push_back([&](){ifft(this -> data -> data(), FFTLength);});
			pipeline.push_back(task);
		}
			pipeline.push_back([&](){fft(this -> data -> data(), FFTLength);});
			pipeline.push_back(task);
			return;
	}
	pipeline.push_back(task);
	return;

}

//execute all tasks on sequence and output data back into the input array
void Filter_Pipeline::execute_tasks(){
	for(auto i = pipeline.begin(); i != pipeline.end(); i++){
		i->operator ()();
	}
}

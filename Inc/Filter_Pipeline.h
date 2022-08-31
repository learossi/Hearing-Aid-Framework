/*
This is a pipeline class that allows programmers to encapsulate the tasks they wish to be performed
on specified data in sequence. The data being operated in is specified by get_data and can be changed
between executions of the pipeline. The pipeline also has a reference to a FilterBank that it uses
to perform any FilterBank operations.
Execute_tasks is used to execute the pipeline. The XXX_filter_bank functions are intended to be inserted
into the pipeline to signify the data is inside the Filterbank. All other functions are used to set up the
pipeline.

 */

#ifndef INC_FILTER_PIPELINE_H_
#define INC_FILTER_PIPELINE_H_

#include <vector>

#include "Implementation_Constants.h"
#include "FilterBank.h"

class Filter_Pipeline{
public:
	void add_task(std::function<void()>, bool freq_domain_flag);
	void get_data(std::array<complex_type, FFTLength>*);
	void execute_tasks();
	void initialize_filter_bank();
	void terminate_filter_bank();
	void set_filter_bank(FilterBank<FIRLength, samples_per_cycle, FilterBank_channels>*);

	std::array<complex_type, FFTLength>* get_data_pointer(){return data;}

private:
	std::array<complex_type, FFTLength>* data;
	FilterBank<FIRLength, samples_per_cycle, FilterBank_channels>* f;
	bool filter_bank_data = false;
	bool freq_domain = false;
	std::vector<std::function<void()>> pipeline;

};



#endif /* INC_FILTER_PIPELINE_H_ */

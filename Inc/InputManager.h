/*
 * This is a class specifically designed to provide access to past data that is being fed in from a DMA system.
 * Specifically, it maintains 2 arrays, one for the previous block of samples, the one for the current data
 * being operated on, and a third for the data being currently read in.
 * The class is designed so that the DMA array switches between two separate arrays: one of which is being
 * written to and the other one is being converted to complex values that can be fed into the FFT algorithm.
 *
 */

#ifndef SRC_INPUTMANAGER_H_
#define SRC_INPUTMANAGER_H_

#include <array>

#include "Implementation_Constants.h"

template <const int input_size>
class Input_Manager {
public:
	Input_Manager();
	~Input_Manager();

	using input_array_type = std::array<std::array<float, input_size>,2>;
	using output_array_type = std::array<std::array<complex_type, input_size>,2>;


	std::array<complex_type, input_size>* get_current_vals(){return &(outputs -> at(counter));} //it's a pointer to array_type you idiot
	std::array<float, input_size>* get_prev_vals(){return &(outputs[(counter+1)%2]);}
	void next_input(){convert_input(); counter+=1; counter = counter%2;} //well, I never said it was thread-safe
	float operator [](int index);
	void convert_input();

private:
	input_array_type* inputs;
	output_array_type* outputs;
	int counter = 0;
};

template <int input_size>
Input_Manager<input_size>::Input_Manager(){
	inputs = new Input_Manager::input_array_type;
	outputs = new Input_Manager::output_array_type;
}

template <int input_size>
Input_Manager<input_size>::~Input_Manager(){
	delete Input_Manager::inputs;
	delete Input_Manager::outputs;
}

template <int input_size>
float Input_Manager<input_size>::operator [](int index){
	//assert_param(index > input_size && index > -input_size);

	if(index >= 0){
		return outputs[counter][index];
	}else{
		return get_prev_vals()[input_size+index-1];
	}
}

template <int input_size>
void Input_Manager<input_size>::convert_input(){
	for(int i = 0; i < input_size; ++i){
		outputs->at(counter).at(i) = complex_type(inputs->at(i).at(counter),0);
	}
}
#endif /* SRC_INPUTMANAGER_H_ */

/*
This is an implementation of a perfect-reconstruction filter-bank that uses frequency domain FIR filters.
Class intended to be owned by a pipeline object that splits the data into a polyphase filterbank and
applies FIR filters of a chosen specification.
Other classes in the program then operate on the components.
*/

#ifndef SRC_FILTERBANK_H_
#define SRC_FILTERBANK_H_

#include <array>

#include "FIRFilter.h"
#include "Implementation_Constants.h"
#include "PPNcomp.h"
#include "Filter_Coefficients.h"

template <int filter_L, int sample_N, int M>
class FilterBank {
public:
	FilterBank();
	~FilterBank();

	void real_ingest_vals();
	void real_output_vals();

	void set_output_array(std::array<complex_type, FFTLength>*);

	void FB_fft();
	void FB_ifft();
	void analysis_filter();
	void synthesis_filter();

	std::array<complex_type, PPN_length>& get_ppn_vals(int);


private:

	std::array<PPN_comp<PPN_length>, FilterBank_channels> ppn; 			//Each array member is an object that corresponds to one modulated prototype filter
	std::array<complex_type, filter_L-1> overlap_buf;

	std::array<std::array<complex_type, PPN_length>, M> fft_container;  //intermediate container to save processing power on FFT computations
	//Note there is nothing enforcing that these be of length that is a power of 2

	std::array<complex_type, FFTLength> *in_out_array;
};


template <int filter_L, int sample_N, int M>
FilterBank<filter_L, sample_N, M>::FilterBank(): in_out_array(nullptr) {

}


//Set the array to which values are both pulled from and to which they are outputted to.
//Once real_ingest_vals is called, this array isn't touched until output.
template <int filter_L, int sample_N, int M>
void FilterBank<filter_L, sample_N, M>::set_output_array(std::array<complex_type, FFTLength>* out){
	in_out_array = out;
}

template <int filter_L, int sample_N, int M>
FilterBank<filter_L, sample_N, M>::~FilterBank() {
	// TODO Auto-generated destructor stub
}

template <int filter_L, int sample_N, int M>
void FilterBank<filter_L, sample_N, M>::real_ingest_vals(){
	for(int i = 0; i < filter_L - 1; ++i){
		//This adds the time domain aliasing from last epoch to the beginning of the new data
		//This method is called Overlap-Add if you want to look it up
		in_out_array -> at(i) = overlap_buf.at(i) + in_out_array -> at(i);
	}

	//Split the input signal into components for the FFT
	//Using a right shift in the data here is a bit unorthodox:
	//the equation can be derived from slide 31 of the following by expanding the X(z) term after the 2nd equality:
	//http://www.ws.binghamton.edu/fowler/fowler%20personal%20page/EE521_files/IV-05%20Polyphase%20FIlters%20Revised.pdf
	for(int i = 0; i < M; ++i){
		write_real(fft_container.at(i).at(0), read_real(in_out_array -> at(FFTLength - i)));
		//We don't loop over PPN_length here because that has been padded to be compatible w/ the FFT
		for(int j = 1; j < FFTLength/M; ++j){
			write_real(fft_container.at(i).at(j), read_real(in_out_array -> at(j*M - i)));
		}
	}

}

//write the final contents of the PPN components to the input-output buffer
template <int filter_L, int sample_N, int M>
void FilterBank<filter_L, sample_N, M>::real_output_vals(){
	for(int i = 0; i < M; ++i){
		write_real(in_out_array -> at(FFTLength - i), read_real(fft_container.at(i).at(0)));
		//We don't loop over all of PPN_length here because that has been padded to be compatible w/ the FFT
		for(int j = 1; j < FFTLength/M; ++j){
			write_real(in_out_array -> at(j*M - i), read_real(fft_container.at(i).at(j)));
		}
	}

	//initialize the overlap buffer from this sample
	for(int i = 0; i < filter_L - 1; ++i){
		overlap_buf[i] = in_out_array -> at(FFTLength-1-filter_L+i);
	}
}

//perform a sample-wise FFT on each of the downsampled PPN data components
template <int filter_L, int sample_N, int M>
void FilterBank<filter_L, sample_N, M>::FB_fft(){
	for(int i = 0; i < M; ++i){
		fft(fft_container.at(i).data(), PPN_length);
	}
}

//perform a sample-wise iFFT on each of the PPN components
template <int filter_L, int sample_N, int M>
void FilterBank<filter_L, sample_N, M>::FB_ifft(){
	for(int i = 0; i < M; ++i){
		ifft(&(fft_container.at(i).at(0)), PPN_length);
	}
}

template <int filter_L, int sample_N, int M>
void FilterBank<filter_L, sample_N, M>::analysis_filter() {
	for(int i = 0; i < FilterBank_channels; ++i){//iterate over various filters
		for(int j = 0; j < M; ++j){//iterate over various polyphase components
			for(int k = 0; k < PPN_length; ++k){//iterate over the downsampled values themselves
				ppn.at(i).get_vals()->at(k) = ppn.at(i).get_vals() -> at(k) +
						Analysis_Freq_Filter_Matrix.at(i).at(j)[k]*fft_container.at(j).at(k);
			}
		}
	}
}

template <int filter_L, int sample_N, int M>
void FilterBank<filter_L, sample_N, M>::synthesis_filter() {
	for(int i = 0; i < FilterBank_channels; ++i){//iterate over various filters
		for(int j = 0; j < M; ++j){//iterate over various polyphase components
			//we've inverted the order in which components are processed since this is a type 2 polyphase system now
			for(int k = PPN_length - 1; k == 0; --k){//iterate over the downsampled values themselves
				ppn.at(i).get_vals()->at(k) = ppn.at(i).get_vals() -> at(k) +
						Synthesis_Freq_Filter_Matrix.at(i).at(j)[k]*fft_container.at(j).at(k);
			}
		}
	}
}

template <int filter_L, int sample_N, int M>
std::array<complex_type, PPN_length>& FilterBank<filter_L, sample_N, M>::get_ppn_vals(int index){
	return *(ppn.at(index).get_vals());
}


#endif /* SRC_FILTERBANK_H_ */

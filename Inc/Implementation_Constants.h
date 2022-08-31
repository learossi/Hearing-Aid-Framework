#ifndef IMPLEMENTATION_CONSTANTS_H
#define IMPLEMENTATION_CONSTANTS_h

#include "utilities.h"

#include <complex>
#include <functional>



#define FIRLength 50
#define samples_per_cycle 200
#define sample_rate 40000 //in Hz
#define DELAY samples_per_cycle/sample_rate
#define FFTLength compile_time_pwr_2<2>(FIRLength + samples_per_cycle - 1)
#define FilterBank_channels 10
#define Downsample_Factor 5
#define PPN_length compile_time_pwr_2<2>(FFTLength/Downsample_Factor)


using data_type = float;
using complex_type = std::complex<float>;

//do the complex real and imaginary write and reads here

extern std::function<float(complex_type)> read_real;
extern std::function<float(complex_type)> read_imag;

extern std::function<void(complex_type&, data_type)> write_real;
extern std::function<void(complex_type&, data_type)> write_imag;

extern std::function<void(complex_type*, int)> fft;
extern std::function<void(complex_type*, int)> ifft;

//extern float Audiogram_Gain[];

#endif

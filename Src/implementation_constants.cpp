#include <Implementation_Constants.h>

//Hooks for the functions that read the real and imaginary parts of the complex number type one chooses
std::function<float(complex_type)> read_real = [](std::complex<float> c){return c.real();};
std::function<float(complex_type)> read_imag = [](std::complex<float> c){return c.imag();};

//Same with the write functions
std::function<void(complex_type&, data_type)> write_real = [](std::complex<float>& target, float val){
	target = std::complex<float>(val, read_imag(target));
};

std::function<void(complex_type&, data_type)> write_imag = [](std::complex<float>& target, float val){
	target = std::complex<float>(read_real(target), val);
};

//The FFT and IFFT functions of choice - I don't enforce anything about compatibility w/ chosen data types
//so that's something you have to be careful of on your own
std::function<void(complex_type*, int)> fft = [](complex_type* vals, int length){
	arm_cfft_f32(get_fft_settings(length), reinterpret_cast<data_type*>(vals), 0, 0);
};
std::function<void(complex_type*, int)> ifft =  [](complex_type* vals, int length){
	arm_cfft_f32(get_fft_settings(length), reinterpret_cast<data_type*>(vals), 1, 0);
};

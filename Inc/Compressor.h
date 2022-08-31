/*
This is a relatively standard compressor. It takes all the normal parameters you can expect from a compressor
(you can read the class declaration - units are also there).
The compressor is applied to a frequency domain array of complex values. I suppose you could write a script
to apply it to an array of PPN components, but right now it's just written to compress the raw values.
 */

#ifndef INC_COMPRESSOR_H_
#define INC_COMPRESSOR_H_

#include <cmath>
#include <array>

#include "Implementation_Constants.h"

template <int bins>
class Compressor {
public:
	using gain_arr = std::array<complex_type, bins>;

	Compressor(float rc, float at, float th, float kw, float cr, int sr, float vl = 0);
	~Compressor();

	void compress_freq(gain_arr freq_bins);

private:
	float conv_to_db(float input);

	gain_arr gain_mem;
	const float release_time_constant; //ms
	const float attack_time_constant; //ms
	const float threshold;
	const float knee_width;
	const float compression_ratio;
	const int samp_rate; //hz
	const float volume_level;
};


//frankly, I should probably have the complicated names as constructor names and the simple ones as member names
template <int bins>
Compressor<bins>::Compressor(float rc, float at, float th, float kw, float cr, int sr, float vl):
	release_time_constant(rc), //ms
	attack_time_constant(at), //ms
	threshold(th), //db
	knee_width(kw), //db
	compression_ratio(cr),
	samp_rate(sr), //hz
	volume_level(vl)
{}

template <int bins>
Compressor<bins>::~Compressor(){}

//implementing this algorithm
//https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7545261/

template <int bins>
void Compressor<bins>::compress_freq(gain_arr freq_bins) {
	for(int i = 0; i < bins; ++i){
		float spec_power_db_r = conv_to_db(read_real(freq_bins[i]));
		float spec_power_db_i = conv_to_db(read_imag(freq_bins[i]));
		//I could use the L1 norm here to save multiplications, but ehhh....
		float spec_power_db = sqrt(pow(spec_power_db_r,2) + pow(spec_power_db_i,2));
		float Vg;

		if((spec_power_db-threshold)*2 > knee_width){
			Vg = threshold + (spec_power_db - threshold)/compression_ratio;
		}else if((spec_power_db-threshold)*2 > -knee_width){
			Vg = spec_power_db + (1/compression_ratio-1)*pow((spec_power_db-threshold+knee_width/2),2)/(2*knee_width);
		}else{
			Vg = spec_power_db;
		}

		float Zg;
		const float time_const;
		if((Vg - spec_power_db) > gain_mem[i]){//attack
			time_const = exp(-bins/(attack_time_constant*samp_rate));
		}else{//release
			time_const = exp(-bins/(release_time_constant*samp_rate));
		}

		Zg = gain_mem[i]*time_const + (1-time_const)*(Vg - spec_power_db);
		gain_mem[i] = Vg;
		//I really need to verify this part to make sure there's no stability concerns

		freq_bins[i] = freq_bins[i] * 10 * ((volume_level+Zg)/20);//This linearalizes the signal
	}
}

template <int bins>
float Compressor<bins>::conv_to_db(float input){
	return 10*log10(pow(input,2));
	//I might want to add some base ratio I can tune for this to make it an actual decibel calculation
}

#endif /* INC_COMPRESSOR_H_ */


#include <Audiogram.h>

//Constructor that allows a non-uniform channel width
Audiogram::Audiogram(const float* g, const int* w) {
	for(int i = 0; i < FilterBank_channels; ++i){
		gain[i] = *(g+i);
		width[i] = *(w+i);
	}
}

//Constructor for a uniform filter-bank with fixed-width channels

Audiogram::Audiogram(const float* g, const int w) {
	for(int i = 0; i < FilterBank_channels; ++i){
			gain[i] = *(g+i);
			width[i] = w;
		}
}


Audiogram::Audiogram() {
	for(int i = 0; i < FilterBank_channels; ++i){
			gain[i] = 0;
			width[i] = 0;
		}
}

Audiogram::~Audiogram() {}

void Audiogram::interpolate_gain(int channel, std::array<complex_type, PPN_length>& freq_bins){
	//assert(channel > 0 && channel < (channels - 1));

	if(channel == 0){
		float slope = 2*(gain[channel+1] - gain[channel])/(width[channel+1]-width[channel]);
		int center = width[0];
		for(int i = 0; i < center; ++i){
			freq_bins[center-i] = freq_bins[center-i]*(gain[channel]-slope*i);
		}
		for(int i = 0; i < width[channel] - center; ++i){
			freq_bins[center+i+1] = freq_bins[center+i+1]*(gain[channel]+slope*i);
		}
	}else if(channel == FilterBank_channels-1){
		float slope = 2*(gain[channel] - gain[channel-1])/(width[channel]-width[channel-1]);
		int center = width[FilterBank_channels-1];
		for(int i = 0; i < center; ++i){
			freq_bins[center-i] = freq_bins[center-i]*(gain[channel]-slope*i);
		}

		for(int i = 0; i < width[channel] - center; ++i){
			freq_bins[center+i+1] = freq_bins[center+i+1]*(gain[channel]+slope*i);
		}
	}else{
		int center = width[channel]/2;
		float slope1 = 2*(gain[channel] - gain[channel-1])/(width[channel]-width[channel-1]);
		float slope2 = 2*(gain[channel+1] - gain[channel])/(width[channel+1]-width[channel]);

		for(int i = 0; i < center; ++i){
			freq_bins[center-i] = freq_bins[center-i]*(gain[channel]-slope1*i);
		}
		for(int i = 0; i < width[channel] - center; ++i){
			freq_bins[center+i+1] = freq_bins[center+i+1]*(gain[channel]+slope2*i);
		}
	}
}

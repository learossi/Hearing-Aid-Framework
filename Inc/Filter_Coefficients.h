

#include <array>

#include "FIRFilter.h"
#include "Implementation_Constants.h"


//The PPN components of the filters decomposed into a matrix with rows being for a single filter
//Coefficients should be decomposed as would normally happen in a PPN network and then transformed
//into the frequency domain. These are system constants so this is work that should be done ahead
//in an application with filter design capabilities like MatLab or somesuch.
//Note, the impulse response of the filter can be a lower degree than the filter,
//but it does need to be padded out to the length of the input in order to meet the standards of the PPN.

//Note this is just the include file - the coefficients should actually be set in the cpp file cuz why
//would using const globals of all things be easy?

//I know this isn't actually a matrix class, but it's a matrix in the literature and we iterate it like one
extern const std::array<std::array<FIRFilter<PPN_length>, Downsample_Factor>,
	FilterBank_channels> Analysis_Freq_Filter_Matrix;

extern const std::array<std::array<FIRFilter<PPN_length>, Downsample_Factor>,
	FilterBank_channels> Synthesis_Freq_Filter_Matrix;

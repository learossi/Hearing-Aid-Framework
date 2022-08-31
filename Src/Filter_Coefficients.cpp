#include "Filter_Coefficients.h"

//You can uncomment the below when you initialize it. I don't wanna deal w/ the compiler error.


const std::array<std::array<FIRFilter<PPN_length>, Downsample_Factor>,
	FilterBank_channels> Analysis_Freq_Filter_Matrix{};

const std::array<std::array<FIRFilter<PPN_length>, Downsample_Factor>,
	FilterBank_channels> Synthesis_Freq_Filter_Matrix{};


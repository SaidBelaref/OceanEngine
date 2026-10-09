#include "OceanWaveShader.h"

IMPLEMENT_GLOBAL_SHADER(
	FOceanWaveCS,
	"/OceanEngine/OceanWave.usf",
	"OceanWaveCS",
	SF_Compute
);

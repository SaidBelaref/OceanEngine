#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

class FOceanWaveCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FOceanWaveCS);
	SHADER_USE_PARAMETER_STRUCT(FOceanWaveCS, FGlobalShader);

public:
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutputTexture)
		SHADER_PARAMETER(float, Time)
		SHADER_PARAMETER(float, Amplitude)
		SHADER_PARAMETER(float, Wavelength)
		SHADER_PARAMETER(float, Speed)
		SHADER_PARAMETER(float, TileSize)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
};

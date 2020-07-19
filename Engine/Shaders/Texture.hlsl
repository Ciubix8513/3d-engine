//globals
SamplerState ss ;
Texture2D shaderTex;

struct PixelInput
{
	float4 position: SV_POSITION;
	float4 color : COLOR;
	float2 UV: TEXCOORD0;

};


float4 TexturePixelShader(PixelInput input) : SV_TARGET
{
	float4 texColors;

	texColors = shaderTex.Sample(ss, input.UV);

	return texColors * input.color;
}
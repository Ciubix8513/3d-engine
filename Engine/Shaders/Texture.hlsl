SamplerState ss;
Texture2D shaderTex;

struct PixelInput
{
	float4 position: SV_POSITION;
	float2 UV: TEXCOORD0;

};

float4 TexturePixelShader(PixelInput input) : SV_TARGET
{
	float4 texColors;
	texColors = shaderTex.Sample(ss, input.UV);
    return texColors;
}
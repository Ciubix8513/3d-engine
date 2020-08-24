//globals
SamplerState ss ;
Texture2D shaderTex;
cbuffer PixelBuffer
{
    float4 pixelColor;
};


struct PixelInput
{
	float4 position: SV_POSITION;
	float2 UV: TEXCOORD0;
};


float4 FontPixelShader(PixelInput input) : SV_TARGET
{
	float4 Color;
	Color = shaderTex.Sample(ss, input.UV);
	
    if (Color.r == 0.0f)
    {
     Color.a = 0.0f;
    }
   else
   {
        Color.a = 1.0f;
        Color = Color * pixelColor;
   }

    return Color;

}
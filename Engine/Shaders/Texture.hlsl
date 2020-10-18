//globals
SamplerState ss ;
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
/*
    if (texColors.r > .8f)
    {
        texColors.a = 0.0f;
        texColors.rgb = float3(0, 0, 0);

    }
    else
    {
        texColors.a = 1.0f;
    }
	*/
	
    //texColors.a = 1.0f;
    
    return texColors;//    float4(1, 1, 1, 1);
 

}
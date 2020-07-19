SamplerState ss;
Texture2D shaderTex;

cbuffer nLightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float pading;
};

struct PixelInput
{
	float4 position: SV_POSITION;
	//float4 color : COLOR;
	float2 UV: TEXCOORD0;
	float3 normal : NORMAL;
};

float4 LightPixelShader(PixelInput input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntencity;
    float4 color = float4(1,1,1,1);

	textureColor = shaderTex.Sample(ss, input.UV);
	lightDir = -lightDirection;

	lightIntencity = saturate(dot(input.normal, lightDirection));

    if(lightIntencity > 0.0f)
        color += (diffuseColor * lightIntencity);
	
	
    color = saturate( lightIntencity);



	color = color * textureColor;
    //return float4(1, 1, 1, 1);
	return color;
}
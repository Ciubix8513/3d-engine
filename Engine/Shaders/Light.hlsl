SamplerState ss;
Texture2D shaderTex;

cbuffer LightBuffer
{
    float4 AmbientColor;
    float4 diffusecolor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

struct PixelInput
{

	float4 position: SV_POSITION;
    float3 viewDir : TEXCOORD1;
	float2 UV: TEXCOORD0;
	float3 normal : NORMAL;
};

float4 LightPixelShader(PixelInput input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntencity;
    float4 color;
    float3 reflection;
    float4 specular;

	
	textureColor = shaderTex.Sample(ss, input.UV);
    color = AmbientColor;
	
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	lightDir = -lightDirection;

	lightIntencity = saturate(dot(input.normal, lightDir));
	
    if (lightIntencity > 0.0f)
    {    
        color += (diffusecolor * lightIntencity);
        color = saturate( color);
        
        reflection = normalize(2 * lightIntencity * input.normal - lightDir);
        specular = pow(saturate(dot(reflection, input.viewDir)), specularPower);

    }
	
	color = color * textureColor;
  
    color = saturate(color + specular);
          

    return color;
}
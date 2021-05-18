SamplerState ss;

Texture2D a;
Texture2D b;
Texture2D c;
Texture2D d;
Texture2D e;
Texture2D f;



struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 UV : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 TexturePixelShader(PixelInput input) : SV_TARGET
{
    float4 texColors;  
    
    if(input.UV.x < 0.25f)
        texColors = a.Sample(ss, float2(input.UV.x * 4, (input.UV.y - 0.5f) * 4));
    else if (input.UV.x > 0.75f)
        texColors = c.Sample(ss, float2((input.UV.x - 0.5f) * 4, (input.UV.y - 0.5f) * 4));
    else if (input.UV.y < 0.25f)
        texColors = f.Sample(ss, float2((input.UV.x - 0.25f) * 4, (input.UV.y - 0.75f) * 4));
    else if (input.UV.y > 0.75f)
        texColors = d.Sample(ss, float2((input.UV.x - 0.25f) * 4, (input.UV.y - 0.75f) * 4)); 
    else if (input.UV.y > 0.5f)
        texColors = b.Sample(ss, float2((input.UV.x - 0.25f) * 4, (input.UV.y - 0.5f) * 4));
    else 
        texColors = e.Sample(ss, float2((input.UV.x - 0.25f) * 4, (input.UV.y - 0.25f) * 4));
   
        return texColors;
}
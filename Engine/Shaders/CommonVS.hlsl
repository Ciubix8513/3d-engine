
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct Vertex
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 UV : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 position : SV_POSITION;    
    float4 color : COLOR;
    float2 UV : TEXCOORD0;
    float3 normal : NORMAL;
};

PixelInput main(Vertex input)
{
    PixelInput output;
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
   
    float4 normal = mul(normal, worldMatrix);
    normal = normalize(normal);  
    
    output.normal = normal.xyz;
    
	
    output.UV = input.UV;    

    return output;
}
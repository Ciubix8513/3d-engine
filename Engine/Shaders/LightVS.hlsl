
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix objectMat;
};
cbuffer CamBuffer
{
    float3 camPosition;
    float padding;
};

//stucts
struct Vertex
{
	float4 position: POSITION;
	float4 color : COLOR;
	float2 UV: TEXCOORD0;
	float4 normal : NORMAL;
};
struct PixelInput
{
	float4 position: SV_POSITION;
    float3 viewDir : TEXCOORD1;
	float2 UV: TEXCOORD0;
	float4 normal : NORMAL;
};


PixelInput LightVertexShader(Vertex input) 
{
	PixelInput output;
    float4 worldPosition;
	
	input.position.w = 1.0f;
	output.position = mul(input.position, objectMat);
    output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);	
	
    input.normal.w = 0;
	output.UV = input.UV;
	output.normal = mul(input.normal,  objectMat);
	output.normal = mul(output.normal,  worldMatrix);
	output.normal = normalize(output.normal);

    worldPosition = mul(input.position, objectMat);
	worldPosition = mul(worldPosition, worldMatrix);
	
    output.viewDir = camPosition.xyz - worldPosition.xyz;
	
    output.viewDir = normalize(output.viewDir);
    
   
	return output;
}
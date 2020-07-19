
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix objectMat;
};
//stucts
struct Vertex
{
	float4 position: POSITION;
	float4 color : COLOR;
	float2 UV: TEXCOORD0;
	float3 normal : NORMAL;
};
struct PixelInput
{
	float4 position: SV_POSITION;
	//float4 color : COLOR;
	float2 UV: TEXCOORD0;
	float3 normal : NORMAL;
};


PixelInput LightVertexShader(Vertex input) 
{
	PixelInput output;
	
	
	input.position.w = 1.0f;
	output.position = mul(input.position, objectMat);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);	
	
	output.UV = input.UV;
	output.normal = mul(input.normal, (float3x3) objectMat);
	output.normal = mul(output.normal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}
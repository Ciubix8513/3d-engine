//globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};
//structs
struct Vertex 
{
	float4 position: POSITION;
	float4 color : COLOR;
	//float2 UV : TEXCOORD0;
};
struct PixelInput
{
	float4 position: SV_POSITION;
	float4 color : COLOR;
};


PixelInput ColorVertexShader(Vertex input)
{
	PixelInput output;
	input.position.w = 1.0f;
	output.position = mul(input.position, 1);
    output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.color = input.color;

	return output;
}
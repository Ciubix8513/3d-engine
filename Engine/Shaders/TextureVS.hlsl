//globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix objectMatrix;
};
//stucts
struct Vertex
{
	float4 position: POSITION;
	float2 UV: TEXCOORD0;
};
struct PixelInput
{
	float4 position: SV_POSITION;
	float2 UV: TEXCOORD0;

};


PixelInput TextureVertexShader(Vertex input)
{
	
	
	PixelInput output;
	input.position.w = 1.0f;
//	output.position = mul(input.position, objectMatrix);
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    output.UV = input.UV;
    //output.UV = float2(1, 1);

	return output;
}
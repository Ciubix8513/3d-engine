struct PixelInput
{
	float4 position: SV_POSITION;
	float4 color : COLOR;
};



float4 ColorPixelShader(PixelInput input) : SV_TARGET
{
    return float4(1, 1, 1, 1);
	
	//input.color / 2;
}
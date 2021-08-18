struct VSIn {
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VSOut {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VSOut main(VSIn input)
{
	VSOut vso;
	//vso.position = (input.position.x, input.position.y, input.position.z, 1.0f);
	vso.position = float4(input.position, 1.0f);
	vso.color = input.color;
	return vso;
}
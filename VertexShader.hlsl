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
	vso.position = (input.position, 0.0);
	vso.color = input.color;
	return vso;
}
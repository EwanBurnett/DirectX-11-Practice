cbuffer Const0{
	matrix mWorldView;
}

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
	vso.position = mul(float4(input.position, 1.0f), mWorldView);
    vso.color = float4(0.5, 0.5, 0.5, 1.0);
	return vso;
}
struct VSOut {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 main(VSOut pin) : SV_Target
{
	return pin.color;
}
cbuffer CBufferPerObject
{
    float4x4 gWorldViewProj;
};

struct VertIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
};

VertOut VS(VertIn input)
{
    VertOut vOut;
    //Transform verts into homogenous clip space
    vOut.PosH = mul(float4(input.pos, 1.0f), gWorldViewProj);
    
    //Pass vertex colour into the pixel shader
    vOut.Color = input.color;
    
    return vOut;
}

float4 PS(VertOut input) : SV_Target
{
    return input.Color;
}


technique11 ColourTech
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS() ));
        SetPixelShader(CompileShader(ps_5_0, PS() ));
    }
}
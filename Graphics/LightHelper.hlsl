struct Material
{
    float4 Ambient;
    float4 Specular;	//w = Specular Power
    float4 Diffuse;	
    float4 Reflect;
};

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Direction;
	float pad;
};

struct PointLight {
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Direction;
    float Spot;

    float3 Position;
    float Range;

    float3 Att;
    float pad;
};

void ComputeDirectionalLight(
	Material mat, DirectionalLight L, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec
) 
{
	//Initialize Output
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Light vector is opposite to the light rays
	float3 LightVec = -L.Direction;

	//Calculate ambient term
	ambient = mat.Ambient * L.Ambient;

	//Add Difuse and specular term
	float diffuseFactor = dot(LightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f) {
		float3 v = reflect(-LightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}

void main()
{
	
}
#version 330

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct PointLight
{
	BaseLight base;
	Attenuation atten;
	vec3 pos;
	float range;
};


uniform PointLight pLight;

vec4 calcPointLight(PointLight base);

vec4 getColor();

void main()
{
	gl_FragColor = calcPointLight(pLight) * getColor();
}
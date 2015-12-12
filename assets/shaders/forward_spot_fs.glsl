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

struct SpotLight
{
	PointLight point;
	vec3 direction;
	float cutoff;
};


uniform SpotLight sLight;

uniform float specularIntensity;
uniform float specularExponent;

vec4 calcPointLight(PointLight point);
vec4 calcSpotLight(SpotLight spotLight);

vec4 getColor();

void main()
{
	gl_FragColor = calcSpotLight(sLight) * getColor();
}
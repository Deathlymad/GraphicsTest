#version 330


struct BaseLight
{
	vec3 color;
	float intensity;
};
struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

uniform DirectionalLight Light;

uniform float specularIntensity;
uniform float specularExponent;

vec4 calcLight(BaseLight base, vec3 direction);

vec4 getColor();

void main()
{
	gl_FragColor = calcLight(Light.base, Light.direction) * getColor();
}
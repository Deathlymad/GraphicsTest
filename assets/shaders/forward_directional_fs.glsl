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


vec4 calcLight(BaseLight base, vec3 direction);

vec4 calcDirectionalLight(DirectionalLight directionalLight)
{
    return calcLight(directionalLight.base, -directionalLight.direction);
}

vec4 getColor();

void main()
{
	gl_FragColor =  calcDirectionalLight(Light) * getColor() ;//+ vec4(1.0, 0.0, 1.0, 1.0);
}
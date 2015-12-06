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


in vec3 world_Pos;
in vec3 normal;
uniform DirectionalLight Light;


vec4 calcLight(BaseLight base, vec3 direction, vec3 normal, vec3 worldPos);
vec4 getColor();

vec4 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 worldPos)
{
    return calcLight(directionalLight.base, -directionalLight.direction, normal, worldPos);
}

void main()
{
	gl_FragColor = calcDirectionalLight(Light, normal, world_Pos) * getColor();
}
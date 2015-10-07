#version 330

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

in vec4 world_Pos;
in vec2 texCoord;
in vec3 normal;

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
};

struct SpotLight
{
	PointLight point;
	vec3 direction;
	float cutoff;
};

uniform vec3 EyePos;
uniform sampler2D sampler;
uniform float specularIntensity;
uniform float specularExponent;

uniform vec3 ambientLight;
uniform DirectionalLight Light;

vec4 calcLight( BaseLight base, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);
    
    vec4 diffuseColor = vec4(0,0,0,0);
    vec4 specularColor = vec4(0,0,0,0);
    
    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
        
        vec3 directionToEye = normalize(EyePos - world_Pos).xyz;
        vec3 reflectDirection = normalize(reflect(direction, normal));
        
        float specularFactor = dot(directionToEye, reflectDirection);
        specularFactor = pow(specularFactor, specularExponent);
        
        if(specularFactor > 0)
        {
            specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
        }
    }
    
    return diffuseColor + specularColor;
}

vec4 calcDirectionalLight( DirectionalLight directional, vec3 normal)
{
	return calcLight( directional.base, normalize(directional.direction), normalize(normal));
}

void main()
{
	BaseLight base;
	base.color = vec3( 0.3, 0.8, 0.2);
	base.intensity = 1;
	DirectionalLight dir;
	dir.base = base;
	dir.direction = vec3( 0, 1, 0);
	gl_FragColor = vec4(ambientLight, 1) + calcDirectionalLight( dir, normal);
	//gl_FragColor = vec4( 0, 1, 0, 1);
}
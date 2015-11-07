#version 330

in vec2 texCoord;
in vec3 world_Pos;
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

uniform vec3 EyePos;
uniform float specularIntensity;
uniform float specularExponent;

uniform DirectionalLight Light;

vec4 calcLight( BaseLight base, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);
    
    vec4 diffuseColor = vec4(0,0,0,0);
    vec4 specularColor = vec4(0,0,0,0);
    
    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
        
        vec3 directionToEye = normalize(EyePos - world_Pos);
        //vec3 reflectDirection = normalize(reflect(direction, normal));
        vec3 halfDirection = normalize(directionToEye - direction);
        
        float specularFactor = dot(halfDirection, normal);
        //float specularFactor = dot(directionToEye, reflectDirection);
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
	return calcLight( directional.base, -normalize(directional.direction), normalize(normal));
}

void main()
{
	gl_FragColor = /* calcDirectionalLight( Light, normal) */ vec4(0.0, 0.0, 1.0, 1.0);
}
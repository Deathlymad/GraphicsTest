#version 330

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

vec4 calcLight(BaseLight base, vec3 direction, vec3 nor)
{
    float diffuseFactor = dot(normal, direction);
    
	if (diffuseFactor < 0)
		diffuseFactor *= -1;

    vec4 diffuseColor = vec4(1,0,0,1);
    vec4 specularColor = vec4(0,1,0,1);
    
    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
        
        vec3 directionToEye = normalize(EyePos - world_Pos).xyz;
        vec3 reflectDirection = normalize(reflect(direction, normal));
        
        float specularFactor = pow(dot(directionToEye, reflectDirection), specularExponent);
        
        if(specularFactor > 0)
        {
            specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
        }
    }
    
    return diffuseColor + specularColor;
}

vec4 calcDirectionalLight( DirectionalLight directional, vec3 nor)
{
	return calcLight( directional.base, normalize(directional.direction), normalize(nor));
}

void main()
{
	gl_FragColor = vec4(normal, 1.0); //calcDirectionalLight(Light, normal);
}
#version 330


in vec3 worldPos;
in vec3 normal;

uniform vec3 EyePos;
uniform float specularIntensity0;
uniform float specularExponent0;

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

vec4 calcLight(BaseLight base, vec3 direction)
{
    float dif = dot(normal, -direction);
    
    vec4 difCol = vec4(0,0,0,0);
    vec4 specCol = vec4(0,0,0,0);
    
    if(dif > 0)
    {
        difCol = vec4(base.color, 1.0) * base.intensity * dif;
        
        vec3 dirToEye = normalize(EyePos - worldPos);
        vec3 hDir = normalize(dirToEye - direction);
        
        float spec = dot(hDir, normal);
        spec = pow(spec, specularExponent0);
        
        if(spec > 0)
        {
            specCol = vec4(base.color, 1.0) * spec * specularIntensity0;
        }
    }
    
    return difCol + specCol;
}

vec4 calcPointLight(PointLight pointLight)
{
    vec3 lightDirection = worldPos - pointLight.pos;
    float distanceToPoint = length(lightDirection);
    
    if(distanceToPoint > pointLight.range)
        return vec4(0,0,0,0);
    
    lightDirection = normalize(lightDirection);
    
    vec4 color = calcLight(pointLight.base, lightDirection);
    
    float attenuation = pointLight.atten.constant + pointLight.atten.linear * distanceToPoint + pointLight.atten.exponent * distanceToPoint * distanceToPoint + 0.0001;
                         
    return color / attenuation;
}

vec4 calcSpotLight(SpotLight spotLight)
{
    vec3 lightDirection = normalize(worldPos - spotLight.point.pos);
    float spotFactor = abs(dot(lightDirection, spotLight.direction));
    
    vec4 color = vec4(0,0,0,0);
    
    if(spotFactor < spotLight.cutoff)
    {
		color = calcPointLight(spotLight.point) * (1.0 - (1.0 - spotFactor)/(1.0 - spotLight.cutoff));
    }
    
    return color;
}
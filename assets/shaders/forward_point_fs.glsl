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


in vec3 worldPos;
in vec2 texCoord;
in vec3 normal;

uniform sampler2D tex;
uniform vec3 EyePos;
uniform float specularIntensity0;
uniform float specularExponent0;

uniform PointLight Light;

vec4 calcLight( BaseLight base, vec3 direction)
{
	vec3 n = normalize(normal);
	float dif = dot(n, -direction);
	float spec = 0;
    
    vec4 difCol = vec4(0,0,0,0);
    vec4 specCol = vec4(0,0,0,0);
    
    if(dif > 0)
    {
        difCol = vec4(base.color, 1.0) * base.intensity * dif;
        
        vec3 dirToEye = normalize(EyePos - worldPos);
        vec3 hDir = normalize(dirToEye - direction);
        
        spec = dot(hDir, n);
        spec = pow(spec, specularExponent0);
        
        if(spec > 0)
        {
            specCol = vec4(base.color, 1.0) * spec * specularIntensity0;
        }
    }
	return (difCol + specCol) * texture2D(tex, texCoord);
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
                         
    return color/attenuation;
}

void main()
{
	gl_FragColor = calcPointLight(Light);
}
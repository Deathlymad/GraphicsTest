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


uniform SpotLight Light;

in vec3 worldPos;
in vec2 texCoord;
in vec3 normal;

uniform sampler2D _tex0;
uniform vec3 EyePos;
uniform float specularIntensity0;
uniform float specularExponent0;

vec4 calcPointLight(PointLight pointLight)
{
    vec3 lightDirection = worldPos - pointLight.pos;
    float distanceToPoint = length(lightDirection);
    
    if(distanceToPoint > pointLight.range)
        return vec4(0,0,0,0);
    
    lightDirection = normalize(lightDirection);
    
	vec3 n = normalize(normal);
	float dif = dot(n, -lightDirection);
	float spec = 0;
    
    vec4 difCol = vec4(0,0,0,0);
    vec4 specCol = vec4(0,0,0,0);
    
    if(dif > 0)
    {
		vec3 temp = pointLight.base.color * pointLight.base.intensity * dif;
        difCol = vec4( temp, length(temp));
        
        vec3 dirToEye = normalize(EyePos - worldPos);
        vec3 hDir = normalize(dirToEye - lightDirection);
        
        spec = dot(hDir, n);
        spec = pow(spec, specularExponent0);
        
        if(spec > 0)
        {
			temp = pointLight.base.color * spec * specularIntensity0;
            specCol = vec4( temp, length(temp));
        }
    }
	vec4 color = (difCol + specCol) * texture2D(_tex0, texCoord);

    float attenuation = pointLight.atten.constant + pointLight.atten.linear * distanceToPoint + pointLight.atten.exponent * distanceToPoint * distanceToPoint + 0.0001;
    
    return color/attenuation;
}

void main()
{
	vec3 lightDirection = normalize(worldPos - Light.point.pos);
    float spotFactor = dot(lightDirection, Light.direction);
    
    vec4 color = vec4(0,0,0,0);
    
    if(spotFactor > Light.cutoff)
    {
		color = calcPointLight(Light.point) * (1.0 - (1.0 - spotFactor)/(1.0 - Light.cutoff));
    }

	gl_FragColor = color;
}

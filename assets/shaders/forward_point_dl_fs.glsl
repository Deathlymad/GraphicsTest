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

uniform sampler2D _tex0;
uniform vec3 EyePos;
uniform float specularIntensity0;
uniform float specularExponent0;

uniform PointLight Light;

void main()
{
    vec3 lightDirection = worldPos - Light.pos;
    float distanceToPoint = length(lightDirection);
    
    if(distanceToPoint > Light.range)
	{
        gl_FragColor = vec4(0,0,0,0);
		return;
	}
    
    lightDirection = normalize(lightDirection);


    vec3 n = normalize(normal);
	float dif = dot(n, -lightDirection);
	float spec = 0;
    
    vec4 difCol = vec4(0,0,0,0);
    vec4 specCol = vec4(0,0,0,0);
    
    if(dif > 0)
    {
		vec3 temp = Light.base.color * Light.base.intensity * dif;
        difCol = vec4( temp, length(temp));
        
        vec3 dirToEye = normalize(EyePos - worldPos);
        vec3 hDir = normalize(dirToEye - lightDirection);
        
        spec = dot(hDir, n);
        spec = pow(spec, specularExponent0);
        
        if(spec > 0)
        {
			temp = Light.base.color * spec * specularIntensity0;
            specCol = vec4( temp, length(temp));
        }
    }
	vec4 color =  (difCol + specCol) * texture2D(_tex0, texCoord);

    float attenuation = Light.atten.constant + Light.atten.linear * distanceToPoint + Light.atten.exponent * distanceToPoint * distanceToPoint + 0.0001;
    
	gl_FragColor = color / attenuation;
}

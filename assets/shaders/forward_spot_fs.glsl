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

void main()
{
    
    vec4 color = vec4(0,0,0,0);

	vec3 lightDirection = worldPos - Light.point.pos;
	float distanceToPoint = length(lightDirection);
    
	float spec = 0;
    
	vec4 difCol = vec4(0,0,0,0);
	vec4 specCol = vec4(0,0,0,0);

	float spot = (1.0 - dot(lightDirection, Light.direction))/(1.0 - Light.cutoff);
	if (spot > 0 && distanceToPoint <= Light.point.range)
	{
		lightDirection = normalize(lightDirection);
    
		vec3 n = normalize(normal);
		float dif = dot(n, -lightDirection);
    
		if(dif > 0)
		{
			difCol = vec4(Light.point.base.color, 1.0) * Light.point.base.intensity * dif;
        
			vec3 dirToEye = normalize(EyePos - worldPos);
			vec3 hDir = normalize(dirToEye - lightDirection);
        
			spec = dot(hDir, n);
			spec = pow(spec, specularExponent0);
        
			if(spec > 0)
			{
				specCol = vec4(Light.point.base.color, 1.0) * spec * specularIntensity0;
			}
		}
		color = (difCol + specCol) ;// * texture2D(_tex0, texCoord);

		float attenuation = Light.point.atten.constant + Light.point.atten.linear * distanceToPoint + Light.point.atten.exponent * distanceToPoint * distanceToPoint + 0.0001;
    
		color *= spot / attenuation;
    }

	gl_FragColor = color;
	
}

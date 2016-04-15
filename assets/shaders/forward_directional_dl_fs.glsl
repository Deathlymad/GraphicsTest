#version 330

in vec3 worldPos;
in vec2 texCoord;
in vec3 normal;

uniform sampler2D _tex0;
uniform vec3 EyePos;
uniform float specularExponent0;
uniform float specularIntensity0;

struct DirectionalLight
{
	vec3 color;
	float intensity;
	vec3 direction;
};

uniform DirectionalLight Light;

void main()
{
	vec3 n = normalize(normal);
	float dif = dot(n, -Light.direction);
	float spec = 0;
    
    vec4 difCol = vec4(0,0,0,0);
    vec4 specCol = vec4(0,0,0,0);
    
    if(dif > 0)
    {
		vec3 temp = Light.color * Light.intensity * dif;
        difCol = vec4( temp, length(temp));
        
        vec3 dirToEye = normalize(EyePos - worldPos);
        vec3 hDir = normalize(dirToEye - Light.direction);
        
        spec = dot(hDir, n);
        spec = pow(spec, specularExponent0);
        
        if(spec > 0)
        {
			temp = Light.color * spec * specularIntensity0;
            specCol = vec4( temp, length(temp));
        }
    }

	gl_FragColor = (difCol + specCol) * texture2D(_tex0, texCoord);
}

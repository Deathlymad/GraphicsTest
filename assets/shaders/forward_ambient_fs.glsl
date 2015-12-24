#version 330

uniform float specularIntensity0;
uniform float specularExponent0;
uniform vec3 EyePos;

vec4 getColor();

void main()
{
	float temp = min(1.0f, specularIntensity0 + specularExponent0 + EyePos.x);
	gl_FragColor = vec4(0.3, 0.3, 0.3, 1.0) * getColor();
}
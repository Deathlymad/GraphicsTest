#version 330


in vec2 texCoord;
uniform sampler2D tex;

vec4 getColor()
{
	return texture2D( tex, texCoord);
}
#version 330


in vec2 texCoord;
uniform sampler2D tex;

vec4 getColor()
{
	vec4 col = texture2D( tex, texCoord);
	if (length(col) > 0)
	{
		return col;
	} else
	return vec4(1.0, 1.0, 1.0, 1.0);
}
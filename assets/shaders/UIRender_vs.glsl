#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 texCoord;

void main()
{
	texCoord = tex;
	gl_Position = vec4(pos.xy, 0.0, 1.0);
}

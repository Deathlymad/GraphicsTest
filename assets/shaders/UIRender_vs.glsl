#version 330

layout (location = 0) in vec3 pos;

out vec2 texCoord;

void main()
{
	texCoord = pos.xy;
	gl_Position = vec4(pos.xy, 0.0, 1.0);
}
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

out vec2 texCoord;

uniform float xRatio;
uniform float yRatio;

void main()
{
	texCoord = vec2(tex.x * xRatio, tex.y * yRatio);
	gl_Position = vec4(pos.xyz, 1.0);
}

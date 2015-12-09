#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 texCoord;

uniform mat4 View;

void main()
{
   texCoord = tex;
   gl_Position = View * vec4( pos, 1);
}

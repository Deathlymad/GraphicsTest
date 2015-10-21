#version 330

layout (location = 0) in vec3 pos;
uniform mat4 View;

void main()
{
   gl_Position = View * vec4( pos, 1);
}
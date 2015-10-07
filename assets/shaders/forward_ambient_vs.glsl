#version 330

layout (location = 0) in vec3 pos;
uniform mat4 Matrix;

void main()
{
   gl_Position = vec4( pos, 1);
}
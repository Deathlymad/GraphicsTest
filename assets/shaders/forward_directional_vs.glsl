#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 Normal;

out vec3 normal;
out vec3 world_Pos;
out vec2 texCoord;

uniform mat4 View;

void main()
{
   normal = Normal;
   world_Pos = pos;
   texCoord = vec2(-1.0, -1.0);
   gl_Position = View * vec4( pos, 1);
}
#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

out vec2 texCoord;
out vec3 normal;
out vec3 world_Pos;

uniform mat4 Matrix;

void main()
{
   normal = Normal;
   texCoord = TexCoord;
   world_Pos = pos;
   gl_Position = Matrix * vec4( pos, 1);
}
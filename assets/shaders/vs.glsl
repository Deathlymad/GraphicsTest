#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

out vec4 world_Pos;
out vec2 texCoord;
out vec3 normal;

uniform mat4 Matrix;
uniform mat4 NMatrix;
uniform mat4 MMatrix;

void main()
{
   world_Pos = MMatrix * vec4(pos, 1);
   normal = Normal;
   texCoord = TexCoord;
   gl_Position = Matrix * vec4( pos, 1);
}
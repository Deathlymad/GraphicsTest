#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 Normal;

out vec3 normal;
out vec3 worldPos;
out vec2 texCoord;

uniform mat4 ViewProj;

void copyAttribData()
{
   normal = Normal;
   worldPos = pos;
   texCoord = tex;
   gl_Position = ViewProj * vec4( pos, 1);
}
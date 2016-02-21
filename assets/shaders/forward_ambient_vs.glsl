#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 Normal;

out vec3 normal;
out vec3 worldPos;
out vec2 texCoord;

uniform mat4 ViewProj;
uniform mat4 TransMatrix0;

void main()
{
   normal = Normal;
   worldPos = (TransMatrix0 * vec4(pos, 1)).xyz;
   texCoord = tex;
   gl_Position = ViewProj * (TransMatrix0 * vec4( pos, 1));
}

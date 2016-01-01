#version 330

layout (location = 0) in vec3 pos;

out vec2 texCoord;

void main()
{
	//tex Coord for rectangle might need  to be modified
	if(pos.x < 0)
		texCoord.x = -1;
	else
		texCoord.x = 1;
	
	if(pos.y < 0)
		texCoord.y = -1;
	else
		texCoord.y = 1;

	gl_Position = vec4(pos.xy, 0.0, 1.0);
}
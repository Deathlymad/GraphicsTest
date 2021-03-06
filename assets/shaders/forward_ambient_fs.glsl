#version 330

in vec2 texCoord;
uniform sampler2D _tex0;

vec4 getColor();

void main()
{
	texture2D( _tex0, texCoord);
	gl_FragColor = vec4(0.3, 0.3, 0.3, 1.0);
}

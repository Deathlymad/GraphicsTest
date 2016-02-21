#version 330

in vec2 texCoord;
uniform sampler2D _tex0;

void main()
{
	gl_FragColor = texture2D( _tex0, texCoord);
}

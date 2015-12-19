#version 330

vec4 getColor();

void main()
{
		gl_FragColor = vec4(0.3, 0.3, 0.3, 1.0) * getColor();
}
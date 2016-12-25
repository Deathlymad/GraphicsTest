#include <GL\glew.h>

#pragma once
struct deleteGLBuffer //needs Size
{
	deleteGLBuffer() {}
	void operator()(GLuint* buf) const
	{
		if (glIsBuffer(buf[0]))
			glDeleteBuffers(int(sizeof(buf) / sizeof(GLuint)), buf);
	}
};
struct deleteGLVAO
{
	deleteGLVAO() {}
	void operator()(GLuint* buf) const
	{
		if (glIsBuffer(buf[0]))
			glDeleteBuffers(int(sizeof(buf) / sizeof(GLuint)), buf);
	}
};
struct deleteGLShader
{
	deleteGLShader() {}
	void operator()(GLuint* s) const
	{
		if (s == nullptr)
			return;
		else if (*s != -1)
			if (glIsShader(*s))
				glDeleteShader(*s);
		delete s;
	}
};
struct deleteGLProgram
{
	deleteGLProgram() {}
	void operator()(GLuint* prgm) const
	{
		if (glIsProgram(*prgm))
			glDeleteProgram(*prgm);
	}
};
struct deleteGLTexture //needs Size
{
	deleteGLTexture() {}
	void operator()(GLuint* tex) const
	{
		if (glIsTexture(*tex))
			glDeleteTextures(1, tex);
	}
};
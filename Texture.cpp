
#define PRAGMALIB
#ifndef GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#ifdef PRAGMALIB
#pragma comment(lib, "glew32s.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#endif
#undef __glewActiveTexture
#undef __glewGenerateMipmap
#endif

#include <iostream>
#include "Shader.h"
#include "Texture.h"
#include "bmp.h"

extern bool GraphicsInitialized;

vector<Texture*> Texture::SamplerList = vector<Texture*>();

GLuint* Texture::last = nullptr;

void Texture::deleteTexture(GLuint* tex) { if (glIsTexture(*tex)) glDeleteTextures(1, tex); }

Texture::Texture(string fileName) : ID([this](GLuint* tex) {deleteTexture(tex); }, new GLuint)
{
	isLoaded = false;
	tex = nullptr;
	texX = 0;
	texY = 0;

	f = fileName;

	load(fileName);
	pos = (unsigned char)SamplerList.size();
	SamplerList.push_back(this); //TODO: fill holes, algorithms allow deletion
}

void Texture::setTexture( string File)
{
	isLoaded = false;
	f = File;
	load(f);
}

void Texture::load(string fileName)
{
	int width, height;
	char* buffer =loadbmp( fileName, width, height);

	if (tex == nullptr)
	{
		if (texX == 0)
			texX = width;
		if (texY == 0)
			texY = height;

		tex = buffer;
	}
	//add Texture to existing Atlas
 }

vector<vector<char>> Texture::getTexData()
{
	vector<vector<char>> t;
	for (unsigned int x = 0; x < texX; x++)
	{
		t.push_back(vector<char>());
		for (unsigned int y = 0; y < texY; y++)
		{
			t[x].push_back(tex[x*texX + y]);
		}
	}
	return t;
}

void Texture::glDownload()
{
	//set sampler to 0
	glActiveTexture( GL_TEXTURE0);
    glGenTextures(1, ID.get());



    glBindTexture(GL_TEXTURE_2D, *(ID.get()));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, texX, texY, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture( GL_TEXTURE_2D, *(ID.get()));

	glGenerateMipmap(GL_TEXTURE_2D);

	isLoaded = true;
}

void Texture::bind()
{
	if (last != ID.get())
	{
		glBindTexture(GL_TEXTURE_2D, *(ID.get())); //Errors 
		last = ID.get();
	}
	GLenum err = glGetError();
}

vector<vector<char>> mergeTexData( vector<vector<char>> tex1, vector<vector<char>> tex2, unsigned int partX, unsigned int partY, unsigned short partsPerLine)
{
	if( tex1.size() == 0 || tex1[0].size() == 0)
		return tex2;
	if (tex2.size() == 0 || tex2[0].size() == 0)
		return tex1;

	unsigned int i;
	for (i = 0; i < tex1.size();)
	{
		i += partY;
		if(tex1[i].size() < partX * partsPerLine)
			break;
	}

	unsigned int startX = tex1[i].size();

	vector<vector<char>> tex = vector<vector<char>>();

	for (unsigned int i = 0; i < tex1.size(); i++) //Copying can be optimized by joining it with other stages
	{
		tex.push_back(vector<char>());
		for (unsigned int j = 0; j < tex1[i].size(); j++)
		{
			tex[i].push_back(tex1[i][j]);
		}
	}

	i = 0;
	for (unsigned int y = i; y < i + partY; y++)
	{
		unsigned int j = 0;
		for (unsigned int x = startX; x < startX + partX; x++)
		{
			tex[y].push_back(tex2[i][j]);
			j++;
		}
		i++;
	}

	return tex;
}

TextureAtlas Texture::operator + (Texture t)
{
	return TextureAtlas(mergeTexData(this->getTexData(), t.getTexData(), texX, texY, 2), texX, texY);
}

TextureAtlas::TextureAtlas(vector<string> Files):Texture()
{
	for (unsigned int i = 0; i < Files.size(); i++)
	{
		*this  = *this + Texture(Files[i]);
	}
}

void TextureAtlas::load(string fileName, unsigned short x, unsigned short y)
{
	if ( bitX == 0 && bitX == x && bitY == 0 && bitY == y) //WTF
	{
		Texture::load( fileName);
		bitX = x;
		bitY = y;
	}
	//TODO see load
	texX += bitX; //TODO two dimensional copying
}

void TextureAtlas::bind()
{
	Texture::bind();
	//bitX
	//bitY
	//partAmtX
	//partAmtY
}
	
TextureAtlas TextureAtlas::operator + (Texture t)
{
	vector<vector<char>> TexData = t.getTexData();
	return TextureAtlas(mergeTexData(this->getTexData(), TexData, bitX, bitY, 8), (unsigned int) TexData.size(), (unsigned int) TexData[0].size());
}
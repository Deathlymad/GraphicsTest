#include <vector>
#include "Util.h"

#pragma once

NSP_UTIL
NSP_STD


typedef unsigned int GLuint;
class TextureAtlas;

class Texture
{
public:
	Texture() : ID([this](GLuint* tex) {deleteTexture(tex); }, new GLuint)
	{
		isLoaded = false;
		f = "";
		tex = nullptr;
		texX = 0;
		texY = 0;
	}

	Texture( vector<vector<char>> tex) : ID([this](GLuint* tex) {deleteTexture(tex); }, new GLuint)
	{
		f = "";
		this->tex = &tex[0][0]; 
		texX = (unsigned short)tex.size();
		texY = (unsigned short)tex[0].size();
	}

	Texture( string fileName);

	void setTexture(string);

	void load( string);

	vector<vector<char>> getTexData();

	void glDownload();

	void bind();

	bool Loaded()
	{
		if (isLoaded)
			return true;
		else
			return false;
	}

	TextureAtlas operator + (Texture);
protected:
	CustomPtr<GLuint> ID; //TextureID
	void deleteTexture(GLuint* tex);

	string f; //filename
	char* tex; //Texture Memory
	unsigned short texX, texY; //size
	bool isLoaded;

	unsigned char pos; //Sampler Count
	static vector<Texture*> SamplerList; //TODO improved sorting update, might not be necessary due to textureAtlas

	static GLuint* last;
};

class TextureAtlas:Texture
{
public:
	TextureAtlas():Texture(){}

	TextureAtlas( vector<vector<char>> tex,unsigned int bitX,unsigned int bitY)
	{
		f = "";
		this->tex = &tex[0][0]; 
		texX = (unsigned short)tex.size();
		texY = (unsigned short)tex[0].size();
		this->bitX = bitX; 
		this->bitY = bitY; 
	}

	TextureAtlas(vector<string>);

	void load(string, unsigned short, unsigned short);

	void bind();
	
	TextureAtlas operator + (Texture);	//TODO: add Merging
private:
	unsigned short bitX, bitY;
};
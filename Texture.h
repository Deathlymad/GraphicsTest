#include <vector>

#pragma once


typedef unsigned int GLuint;
class TextureAtlas;

class Texture
{
public:
	Texture()
	{
		isLoaded = false;
		ID = 0;
		f = "";
		tex = nullptr;
		texX = 0;
		texY = 0;
	}

	Texture( std::vector<std::vector<char>> tex)
	{
		ID = 0;
		f = "";
		this->tex = &tex[0][0]; 
		texX = tex.size();
		texY = tex[0].size();
	}

	Texture( std::string fileName);

	void setTexture(std::string);

	void load( std::string);

	std::vector<std::vector<char>> getTexData();

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
	GLuint ID;

	std::string f;
	char* tex;
	unsigned short texX, texY;
	bool isLoaded;

	unsigned char pos;
	static std::vector<Texture*> SamplerList; //TODO improved sorting update
};

class TextureAtlas:Texture
{
public:
	TextureAtlas():Texture(){}

	TextureAtlas( std::vector<std::vector<char>> tex,unsigned int bitX,unsigned int bitY)
	{
		ID = 0;
		f = "";
		this->tex = &tex[0][0]; 
		texX = tex.size();
		texY = tex[0].size();
		this->bitX = bitX; 
		this->bitY = bitY; 
	}

	TextureAtlas(std::vector<std::string>);

	void load(std::string, unsigned short, unsigned short);

	void bind(); //TODO: Download of Uniform
	
	TextureAtlas operator + (Texture);	//TODO: add Merging
private:
	unsigned short bitX, bitY;
};
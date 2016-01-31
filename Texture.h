#include "Util.h"
#include "UniformRegistry.h"
#include <fstream>

NSP_STD
NSP_UTIL

class Shader;
typedef unsigned int GLuint;
typedef unsigned int GLenum;

#pragma once
class Image
{
public:
	enum ImageType
	{
		BMP,
		PNG
	};
	enum PixelStructure
	{
		A = 1,
		RGB = 3,
		RGBA = 4
	};

	Image();
	Image(string);

	void load(string file);
	char* getData();
	PixelStructure getPixelStructure();

	unsigned int getHeight() { return _height; }
	unsigned int getWidth() { return _width; }

private:
	void load(ifstream& f);
	void format();
	ImageType getFileImageType( ifstream&);

	char* _data;

	PixelStructure _structure;
	ImageType _type;
	unsigned short _depth;
	unsigned int _compression;
	unsigned int _bitMasks[3];
	unsigned int _width, _height;

	char* _colorTable;
	unsigned int _tableSize;
};
#pragma once
class Texture
{
public:
	Texture(Image, unsigned int samplerID = 0);
	~Texture();

	virtual void glDownload();
	virtual void bind();
protected:
	Texture();
	unsigned int _samplerID;
private:
	GLenum getTextureType(Image::PixelStructure);
	void deleteTexture(GLuint* tex);

	CustomPtr<GLuint> _ID;
	static GLuint _lastTexID;
	UniformRegistry _sampler;
	Image _image;
};
#pragma once
class LayeredTexture : public Texture
{
public:
	LayeredTexture(string);
	LayeredTexture(vector<string>);
	LayeredTexture(vector<Image>);
	~LayeredTexture();

	virtual void glDownload();
	virtual void bind();
private:
	vector<Texture> _samplerList;
};
#pragma once
class TextureAtlas : public LayeredTexture
{
public:
	TextureAtlas(string, unsigned int xCount, unsigned int yCount);
	TextureAtlas(vector<string>, unsigned int xCount, unsigned int yCount);
	TextureAtlas(vector<Image>, unsigned int xCount, unsigned int yCount);

	~TextureAtlas();
private:
	unsigned int _countX;
	unsigned int _countY;

	double _xRatio, _yRatio;
	UniformRegistry _xUni, _yUni;
};
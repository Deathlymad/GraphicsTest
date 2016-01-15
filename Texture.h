#include "Util.h"
#include "UniformRegistry.h"
#include <fstream>

NSP_STD
NSP_UTIL

class Shader;
typedef unsigned int GLuint;

#pragma once
class Image
{
public:
	enum ImageType
	{
		BMP,
		PNG
	};

	Image();
	Image(string);

	void load(string file);
	char* getData();

	unsigned int getHeight() { return _height; }
	unsigned int getWidth() { return _width; }

private:
	void load(ifstream& f);
	void format();
	ImageType getFileImageType( ifstream&);

	char* _data;

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
	friend class Layeredtexture;
public:
	Texture(Image);
	~Texture();
	
	virtual void init(Shader* s);

	virtual void glDownload();
	virtual void bind();

private:
	void deleteTexture(GLuint* tex);
	Texture(Image, unsigned int samplerID);

	CustomPtr<GLuint> _ID;
	static GLuint _lastTexID;
	UniformRegistry<1> _sampler;
	unsigned int _samplerID;
	Image _image;
};
/*
#pragma once
class LayeredTexture : public Texture
{
	LayeredTexture();
	~LayeredTexture();

	virtual void glDownload();
	virtual void writeSampler(string name, Shader*s);
	virtual void bind();
public:
	vector<Texture> _sampler;
};

#pragma once
class TextureAtlas : public LayeredTexture
{
	TextureAtlas();
	~TextureAtlas();

	virtual void writeSampler();

public:
	unsigned int _bitX, _countX;
	unsigned int _bitY, _countY;
};
*/
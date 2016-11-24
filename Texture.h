#include "Util.h"
#include "Deletors.h"
#include "UniformRegistry.h"
#include "RessourceHandler.h"
#include <fstream>

NSP_STD
NSP_UTIL

class Shader;
typedef unsigned int GLuint;
typedef unsigned int GLenum;

#pragma once
class Image : public RessourceLoader
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
	Image(string&);
	Image(const Image& other);

	void operator=(const Image& other);

	string getPath() { return path; }
	void load(ifstream& f);
	void* get() { return this; }
	char* getData();
	PixelStructure getPixelStructure();

	unsigned int getHeight() { return _height; }
	unsigned int getWidth() { return _width; }

	~Image()
	{
		_data = nullptr;
		_colorTable = nullptr;
	}
private:
	void format();
	ImageType getFileImageType( ifstream&);

	string path;
	std::unique_ptr<char[]> _data;

	PixelStructure _structure;
	ImageType _type;
	unsigned short _depth;
	unsigned int _compression;
	unsigned int _bitMasks[3];
	int _width, _height;

	unique_ptr<char[]> _colorTable;
	unsigned int _tableSize;
};
#pragma once
class Texture
{
public:
	Texture();
	explicit Texture(string&, unsigned int samplerID = 0);
	explicit Texture(Texture&& other);

	~Texture();

	virtual void load(RessourceHandler&);
	virtual void glDownload();
	virtual void bind();
protected:

	unsigned int _samplerID;
private:
	GLenum getTextureType(Image::PixelStructure);

	unique_ptr<GLuint, deleteGLTexture> _ID;
	static GLuint _lastTexID;
	UniformRegistry _sampler;
	Image _image;
	RessourceHandler::Ressource _imgLink;
};
#pragma once
class LayeredTexture : public Texture
{
public:
	LayeredTexture(string&);
	LayeredTexture(vector<string>&);
	~LayeredTexture();

	virtual void load(RessourceHandler&);
	virtual void glDownload();
	virtual void bind();
private:
	vector<Texture> _samplerList;
};
#pragma once
class TextureAtlas : public LayeredTexture
{
public:
	TextureAtlas(string&, unsigned int xCount, unsigned int yCount);
	TextureAtlas(vector<string>&, unsigned int xCount, unsigned int yCount);

	~TextureAtlas();
private:
	unsigned int _countX;
	unsigned int _countY;

	double _xRatio, _yRatio;
	UniformRegistry _xUni, _yUni;
};
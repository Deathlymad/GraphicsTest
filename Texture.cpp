#include "Texture.h"
#include <exception>
#include <iostream>

#ifndef GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#undef __glewActiveTexture
#undef __glewGenerateMipmap
#endif

#include "Shader.h"
#include "RessourceHandler.h"

GLuint Texture::_lastTexID = -1;

Image::Image() : _data(nullptr), _colorTable(nullptr)
{
	_bitMasks[0] = -1;
	_bitMasks[1] = -1;
	_bitMasks[2] = -1;
}
Image::Image(string fileName) : _data(nullptr), _colorTable(nullptr)
{
	path = fileName;
	_bitMasks[0] = -1;
	_bitMasks[1] = -1;
	_bitMasks[2] = -1;
}

char* Image::getData()
{
	return _data;
}

void Image::load(ifstream& file)
{
	if (!file.is_open() || file.eof())
		return;
	
	_type = getFileImageType(file);
	
	file.seekg(0);

	switch (_type)
	{
		case BMP:
		{
			file.seekg(10);//goes the the end of the Header

			unsigned int offset(0); //reads Data Offset
			file.read((char*)&offset, 4);

			file.seekg(18);
			file.read((char*)&_width, 4);
			file.read((char*)&_height, 4);

			file.seekg(28);
			file.read((char*)&_depth, 2);

			file.read((char*)&_compression, 4);


			if (_compression == 3)
			{
				file.read((char*)&_bitMasks[0], 4);
				file.read((char*)&_bitMasks[1], 4);
				file.read((char*)&_bitMasks[2], 4);
			}
			else
				file.seekg(46);

			file.read((char*)&_tableSize, 4);
			if (_tableSize == 1 || _tableSize == 4 || _tableSize == 8)
				_colorTable = new char[_tableSize];
			else
				_colorTable = nullptr;
			
			if (_colorTable) //read color Table
				for (size_t i = 0; i < _tableSize; i++)
					file.read(&_colorTable[i], 1);

			unsigned int dataSize = (unsigned int)ceil((_width * _height * _depth) / 8);
			_data = new char[dataSize];

			_structure = PixelStructure(_depth / 8);

			file.seekg(offset);
			unsigned int last = dataSize % 0xffffffff; 
			unsigned int stepSize = (dataSize - last) / 0xffffffff; //if bigger than 32bit separate
			for (size_t i = 0; i < (dataSize - last); i += stepSize)
					file.read(&_data[i], stepSize);
			file.read(&_data[dataSize - last], last);

			break;
		}
		case PNG:
		{
			file.seekg(8);
			break;
		}
	}
	format();
}

Image::PixelStructure Image::getPixelStructure()
{
	return _structure;
}

void Image::format()
{
	switch (_type)
	{
		case BMP:
		{
			//convert everything to 24 bit or 32 bit
			//flip Bitmap
			//level 1, 2 decompression
			//apply bitfields
			if (_compression == 3 && (_depth & 48))
				throw std::exception("Wrong compression or Depth. It doesn't work together");
			if (_depth == 1)
			{
				char* newData = new char[_width * _height * 3];
				for (unsigned int i = 0; i < _width * _height * 3; i++)
					newData[i] = 0;
				for (unsigned int i = 0; i < (_width * _height) / 8; i++)
				{
					char temp = _data[i];
					for (unsigned int j = 0; j < 8; j++)
					{
						if (temp & (1 << j))
							*((int*)(&newData[(i * 8 + j) * 3])) = 0x00FFFFFF;
					}
				}
				delete[_width * _height] _data;
				_data = newData;
			}
			else if (_depth == 24)
			{
				for (unsigned int i = 0; i < _width * _height; i++) //flip from BGR to RGB
				{
					char temp = _data[(i*3) + 2];
					_data[(i * 3) + 2] = _data[(i * 3)];
					_data[(i * 3)] = temp;
				}
			}
		}
		case PNG:
		{

		}
	}
}

Image::ImageType Image::getFileImageType(ifstream& file)
{
	file.seekg(0);

	char *Sig = new char[8];
	file.read(Sig, 8);
	if (Sig[0] == 'B' && Sig[1] == 'M')
		return BMP;
	else if (*Sig == 0x89504E470D0A1A0A) //magic number check for PNG
		return PNG;
	return BMP;
}

Texture::Texture(string path, unsigned int samplerID) : _image(Image(path)), _samplerID(samplerID), _ID([this](GLuint* tex) {deleteTexture(tex); }, new GLuint), _sampler("_tex" + std::to_string(_samplerID), 1)
{
}

Texture::Texture() : _image(Image()), _samplerID(0), _ID([this](GLuint* tex) {deleteTexture(tex); }, new GLuint), _sampler("_tex" + std::to_string(_samplerID), 1)
{
}

GLenum Texture::getTextureType(Image::PixelStructure id)
{
	switch (id)
	{
	case Image::PixelStructure::A:
		return GL_ALPHA;
	case Image::PixelStructure::RGB:
		return GL_RGB;
	case Image::PixelStructure::RGBA:
		return GL_RGBA;
	default:
		return GL_RGB;
	}
}

void Texture::deleteTexture(GLuint* tex)
{
	if (glIsTexture(*tex))
		glDeleteTextures(1, tex);
}

Texture::~Texture()
{
}

void Texture::load(RessourceHandler * loader)
{
	_imgLink = loader->getRessource<Image>(_image.getPath(), &_image);
}

void Texture::glDownload()
{
	//get image
	if (_image.get() != _imgLink->get())
	{
		_image = *(_imgLink->get());
	}

	_sampler.update((float*)&_samplerID);

	glActiveTexture(GL_TEXTURE0 + _samplerID);
	glGenTextures(1, _ID.get());

	glBindTexture(GL_TEXTURE_2D, *(_ID.get()));
	
	glTexImage2D(GL_TEXTURE_2D, 0, getTextureType(_image.getPixelStructure()), _image.getWidth(), _image.getHeight(), 0, getTextureType(_image.getPixelStructure()), GL_UNSIGNED_BYTE, _image.getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, *(_ID.get()));

	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind()
{
	if (_lastTexID != *_ID.get())
	{
		glBindTexture(GL_TEXTURE_2D, *(_ID.get()));
		_lastTexID = *_ID.get();
	}
}

LayeredTexture::LayeredTexture(string file)
{
	_samplerList.push_back(Texture(file));
}
LayeredTexture::LayeredTexture(vector<string> files)
{
	for (unsigned int i = 0; i < files.size(); i++)
		_samplerList.push_back(Texture(files[i], i));
}

LayeredTexture::~LayeredTexture()
{

}

void LayeredTexture::load(RessourceHandler * loader)
{
	for (Texture& tex : _samplerList)
		tex.load(loader);
}

void LayeredTexture::glDownload()
{
	for (Texture& t : _samplerList)
		t.glDownload();
}

void LayeredTexture::bind()
{
	for (unsigned int i = 0; i < _samplerList.size(); i++)
		_samplerList[i].bind();
}

TextureAtlas::TextureAtlas(string file, unsigned int xCount, unsigned int yCount) : LayeredTexture(file), 
	_xUni("_tex" + std::to_string(_samplerID) + "_xR", 1), _yUni("_tex" + std::to_string(_samplerID) + "_yR", 1)
{
	_countX = xCount;
	_countY = yCount;

	_xRatio = 1.0 / _countX;
	_yRatio = 1.0 / _countY;
}
TextureAtlas::TextureAtlas(vector<string> files, unsigned int xCount, unsigned int yCount) : LayeredTexture(files),
	_xUni("_tex" + std::to_string(_samplerID) + "_xR", 1), _yUni("_tex" + std::to_string(_samplerID) + "_yR", 1)
{
	_countX = xCount;
	_countY = yCount;

	_xRatio = 1.0 / _countX;
	_yRatio = 1.0 / _countY;
}

TextureAtlas::~TextureAtlas(){}
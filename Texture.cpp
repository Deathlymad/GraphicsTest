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

GLuint Texture::_lastTexID = -1;

Image::Image()
{
	_bitMasks[0] = -1;
	_bitMasks[1] = -1;
	_bitMasks[2] = -1;
}
Image::Image(string fileName)
{
	load(fileName);
	_bitMasks[0] = -1;
	_bitMasks[1] = -1;
	_bitMasks[2] = -1;
}

void Image::load(string s)
{
	unsigned int termination = s.find_last_of('.');
	ifstream file(s, ios::binary);

	if (s.compare( termination - 4, 4, ".bmp"))
	{
		load(file);
		if (_type != BMP)
			cout << "File has the wrong Termination." << endl; //add actual type
	}

	file.close();
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

			unsigned int dataSize = ceil((_width * _height * _depth) / 8);
			_data = new char[dataSize];

			file.seekg(offset);
			for (size_t i = 0; i < dataSize; i++)
					file.read(&_data[i], 1);

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

void Image::format()
{
	switch (_type)
	{
		case BMP:
		{
			//convert everything to 24 bit or  32 bit
			//flip Bitmap
			//level 1, 2 decompression
			//apply bitfields
			//removing indexes
			if (_compression == 3 && (_depth & 48))
				throw std::exception("Wrong compression or Depth. It doesn't work together");
			for (unsigned int i = 0; i < _width * _height; i++) //flip from BGR to RGB
			{
				char temp = _data[(i*3) + 2];
				_data[(i * 3) + 2] = _data[(i * 3)];
				_data[(i * 3)] = temp;
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
}



Texture::Texture( Image i) : _image(i), _samplerID(0), _ID([this](GLuint* tex) {deleteTexture(tex); }, new GLuint)
{
}

Texture::Texture(Image i, unsigned int samplerID) : _image(i), _samplerID(0), _ID([this](GLuint* tex) {deleteTexture(tex); }, new GLuint)
{

}

void Texture::deleteTexture(GLuint* tex)
{
	if (glIsTexture(*tex))
		glDeleteTextures(1, tex);
}

Texture::~Texture()
{
}

void Texture::init(Shader* s)
{
	float* f;
	s->addUniform(Shader::Uniform("_tex" + std::to_string(_samplerID), f, 1));
	_sampler.addMemPos(f);
}

void Texture::glDownload()
{
	glActiveTexture(GL_TEXTURE0 + _samplerID);
	glGenTextures(1, _ID.get());

	glBindTexture(GL_TEXTURE_2D, *(_ID.get()));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _image.getWidth(), _image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, _image.getData());

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
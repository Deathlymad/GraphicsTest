#include <glm/gtc/matrix_transform.hpp>

#include "Transformation.h"
#include "Shader.h"


Transformation::Transformation() : EngineObject(), 
	_translation(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f), _ID(-1), 
	TransformationMatrix("TransMatrix")
{}

Transformation::Transformation(unsigned int ID) : EngineObject(), 
	_translation(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f), _ID(ID), 
	TransformationMatrix("TransMatrix" + to_string(ID))
{}

Transformation::Transformation(vec3 translation, vec3 rotation, vec3 scale) : EngineObject(), 
	_translation(translation), _rotation(rotation), _scale(scale), _ID(-1), 
	TransformationMatrix("TransMatrix")
{}

Transformation::Transformation(unsigned int ID, vec3 translation, vec3 rotation, vec3 scale) : EngineObject(), 
	_translation(translation), _rotation(rotation), _scale(scale), _ID(ID), 
	TransformationMatrix("TransMatrix" + to_string(ID))
{}

void Transformation::setTranslation(vec3 trans)
{
	_translation = trans;
}

vec3 Transformation::getTranslation()
{
	return _translation;
}

void Transformation::translate(vec3 trans)
{
	_translation += trans;
}

void Transformation::setRotation(vec3 rot)
{
	_rotation = rot;
}

vec3 Transformation::getRotation()
{
	return _rotation;
}

void Transformation::rotate(vec3 rot)
{
	_rotation += rot;
}

void Transformation::setScale(vec3 scale)
{
	_scale = scale;
}

vec3 Transformation::getScale()
{
	return _scale;
}

void Transformation::scale(vec3 scale)
{
	_scale += scale;
}

mat4 Transformation::getTransMat()
{
	return glm::translate( mat4(), _translation);
}

mat4 Transformation::getRotMat()
{
	glm::mat4 rx = glm::mat4();
	glm::mat4 ry = glm::mat4();
	glm::mat4 rz = glm::mat4();

	float x = (float)(_rotation.x * 3.141 / 180.0);
	float y = (float)(_rotation.y * 3.141 / 180.0);
	float z = (float)(_rotation.z * 3.141 / 180.0);

	rz[0][0] = cosf(z);			rz[0][1] = -sinf(z);		rz[0][2] = 0;			    rz[0][3] = 0;
	rz[1][0] = sinf(z);			rz[1][1] = cosf(z);			rz[1][2] = 0;			    rz[1][3] = 0;
	rz[2][0] = 0;			    rz[2][1] = 0;				 rz[2][2] = 1;			    rz[2][3] = 0;
	rz[3][0] = 0;				rz[3][1] = 0;				 rz[3][2] = 0;			    rz[3][3] = 1;

	rx[0][0] = 1;				rx[0][1] = 0;				 rx[0][2] = 0;			    rx[0][3] = 0;
	rx[1][0] = 0;				rx[1][1] = cosf(x);			rx[1][2] = -sinf(x);		rx[1][3] = 0;
	rx[2][0] = 0;				rx[2][1] = sinf(x);			rx[2][2] = cosf(x);			 rx[2][3] = 0;
	rx[3][0] = 0;				rx[3][1] = 0;			 	 rx[3][2] = 0;			    rx[3][3] = 1;

	ry[0][0] = cosf(y);			ry[0][1] = 0;			    ry[0][2] = -sinf(y);		ry[0][3] = 0;
	ry[1][0] = 0;			    ry[1][1] = 1;			    ry[1][2] = 0;			    ry[1][3] = 0;
	ry[2][0] = sinf(y);			ry[2][1] = 0;			    ry[2][2] = cosf(y);			ry[2][3] = 0;
	ry[3][0] = 0;			    ry[3][1] = 0;			    ry[3][2] = 0;			    ry[3][3] = 1;

	return rz*ry*rx;
}

mat4 Transformation::getScaleMat()
{
	return glm::scale( mat4(), _scale);
}

mat4 Transformation::getMatrix()
{
	mat4 trans = getTransMat();
	mat4 rot   = getRotMat();
	mat4 scale = getScaleMat();
	return trans*rot*scale;
}

void Transformation::init(KeyMap& s)
{
	EngineObject::init(s);
}

void Transformation::render(Shader& s, RenderingEngine::RenderState firstPass)
{
	mat4 mat = getMatrix();
	s.setUniform(TransformationMatrix, &mat[0][0]);

	EngineObject::render(s, firstPass);
}


Transformation::~Transformation()
{
}

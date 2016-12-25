#include <glm\glm.hpp>
#include "Def.h"
#include "EngineObject.h"

NSP_GLM

#pragma once
class Transformation : public EngineObject
{
public:
	Transformation();
	Transformation(unsigned int);
	Transformation(vec3, vec3, vec3);
	Transformation(unsigned int, vec3, vec3, vec3);

	void setTranslation(vec3 trans);
	vec3 getTranslation();
	void translate(vec3 trans);

	void setRotation(vec3 rot);
	vec3 getRotation();
	void rotate(vec3 rot);

	void setScale(vec3 scale);
	vec3 getScale();
	void scale(vec3);


	void init(Screen&);
	void update(ThreadManager& mgr) { EngineObject::update(mgr); }
	void render(Shader&, RenderingEngine::RenderState);


	~Transformation();
private:
	mat4 getTransMat();
	mat4 getRotMat();
	mat4 getScaleMat();
	mat4 getMatrix();

	vec3 _translation;
	vec3 _rotation;
	vec3 _scale;

	const string TransformationMatrix;

	unsigned int _ID;
};


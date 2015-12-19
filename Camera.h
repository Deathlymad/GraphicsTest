#include <glm\glm.hpp>

#pragma once
#include "EngineObject.h"
#include "InputHandler.h"
#include "KeyMap.h"
#include "UniformRegistry.h"
#include "Util.h"

class RenderingEngine;

NSP_UTIL

class Camera :
	public InputHandler,
	public EngineObject
{
public:
	Camera();

	void update();

	void setFoV(float);
	void setAspect(float);

	void registerKeyBinds(KeyMap*);
	void registerUniform(Shader*);
	void onMouseMove(double x, double y);

	~Camera();
protected:
private:
	void move(unsigned short key);

	glm::vec3 _pos, *pos;
	glm::vec3 forward;
	glm::vec3 up;

	glm::mat4 View;
	float FoV;
	float Aspect;
	glm::mat4 projection;
	UniformRegistry<16> ViewProjMat;

	double XAngle, YAngle;

	static float speed;
	static glm::vec3 YAxis;
};


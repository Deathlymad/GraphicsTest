#include <glm\glm.hpp>

#pragma once
#include "EngineObject.h"
#include "InputHandler.h"
#include "KeyMap.h"
#include "UniformRegistry.h"
#include "Util.h"

class RenderingEngine;
class Shader;

NSP_UTIL
NSP_GLM

class Camera :
	public InputHandler,
	public EngineObject
{
public:
	Camera();

	void update(ThreadManager&);
	void render(Shader&, RenderingEngine::RenderState);

	void setFoV(float);
	void setAspect(float);

	vec3 getPos() { return _pos; }

	void registerKeyBinds(KeyMap&);
	void onMouseMove(double x, double y); //move, cameras need to be static too

	~Camera();
protected:
private:
	void move(unsigned short key);

	vec3 _pos;
	vec3 forward;
	vec3 up;
	UniformRegistry EyePos;
	
	mat4 View;
	float FoV;
	float Aspect;
	mat4 projection;
	UniformRegistry ViewProjMat;
	bool _update;

	double XAngle, YAngle;

	static float speed;
	static vec3 YAxis;
};


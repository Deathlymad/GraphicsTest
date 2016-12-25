#include <glm\glm.hpp>

#pragma once
#include "EngineObject.h"
#include "Util.h"
#include "ScreenCallbacks.h"

class RenderingEngine;
class Shader;
class Screen;

NSP_UTIL
NSP_GLM

class Camera :
	public EngineObject,
	public KeyCallback
{
public:
	Camera();

	void update(ThreadManager&);
	void render(Shader&, RenderingEngine::RenderState);

	void setFoV(float);
	void setAspect(float);

	vec3 getPos() { return _pos; }

	void onMouseMove(double x, double y); //move, cameras need to be static too

	~Camera();
protected:
private:
	void onCallback(char button, char action, char mods);

	vec3 _pos;
	vec3 forward;
	vec3 up;
	const string EyePos;
	
	mat4 View;
	float FoV;
	float Aspect;
	mat4 projection;
	const string ViewProjMat;
	bool _update;

	double XAngle, YAngle;

	static float speed;
	static vec3 YAxis;
};


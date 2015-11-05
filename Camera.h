#include <glm\glm.hpp>

#pragma once
#include "EngineObject.h"
#include "InputHandler.h"
#include "KeyMap.h"

class Shader;

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
	void registerUniforms(Shader*);

	~Camera();
protected:
	void onMouseMove(double x, double y); //possibly should be moved
private:
	void move(unsigned short key);

	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;

	glm::mat4 View;
	float FoV;
	float Aspect;
	glm::mat4 projection;
	float ViewProjMatPtr[16];

	double XAngle, YAngle;

	static float speed;
	static glm::vec3 YAxis;
};


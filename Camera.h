#include <glm\glm.hpp>

#pragma once
#include "EngineObject.h"
#include "InputHandler.h"

class Shader;

class Camera :
	public InputHandler,
	public EngineObject
{
public:
	Camera();

	void update();

	void registerUniforms(Shader*);

	~Camera();
protected:
	void onKeyPress(char button, char action, char mods);
	void onMouseMove(double x, double y);
private:
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;

	glm::mat4 View;
	glm::mat4 projection;

	double XAngle, YAngle;

	static float speed;
	static glm::vec3 YAxis;
};


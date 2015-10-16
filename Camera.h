#include <glm\glm.hpp>

#pragma once
#include "InputHandler.h"
class Camera :
	public InputHandler
{
public:
	Camera();
	~Camera();
protected:
	void onKeyPress(char button, char action, char mods);
	void onMouseMove(double x, double y);
private:
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::mat4 projection;

	static float speed;
};


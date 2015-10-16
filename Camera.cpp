#include<GLFW\glfw3.h> //needed for defines
#include "Camera.h"

float Camera::speed = 0.005;

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::onKeyPress(char button, char action, char mods)
{
	if (action = GLFW_PRESS && mods == 0)
	{
		switch (button)
		{
		case GLFW_KEY_W:
			pos += forward * speed;
		case GLFW_KEY_A:
			pos += glm::cross(up, forward) * speed;
		case GLFW_KEY_S:
			pos += -forward * speed;
		case GLFW_KEY_D:
			pos += glm::cross( forward, up) * speed;
		default:
			break;
		}
	}
}

void Camera::onMouseMove(double x, double y)
{
}

#include<GLFW\glfw3.h> //needed for defines
#include "Camera.h"
#include "Shader.h"

float Camera::speed = 0.005f;
glm::vec3 Camera::YAxis = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera()
{
}

void Camera::registerUniforms(Shader * s)
{
	s->addUniform(Shader::Uniform("proj", &projection[0][0]));
	Shader::Uniform("View", &projection[0][0]); //compute View Matrix
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

void Camera::onMouseMove(double dX, double dY)
{
	//computing X rotation
	XAngle += dX;

	glm::vec3 HorizontalAxis = glm::normalize(glm::cross(YAxis, forward));

	forward = glm::vec3(
		cos(XAngle) * sin(YAngle),
		sin(XAngle),
		cos(XAngle) * cos(YAngle));

	up = glm::normalize(glm::cross(HorizontalAxis, forward));

	//computing Y rotation
	YAngle += dY;

	HorizontalAxis = glm::normalize(glm::cross(YAxis, forward));

	forward = glm::vec3(
		cos(XAngle) * sin(YAngle),
		sin(XAngle),
		cos(XAngle) * cos(YAngle));


	up = glm::normalize(glm::cross(forward, HorizontalAxis));
	
}

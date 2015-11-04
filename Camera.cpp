#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Shader.h"
#include<GLFW\glfw3.h> //needed for defines

float Camera::speed = 0.05f;
glm::vec3 Camera::YAxis = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() : InputHandler(), EngineObject()
{
	pos = glm::vec3(0, 0, -5);
	forward = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	projection = glm::perspective(45.0f, (float) 4/3, 0.1f, 100.0f); 
}

void Camera::update()
{
	View = projection * glm::lookAt(pos, pos + glm::normalize(forward), glm::normalize(up));
	if (ViewProjMatPtr)
	{
		for (unsigned char i = 0; i < 16; i++) //writes memory
		{
			ViewProjMatPtr[i] = View[floor(i/4)][i - (floor(i / 4) * 4)];
		}
	}
}

void Camera::registerKeyBinds(KeyMap * k)
{
	if (!this)
		return;
	k->addKeyBind(87, [this](unsigned short key) { move(key); }, "Move Forward");//W
	k->addKeyBind(83, [this](unsigned short key) { move(key); }, "Move Backward");//S
	k->addKeyBind(65, [this](unsigned short key) { move(key); }, "Strafe Left");//A
	k->addKeyBind(68, [this](unsigned short key) { move(key); }, "Strafe Right");//D
}

void Camera::registerUniforms(Shader * s)
{
	//s->addUniform(Shader::Uniform("proj", &projection[0][0], 16));
	s->addUniform(Shader::Uniform("View", ViewProjMatPtr, 16)); //leads to change in Position need to find out why
}


Camera::~Camera()
{
	delete[16](&ViewProjMatPtr[0]);
}

void Camera::onMouseMove(double dY, double dX)
{
	//computing X rotation
	XAngle += dX * -0.0005;
	YAngle += dY * -0.0005;

	glm::vec3 HorizontalAxis = glm::normalize(glm::cross(YAxis, forward));

	//rotate forward
	forward = glm::vec3(
		cos(XAngle) * sin(YAngle),
		sin(XAngle),
		cos(XAngle) * cos(YAngle));
	
	up = glm::normalize(glm::cross(forward, HorizontalAxis));
}

void Camera::move(unsigned short key)
{
	switch (key)
	{
	case 65:
		pos += glm::normalize(glm::cross(up, forward)) * 0.05f;
		break;
	case 68:
		pos += glm::normalize(glm::cross(forward, up)) * 0.05f;
		break;
	case 83:
		pos -= 0.05f * forward; //not working
		break;
	case 87:
		pos += 0.05f * forward; //not working
		break;
	}
}

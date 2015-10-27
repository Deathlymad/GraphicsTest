#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Shader.h"
#include<GLFW\glfw3.h> //needed for defines

float Camera::speed = 0.05f;
glm::vec3 Camera::YAxis = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() : InputHandler(), EngineObject()
{
	pos = glm::vec3(0, 0, 0);
	forward = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
}

void Camera::update()
{
	//View = glm::mat4(1.0f);
	View = glm::lookAt( pos, pos + forward, up);
	if (ViewMatPtr)
	{
		for (unsigned char i = 0; i < 16; i++) //writes memory
		{
			ViewMatPtr[i] = View[floor(i/4)][i - (floor(i / 4) * 4)];
		}
	}
}

void Camera::registerKeyBinds(KeyMap * k) //crashing Vectos can't be resolved, maybe they didn't like copying, also this == 0x00000000
{
	if (!this)
		return;
	k->addKeyBind(87, [this](unsigned short) {pos += speed * forward; });//W
	k->addKeyBind(83, [this](unsigned short) {pos -= speed * forward; });//S
	k->addKeyBind(65, [this](unsigned short) {pos += glm::cross(up, forward) * speed; });//A
	k->addKeyBind(68, [this](unsigned short) {pos += glm::cross(forward, up) * speed; });//D
}

void Camera::registerUniforms(Shader * s)
{
	//s->addUniform(Shader::Uniform("proj", &projection[0][0], 16));
	s->addUniform(Shader::Uniform("View", ViewMatPtr, 16)); //leads to change in Position need to find out why
}


Camera::~Camera()
{
	delete[16](&ViewMatPtr[0]);
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
	
	//keeping angles low
	if (XAngle >= 360)
		XAngle -= 360;
	if (YAngle >= 360)
		YAngle -= 360;
}

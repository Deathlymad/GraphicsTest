#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Shader.h"
#include "RenderingEngine.h"
#include<GLFW\glfw3.h> //needed for defines

float Camera::speed = 0.05f;
glm::vec3 Camera::YAxis = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() : InputHandler(), EngineObject(), ViewProjMatPtr(new float[16])
{
	pos = glm::vec3(0, 0, -5);
	forward = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	FoV = 45.0f;
	Aspect = 4 / 3;
	View = glm::mat4();
	projection = glm::perspective( FoV, Aspect, 0.1f, 100.0f);
}

void Camera::update()
{
	View = projection * glm::lookAt(pos, pos + glm::normalize(forward), glm::normalize(up));
	if (ViewProjMatPtr.get() != nullptr)
	{
		for (unsigned char i = 0; i < 16; i++) //writes memory
		{
			ViewProjMatPtr.set(View[floorf(i / 4)][i - (floorf(i / 4) * 4)], i);
		}
	}
}

void Camera::setFoV(float fov)
{
	FoV = fov;
	projection = glm::perspective(FoV, Aspect, 0.1f, 100.0f);
}

void Camera::setAspect(float aspect)
{
	Aspect = aspect;
	projection = glm::perspective(FoV, Aspect, 0.1f, 100.0f);
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

void Camera::registerUniform(Shader * s)
{
	s->addUniform(Shader::Uniform("ViewProj", ViewProjMatPtr.get(), 16));
	s->addUniform(Shader::Uniform("EyePos", &pos[0], 3));
}

Camera::~Camera()
{
}

void Camera::onMouseMove(double dY, double dX)
{
	//computing X rotation
	XAngle += dX * -(speed / 100);
	YAngle += dY * -(speed / 100);

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
		pos += glm::normalize(glm::cross(up, forward)) * speed;
		break;
	case 68:
		pos += glm::normalize(glm::cross(forward, up)) * speed;
		break;
	case 83:
		pos -= speed * forward; //not working
		break;
	case 87:
		pos += speed * forward; //not working
		break;
	}
}

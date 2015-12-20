#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Shader.h"
#include "RenderingEngine.h"
#include<GLFW\glfw3.h> //needed for defines

float Camera::speed = 0.05f;
vec3 Camera::YAxis = vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() : InputHandler(), EngineObject()
{
	_pos = vec3(0, 0, -5);
	forward = vec3(0, 0, -1);
	up = vec3(0, 1, 0);
	FoV = 45.0f;
	Aspect = 4 / 3;
	View = mat4();
	projection = perspective( FoV, Aspect, 0.1f, 100.0f);
}

void Camera::update()
{
	View = projection * lookAt(_pos, _pos + normalize(forward), normalize(up));
	ViewProjMat.update(&View[0][0]);
}

void Camera::setFoV(float fov)
{
	FoV = fov;
	projection = perspective(FoV, Aspect, 0.1f, 100.0f);
}

void Camera::setAspect(float aspect)
{
	Aspect = aspect;
	projection = perspective(FoV, Aspect, 0.1f, 100.0f);
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
	float* temp = nullptr;
	s->addUniform(Shader::Uniform("ViewProj", temp, 16));
	ViewProjMat.addMemPos(temp);
	temp = nullptr;
	s->addUniform(Shader::Uniform("EyePos", temp, 3));
	pos = (vec3*)temp;
}

Camera::~Camera()
{
}

void Camera::onMouseMove(double dY, double dX)
{
	//computing X rotation
	XAngle += dX * - (speed / 100);
	YAngle += dY * - (speed / 100);

	vec3 HorizontalAxis = normalize(cross(YAxis, forward));

	//rotate forward
	forward = vec3(
		cos(XAngle) * sin(YAngle),
		sin(XAngle),
		cos(XAngle) * cos(YAngle));
	
	up = normalize(cross(forward, HorizontalAxis));
}

void Camera::move(unsigned short key)
{
	switch (key)
	{
	case 65:
		_pos += normalize(cross(up, forward)) * speed;
		break;
	case 68:
		_pos += normalize(cross(forward, up)) * speed;
		break;
	case 83:
		_pos -= speed * forward; //not working
		break;
	case 87:
		_pos += speed * forward; //not working
		break;
	}
}

#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Shader.h"
#include "RenderingEngine.h"
#include<GLFW\glfw3.h> //needed for defines

float Camera::speed = 0.0005f;
vec3 Camera::YAxis = vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() : InputHandler(), EngineObject(), ViewProjMat("ViewProj", 16), EyePos("EyePos", 3)
{
	_pos = vec3(0, 0, -5);
	forward = vec3(0, 0, -1);
	up = vec3(0, 1, 0);
	FoV = 45.0f;
	Aspect = 1.333f;
	View = mat4();
	projection = perspective( FoV, Aspect, 0.1f, 100.0f);
}

void Camera::update()
{
	View = projection * lookAt(_pos, _pos + normalize(forward), normalize(up));
}

void Camera::render(Shader *, bool firstPass)
{
	if (firstPass)
	{
		ViewProjMat.update(&View[0][0]);
		EyePos.update(&_pos[0]);
	}
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
	k->addKeyBind(87, [this](unsigned short key, KeyMap::KeyState) { move(key); }, "Move Forward", KeyMap::KeyState::ONHOLD | KeyMap::KeyState::ONPRESS);//W
	k->addKeyBind(83, [this](unsigned short key, KeyMap::KeyState) { move(key); }, "Move Backward", KeyMap::KeyState::ONHOLD | KeyMap::KeyState::ONPRESS);//S
	k->addKeyBind(65, [this](unsigned short key, KeyMap::KeyState) { move(key); }, "Strafe Left", KeyMap::KeyState::ONHOLD | KeyMap::KeyState::ONPRESS);//A
	k->addKeyBind(68, [this](unsigned short key, KeyMap::KeyState) { move(key); }, "Strafe Right", KeyMap::KeyState::ONHOLD | KeyMap::KeyState::ONPRESS);//D
}

Camera::~Camera()
{
}

void Camera::onMouseMove(double dY, double dX)
{
	//computing X rotation
	XAngle += dX * - speed;
	YAngle += dY * - speed;

	vec3 HorizontalAxis = normalize(cross(YAxis, forward));

	//rotate forward
	forward = normalize(vec3(
		cos(XAngle) * sin(YAngle),
		sin(XAngle),
		cos(XAngle) * cos(YAngle)));
	
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

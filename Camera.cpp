#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Shader.h"
#include "RenderingEngine.h"

float Camera::speed = 0.05f;
vec3 Camera::YAxis = vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() : KeyCallback(), EngineObject(), ViewProjMat("ViewProj"), EyePos("EyePos")
{
	_pos = vec3(0, 0, 0);
	forward = vec3(0, 0, -1);
	up = vec3(0, 1, 0);
	FoV = 90.0f;
	Aspect = 1.333f;
	View = mat4();
	projection = perspective( FoV, Aspect, 0.1f, 100000.0f);
}

void Camera::update(ThreadManager& mgr)
{
}

void Camera::render(Shader& s, RenderingEngine::RenderState firstPass)
{
	View = projection * lookAt(_pos, _pos + normalize(forward), normalize(up));
	s.setUniform(ViewProjMat, &View[0][0]);
	s.setUniform(EyePos, &_pos[0]);
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

Camera::~Camera()
{
}

void Camera::onMouseMove(double dY, double dX)
{
	//computing X rotation
	XAngle += dX * - speed * 0.01;
	YAngle += dY * - speed * 0.01;

	vec3 HorizontalAxis = normalize(cross(YAxis, forward));

	//rotate forward
	forward = normalize(vec3(
		cos(XAngle) * sin(YAngle),
		sin(XAngle),
		cos(XAngle) * cos(YAngle)));
	
	up = normalize(cross(forward, HorizontalAxis));
}

void Camera::onCallback(char button, char action, char mods)
{
	switch (button)
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

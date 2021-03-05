#include "stdafx.h"
#include "RayCaster.h"

std::shared_ptr<Camera> RayCaster::m_mainCam{};
glm::vec2 RayCaster::resolution{};

void RayCaster::Init(int width, int height)
{
	using namespace SignalSystem;
	InputSignals::Get()->onMouseLeftClick.connect(LeftMouseClick);
	ApplicationEvents::Get()->onScreenResize.connect(ScreenResize);
	m_mainCam = DataHolder::Get()->GetCameraShared();
	resolution = glm::vec2{width, height};
}

glm::vec3 RayCaster::GetMouseWorldPos(const glm::vec2& mousePos)
{
	const float x = (2.0f*((mousePos.x)/(resolution.x)))-1.0f;
	const float y = 1.0f-(2.0f*((mousePos.y)/(resolution.y)));
	const glm::vec4 deviceCoords =
	{
		x,
		y,
		-0.99f,
		1
	};
	const RayWithMouse ray{CreateRay(deviceCoords)};
	return ray.m_origin + ray.m_direction * 2.0f;
}

void RayCaster::LeftMouseClick(const glm::vec2& mousePos)
{
	using namespace SignalSystem;
	
	const float x = (2.0f*((mousePos.x)/(resolution.x)))-1.0f;
	const float y = 1.0f-(2.0f*((mousePos.y)/(resolution.y)));
	const glm::vec4 deviceCoords =
	{
		x,
		y,
		0,
		1
	};
	RayWithMouse ray(CreateRay(deviceCoords));
	ray.m_screenPos = mousePos;
	ApplicationEvents::Get()->onRayCast(ray);
}

RayWithMouse RayCaster::CreateRay(const glm::vec4& deviceOrigin)
{
	using namespace glm;
	auto viewProj = m_mainCam->GetViewProjectionMatrix();
	viewProj = inverse(viewProj);
	glm::vec4 tmp = viewProj * deviceOrigin;
	glm::vec3 worldCoordsOrigin = tmp / tmp.w;
	
	const auto dir2 = m_mainCam->GetPosition() - worldCoordsOrigin;

	
	return RayWithMouse{m_mainCam->GetPosition(), RUBIX_CUBE_RAY_DISTANCE, normalize(-dir2), worldCoordsOrigin};
}

void RayCaster::ScreenResize(const glm::vec2& res)
{
	resolution = res;
}

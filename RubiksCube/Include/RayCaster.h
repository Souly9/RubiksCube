#pragma once
#include "InputSignals.h"

class RayCaster
{
public:
	static void Init(int width, int height);
	static glm::vec3 GetMouseWorldPos(const glm::vec2& mousePos);
	static void LeftMouseClick(const glm::vec2& mousePos);
	// Callback to keep up to date with the screen size
	static void ScreenResize(const glm::vec2& res);
	static RayWithMouse CreateRay(const glm::vec4& deviceOrigin);
	static glm::vec2 resolution;
private:
	
	static std::shared_ptr<Camera> m_mainCam;
	
};



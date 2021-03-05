#pragma once

struct RayWithMouse
{
	glm::vec3 m_origin;
	float m_distance;
	glm::vec3 m_direction;
	glm::vec2 m_screenPos;
};

struct RaySlim
{
	glm::vec3 m_origin;
	float m_distance;
	glm::vec3 m_direction;
};

struct Plane
{
	glm::vec3 m_center;
	glm::vec3 m_normal;
	float m_span;
};


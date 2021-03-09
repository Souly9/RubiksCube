#include "stdafx.h"
#include "Camera.h"


Camera::Camera(float fov, float zNear, float zFar, float aspectRatio,
               glm::vec4 pos,
               glm::quat rot)
	: m_FoV(fov),
	  m_zNear(zNear),
	  m_zFar(zFar),
	  m_AspectRatio(aspectRatio)
{
	m_rotatorState = ShouldIgnore;
	SignalSystem::InputSignals::Get()->onMouseScroll.connect([this](const auto& val){Scroll(val);});
	SignalSystem::ApplicationEvents::Get()->onScreenResize.connect([this](const glm::vec2& size)
	{
		m_AspectRatio = size.x / size.y;
		m_isDirty = true;
	});
	const auto p = static_cast<AlignedCameraData*>(_aligned_malloc(sizeof(AlignedCameraData), 16));
	m_pData = std::unique_ptr<AlignedCameraData>(p);
	m_pData->m_target = glm::vec4(0,0,0,1);
	m_pData->m_upDirection = glm::vec4(0,1,0,0);
	m_pData->m_position = pos;
	m_pData->m_rotationQuat = rot;
}

Camera::~Camera()
{
	// Take out the object
	auto p = m_pData.get();
	m_pData.release();
	// Delete it manually
	_aligned_free(p);
}

void Camera::UpdateMatrices()
{
	if (m_isDirty)
	{
		m_pData->m_projMatrix = glm::perspective(glm::radians(m_FoV), m_AspectRatio, m_zNear, m_zFar);
		LookAt( m_pData->m_position, m_pData->m_target, m_pData->m_upDirection);
		m_pData->m_viewProjMatrix = m_pData->m_projMatrix * m_pData->m_viewMatrix;
		m_isDirty = false;
	}
}

void Camera::LookAt(const glm::vec3 pos, const glm::vec3 target, const glm::vec3 upDirection)
{
	using namespace glm;
	m_pData->m_position = glm::vec4(pos.x, pos.y, pos.z, 1);
	m_pData->m_target = glm::vec4(target.x, target.y, target.z, 1);
	m_pData->m_upDirection = glm::vec4(upDirection.x, upDirection.y, upDirection.z, 1);
	const vec3 zAxis = normalize(pos - target);
	const vec3 xAxis = normalize(
		cross(normalize(upDirection), zAxis));
	const vec3 yAxis = cross(zAxis, xAxis);
	
	
	m_pData->m_viewMatrix = mat4{
		xAxis.x, yAxis.x, zAxis.x, 0,
		xAxis.y, yAxis.y, zAxis.y, 0,
		xAxis.z, yAxis.z, zAxis.z, 0,
		-dot(xAxis, pos), -dot(yAxis, pos), -dot(zAxis, pos), 1
	};
	m_isDirty = true;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return m_pData->m_viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_pData->m_projMatrix;
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	return m_pData->m_viewProjMatrix;
}

void Camera::set_FoV(const float fov)
{
	m_FoV = fov;
	m_isDirty = true;
}

void Camera::SetPositions(const glm::vec4 position)
{
	m_pData->m_position = position;
}

glm::vec3 Camera::GetPosition() const
{
	return m_pData->m_position;
}
void Camera::SetRotation(const glm::quat& rotation)
{
	m_pData->m_rotationQuat = rotation;
	m_isDirty = true;
}

glm::quat Camera::GetRotation() const
{
	return m_pData->m_rotationQuat;
}

void Camera::Translate(const glm::vec4 trans)
{
	m_pData->m_position += trans;
	m_isDirty = true;
}

void Camera::Scroll(const float val)
{
	const glm::mat4& view = m_pData->m_viewMatrix;
	const glm::vec4 dir
	{
		view[0][2],
		view[1][2],
		view[2][2],
		1
	};
	const auto translation = dir * (val * 0.1f);
	Translate(translation);
}

using namespace SignalSystem;


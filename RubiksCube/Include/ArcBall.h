#ifndef ARCBALL_H
#define ARCBALL_H
#include <glm/vec2.hpp>
#include <glm/gtx/quaternion.hpp>
#include "ApplicationEvents.h"


/**
 * Header only utility class to implement rotation through ArcBall
 */
class ArcBall
{
	static float m_radius;

	static bool m_isDragging;
	static bool m_isDirty;

	static glm::vec2 m_center;
	static glm::vec2 m_mousePos;
	static glm::vec2 m_mouseDown;

	static glm::vec3 m_arcPosFrom;
	static glm::vec3 m_arcPosTo;

	static glm::quat m_rotQuat;
	static glm::quat m_rotQuatDown;
	static glm::quat m_currentQuat;

	static glm::mat4 m_rotationMatrix;

public:

	static void UpdatePos(const glm::vec2& center, const float radius) noexcept
	{
		m_center = center;
		m_radius = radius;
		m_mouseDown = m_center;
		m_mousePos = m_center;
	}

	static void SetMousePos(const glm::vec2& pos)
	{
		m_mousePos = pos;
		m_isDirty = true;
		glm::mat4 mat;
		GetRotationMatrix(mat);
		SignalSystem::ApplicationEvents::Get()->onArcBallUpdate(m_currentQuat);
	}

	static void StartDragging()
	{
		m_isDragging = true;
		m_mouseDown = m_mousePos;
		m_isDirty = true;
		glm::mat4 mat;
		GetRotationMatrix(mat);
		SignalSystem::ApplicationEvents::Get()->onArcBallUpdate(m_currentQuat);
	}

	static void StopDragging()
	{
		m_isDragging = false;
		m_rotQuatDown = m_currentQuat;
		m_isDirty = true;
		glm::mat4 mat;
		GetRotationMatrix(mat);
		SignalSystem::ApplicationEvents::Get()->onArcBallUpdate(m_currentQuat);
	}

	static void GetRotationMatrix(glm::mat4& rslt)
	{
		if (m_isDirty)
		{
			m_arcPosFrom = ConvertToSphere(m_mouseDown);
			m_arcPosTo = ConvertToSphere(m_mousePos);
			if (m_isDragging)
			{
				m_rotQuat = FromToQuat(m_arcPosFrom, m_arcPosTo);
				m_currentQuat = normalize(m_rotQuat * m_rotQuatDown);
			}
			m_rotationMatrix = transpose(toMat4(m_currentQuat));
			m_isDirty = false;
		}
		rslt = m_rotationMatrix;
	}

	static glm::quat GetRot() { return m_currentQuat; }

	static void HookEvents()
	{
		using namespace SignalSystem;
		InputSignals::Get()->onMouseRightClick.connect(SetMousePos);
		InputSignals::Get()->onMouseDrag.connect(SetMousePos);
		InputSignals::Get()->onMouseStartRightDrag.connect(StartDragging);
		InputSignals::Get()->onMouseStopRightDrag.connect(StopDragging);
	}

private:


	static glm::vec3 ConvertToSphere(const glm::vec2& mousePos)
	{
		glm::vec3 scaledDownMouse(
			// Reverse x-rotation as it's inversed
			(m_center.x - mousePos.x) / m_radius,
			(mousePos.y - m_center.y) / m_radius,
			0
		);

		float dist = dot(scaledDownMouse, scaledDownMouse);

		if (dist > 1.0f)
		{
			// Distance bigger than radius
			dist = sqrt(dist);
			scaledDownMouse /= dist;
			scaledDownMouse[2] = 0;
		}
		else
		{
			scaledDownMouse[2] = sqrt(1.0 - dist);
		}

		return scaledDownMouse;
	}

	// Half-way Quaternion based procedure to find the rotation between two vectors
	// Simplified the equations to get rid of the square roots and alike
	static glm::quat FromToQuat(const glm::vec3& from, const glm::vec3& to)
	{
		return glm::quat(
			from[0] * to[0] + from[1] * to[1] + from[2] * to[2],
			from[1] * to[2] - from[2] * to[1],
			from[2] * to[0] - from[0] * to[2],
			from[0] * to[1] - from[1] * to[0]
		);
	}
};


#endif

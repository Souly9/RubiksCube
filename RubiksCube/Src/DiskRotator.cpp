#include "stdafx.h"
#include "DiskRotator.h"

#include "RayCaster.h"

using namespace RubixCubeEnums;

DiskRotator::DiskRotator(const float& anglePerPixel) : m_pCurrentDisk(std::make_shared<Object>()), m_curAxisType(),
                                                m_initialParams(), m_anglePerPixel(anglePerPixel)
{
}

void DiskRotator::StartRotation(const glm::vec2& mousePos, RubixCubeStructs::RotatorInitParams&& params, Object* parent)
{
	m_state = AnimatorState::Animating;
	m_initialParams = params;
	m_lastMousePos = mousePos;
	m_initialParams.m_inverseMatrix = m_initialParams.m_inverseMatrix;
	m_lastCubeMousePos = m_initialParams.m_inverseMatrix * glm::vec4(RayCaster::GetMouseWorldPos(mousePos), 1);
	m_accAngle = 0;
	m_pCurrentDisk->ResetMatrix();
	m_pCurrentDisk->SetName("Disk");
	m_pCurrentDisk->SetParent(parent);
	m_isAxisSelected = false;
	m_wasAxisSelectedLastFrame = false;
}

void DiskRotator::ContinueRotation(const glm::vec2& mousePos)
{
	using namespace glm;
	if (m_state == AnimatorState::Animating)
	{
		const auto diff = m_lastMousePos - mousePos;
		m_lastVel = diff;


		const vec3 po = m_initialParams.m_inverseMatrix * vec4(RayCaster::GetMouseWorldPos(mousePos), 1);
		const auto cubeSpaceVel = -normalize(m_lastCubeMousePos - po);

		if (!m_isAxisSelected && (length2(diff) > 6))
		{
			const auto allNormalizedAxes = vec3{1, 1, 1};

			m_curRotationAxis = abs(dot(cubeSpaceVel, m_initialParams.m_yAxis)) < 0.5f
				                    ? m_initialParams.m_yAxis
				                    : m_initialParams.m_xAxis;
			const auto logicalAxis = allNormalizedAxes - m_curRotationAxis;

			if (logicalAxis.x < logicalAxis.y && logicalAxis.x < logicalAxis.z)
			{
				m_curAxisType = yAxis;
			}
			else if (logicalAxis.y < logicalAxis.x && logicalAxis.y < logicalAxis.z)
			{
				m_curAxisType = xAxis;
			}
			else
			{
				m_curAxisType = zAxis;
			}
			m_isAxisSelected = true;
			m_wasAxisSelectedLastFrame = false;
		}
		else if (m_isAxisSelected)
		{
			// Turn the angle when we moved in the other direction
			int orientation = abs(diff.x) > abs(diff.y) ? diff.x * m_initialParams.m_xInverted: diff.y* m_initialParams.m_yInverted;
			orientation = -((0.0f < orientation) - (orientation < 0.0f));
			if (orientation > COMBUSTION_FLOAT_TOLERANCE) orientation = 1;

			const auto angle = orientation * length(diff) * m_anglePerPixel;
			
			m_wasAxisSelectedLastFrame = true;
			m_lastCubeMousePos = po;
			m_lastMousePos = mousePos;

			m_accAngle += angle;
			m_lastRotation = angleAxis(radians(angle), normalize(m_curRotationAxis));
			m_pCurrentDisk->AdditiveRotation(normalize(m_lastRotation));
		}
	}
}

void DiskRotator::EndRotation(const glm::vec2& mousePos, RotationType& type, RotationAxis& logicalRotationAxis,
                              int& numberOfTurns)
{
	if (m_state == AnimatorState::Animating)
	{
		ContinueRotation(mousePos);
		type = m_accAngle < 0 ? CounterClockwise : Clockwise;
		logicalRotationAxis = m_curAxisType;
		numberOfTurns = 0;
		while (abs(m_accAngle) > 90.1f)
		{
			++numberOfTurns;
			m_accAngle -= 90.0f * (type == CounterClockwise ? -1 : 1);
		}

		numberOfTurns += (abs(m_accAngle) > 40);
			
		m_lastRotation = angleAxis(
			glm::radians((numberOfTurns * 90.0f) * (type == CounterClockwise ? -1 : 1)),
			m_curRotationAxis);
		m_pCurrentDisk->Rotate(normalize(m_lastRotation));
		m_state = AnimatorState::Idle;
	}
}

std::shared_ptr<Object> DiskRotator::GetDisk() const
{
	return m_pCurrentDisk;
}

glm::vec3 DiskRotator::GetRotAxis() const
{
	return m_curRotationAxis;
}

RotationAxis DiskRotator::GetRotType() const
{
	return m_curAxisType;
}


bool DiskRotator::IsFirstRot() const
{
	return m_isAxisSelected && !m_wasAxisSelectedLastFrame;
}

#include "stdafx.h"
#include "ArcBall.h"

float ArcBall::m_radius;

bool ArcBall::m_isDragging;
bool ArcBall::m_isDirty{true};

glm::vec2 ArcBall::m_center;
glm::vec2 ArcBall::m_mousePos;
glm::vec2 ArcBall::m_mouseDown;

glm::vec3 ArcBall::m_arcPosFrom;
glm::vec3 ArcBall::m_arcPosTo;

glm::quat ArcBall::m_rotQuat{glm::quat(1, 0, 0, 0)};
glm::quat ArcBall::m_rotQuatDown{glm::quat(1, 0, 0, 0)};
glm::quat ArcBall::m_currentQuat{glm::quat(1, 0, 0, 0)};

glm::mat4 ArcBall::m_rotationMatrix;

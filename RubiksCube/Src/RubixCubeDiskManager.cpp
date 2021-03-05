#include "stdafx.h"
#include "RubixCube.h"
#include "RubixCubeDiskManager.h"

int RubixCubeDiskManager::RayCastCube(const RaySlim& ray, CVec<int>& cubieIndex, const RubixCube* rubiksCube) const
{
	using namespace RubixCubeEnums;
	using namespace glm;
	for (int i = 0; i < 6; ++i)
	{
		// Since we only check the center rubiksCube for now, the vector will always contain only one elemant
		Color color;
		switch(i)
		{
		case 0:
			color = Color::White;
			break;
			case 1:
			color = Color::Yellow;
			break;
			case 2:
			color = Color::Red;
			break;
			case 3:
			color = Color::Blue;
			break;
			case 4:
			color = Color::Orange;
			break;
			case 5:
			color = Color::Green;
			break;
		}
		const CVec<int> center = rubiksCube->m_centerPieces[i];
		const vec3 planeNormal = vec4
		{
			rubiksCube->m_cubies.at(rubiksCube->m_logicalCube[center.x][center.y][center.z])->GetData()->m_normals.at(
				static_cast<int>(color) * 4),
			1
		};
		const vec3 planeCenter = vec3{
			rubiksCube->m_center
		} + planeNormal * (rubiksCube->m_sideLengthCubie * 1.5f);
		const Plane plane{planeCenter, planeNormal, rubiksCube->m_sideLengthCubieFace};

		float t;
		if (IsRayInFace(ray, plane, t))
		{
			const vec3 intersection = ray.m_origin + (ray.m_direction * t);
			const auto len = rubiksCube->m_sideLengthCubieFace * 0.5f;
			vec3 localYAxis = vec3(0, 1, 0);
			vec3 localXAxis = normalize(glm::cross(planeNormal, localYAxis));
			if (planeNormal.y < -0.8f)
			{
				localYAxis = vec3(0, 0, 1);
				localXAxis = normalize(glm::cross(planeNormal, -localYAxis));
			}
			if (planeNormal.y > 0.8f)
			{
				localYAxis = vec3(0, 0, -1);
				localXAxis = normalize(glm::cross(planeNormal, -localYAxis));
			}
			if (planeNormal.x > 0 || planeNormal.y > 0 || planeNormal.z < 0)
			{
				localXAxis = normalize(glm::cross(planeNormal, -localYAxis));
			}


			const vec3 downLeft = planeCenter - (localXAxis * len + localYAxis * len);
			const vec3 topLeft = planeCenter + (localXAxis * -len + localYAxis * len);
			const vec3 topRight = planeCenter + (localXAxis * len - localYAxis * len);

			if (IsRayInSquare(topLeft, downLeft, topRight, intersection))
			{
				const auto cubieLen = rubiksCube->m_sideLengthCubie * 0.5f;
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						const CVec<int> in = rubiksCube->GetNextLogicalPos(color, rubiksCube->m_centerPieces[i], x, y);

						const Cubie& cubie = *rubiksCube->m_cubies.at(rubiksCube->m_logicalCube[in.x][in.y][in.z]);
						const vec3 cubieNormal =
							cubie.GetData()->m_normals.at(static_cast<int>(color) * 4);
						const vec3 cubieCenter = vec3{
							cubie.GetCenter().x, cubie.GetCenter().y, cubie.GetCenter().z
						} + cubieNormal * (cubieLen);

						const vec3 cubieDownLeft = cubieCenter - (localXAxis * cubieLen + localYAxis * cubieLen);
						const vec3 cubieTopLeft = cubieCenter + (localXAxis * -cubieLen + localYAxis * cubieLen);
						const vec3 cubieTopRight = cubieCenter + (localXAxis * cubieLen + localYAxis * cubieLen);

						if (IsRayInSquare(cubieTopLeft, cubieDownLeft, cubieTopRight, intersection))
						{
							cubieIndex = CVec<int>{in.x, in.y, in.z};
							return i;
						}
					}
				}
			}
		}
	}
	return 99;
}

bool RubixCubeDiskManager::IsRayInSquare(const glm::vec3& topLeft, const glm::vec3& downLeft, const glm::vec3& topRight,
                              const glm::vec3& intersection) const
{
	const auto leftCIntersect = intersection - topLeft;
	const auto leftRight = (topRight - topLeft);
	const auto downBottom = (downLeft - topLeft);
	const float u = dot(leftCIntersect, leftRight);
	const float v = dot(leftCIntersect, downBottom);
	return (u >= 0.0f && u <= dot(leftRight, leftRight) && v >= 0.0f && v <= dot(downBottom, downBottom));
}


inline bool RubixCubeDiskManager::IsRayInFace(const RaySlim& ray, const Plane& plane, float& dist) const
{
	const auto divisor = dot(plane.m_normal, ray.m_direction);
	if (divisor < COMBUSTION_FLOAT_TOLERANCE)
	{
		const auto planeRay = plane.m_center - ray.m_origin;
		dist = dot(planeRay, plane.m_normal) / divisor;

		return (dist >= 0);
	}
	return false;
}
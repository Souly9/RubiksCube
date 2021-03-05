#pragma once

struct Plane;
struct RayWithMouse;
class RubixCube;

class RubixCubeDiskManager
{
public:
	/// <summary>
	/// Checks if the ray hits the rubix cube and if so, where
	/// </summary>
	/// <param name="ray">The ray to cast against</param>
	/// <param name="cubieIndex">The cubie the ray hits</param>
	/// <param name="rubiksCube">A pointer to enable access to attributes</param>
	/// <returns>The index of the face</returns>
	int RayCastCube(const RaySlim& ray, CVec<int>& cubieIndex, const RubixCube* rubiksCube) const;

	/*
	 * Helper function to cast against a 2d square and face
	 */
	
	bool IsRayInSquare(const glm::vec3& topLeft, const glm::vec3& downLeft, const glm::vec3& topRight, const glm::vec3& intersection) const;
	bool IsRayInFace(const RaySlim& ray, const Plane& plane, float& dist) const;
};


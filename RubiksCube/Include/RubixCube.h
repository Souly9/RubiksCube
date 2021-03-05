#pragma once
#include "VertexStructs.h"
#include <array>
#include "Cubie.h"

#include "DiskRotator.h"
#include "RubixCubeDiskManager.h"

struct Plane;
struct RayWithMouse;
class Object;

class RubixCube : public Object
{
private:
	friend class RubixCubeDiskManager;
	std::unique_ptr<RubixCubeDiskManager> m_pRayCastHandler;
	std::vector<std::shared_ptr<Cubie>> m_cubies;
	std::array<std::array<std::array<int, 3>, 3>, 3> m_logicalCube;
	std::array<CVec<int>, 6> m_centerPieces;
	std::array<std::array<int, 4>, 3> m_faceIndices;
	std::vector<int> m_curIndices;
	std::vector<RenderBatch*> m_cubieRenderBatches;
	CVec<int> m_curCubeIndex;

	std::unique_ptr<DiskRotator> m_pRotator;

	int m_rotateIndex;

	glm::vec3 m_center;
	float m_sideLengthCubie;
	float m_sideLengthCubieFace;

	int m_xCubes;
	int m_yCubes;

	RubixCubeEnums::AnimatorState m_animState;
	MouseRotation m_rotatorState;

	std::shared_ptr<Cubie> CreateCubie(const glm::vec3& center, const int& x, const int& y, const int& z,
	                                   Mesh3D* baseMesh,
	                                   const Material& mat);
	/**
	 * Determines the type (Edge, Side, Middle) of a cube given its position of the cube
	 * @param x The x position of the cubie in the parentCube
	 * @param y The y position of the cubie in the parentCube
	 * @param z The z position of the cubie in the parentCube
	 */
	void DetermineCubieColor(const int& x, const int& y, const int& z,
	                         std::vector<RubixCubeEnums::Color>& colors) const;
	/// <summary>
	/// Method to handle all raycast events
	/// </summary>
	/// <param name="ray">The ray that may hit the cube</param>
	void RayCastAgainst(const RayWithMouse& ray);

	/// <summary>
	/// Rotates the disk managed by DiskRotator for every move of the mouse while the left button is pressed
	/// </summary>
	/// <param name="mousePos">Mouse position in screen space</param>
	void RotateDisk(const glm::vec2& mousePos);
	/// <summary>
	/// Rotates the logical disk corresponding to the physical disk that got rotated once the mouse got released
	/// </summary>
	/// <param name="axis">The logical rotation axis</param>
	/// <param name="type">The rotation direction</param>
	/// <param name="turns">The number of rotations (90Åã)</param>
	void RotateLogicalDisc(const RubixCubeEnums::RotationAxis& axis, const RubixCubeEnums::RotationType& type,
	                       const int& turns);
	void EndDiskRotation(const glm::vec2& mousePos);
	bool CheckSolved();
	void Solved();
	/// <summary>
	/// Shuffles the cube with a random series of moves
	/// </summary>
	void Shuffle();
	/// <summary>
	/// Get next cube position in the logical model based on a given face and position
	/// </summary>
	/// <param name="color">The color of the face</param>
	/// <param name="in">The current position</param>
	/// <param name="x">The next x position</param>
	/// <param name="y">The next y position</param>
	/// <returns>The cube index of the next cube in x and y direction of the face</returns>
	CVec<int> GetNextLogicalPos(const RubixCubeEnums::Color& color, const CVec<int>& in, const int& x,
	                            const int& y) const;
	void CollectXTurnIndices(const CVec<int>& indices, std::vector<int>& rslt) const;
	void CollectYTurnIndices(const CVec<int>& indices, std::vector<int>& rslt) const;
	void CollectZTurnIndices(const CVec<int>& indices, std::vector<int>& rslt) const;
public:
	RubixCube(Mesh3D* baseMesh, std::vector<std::shared_ptr<RenderBatch>>& renderBatches, const glm::vec3& center,
	          float sideLength);
	~RubixCube() = default;
	RubixCube(const RubixCube& cube) = delete;
	RubixCube& operator=(const RubixCube& cube) = delete;
	// Explicitly enable moving again, though we likely don't need it
	RubixCube(RubixCube&& cube) = default;
	RubixCube& operator=(RubixCube&& cube) = default;

	void HookEvents();
};

#pragma once
class DiskRotator
{
public:
	DiskRotator(const float& anglePerPixel);
	/// <summary>
	/// Initializes the rotator
	/// </summary>
	/// <param name="mousePos">The mouse position in screen space</param>
	/// <param name="params">Struct to provide easier and cache friendlier accesss to secondary paramteres like the inversed model matrix</param>
	/// <param name="parent">The parent the disk should be attached to</param>
	void StartRotation(const glm::vec2& mousePos, RubixCubeStructs::RotatorInitParams&& params, Object* parent);

	/// <summary>
	/// Rotates the internal Disk according to the change of mouse position
	/// Doesn't initialize itself
	/// </summary>
	/// <param name="mousePos">The mouse position in screen space</param>
	void ContinueRotation(const glm::vec2& mousePos);
	// Returns the final values
	void EndRotation(const glm::vec2& mousePos, RubixCubeEnums::RotationType& type, RubixCubeEnums::RotationAxis& logicalRotationAxis, int& numberOfTurns);

	std::shared_ptr<Object> GetDisk() const;
	glm::quat GetRot() const;
	bool IsFirstRot() const;
	glm::vec3 GetRotAxis() const;
	RubixCubeEnums::RotationAxis GetRotType() const;
private:
	std::shared_ptr<Object> m_pCurrentDisk;
	glm::quat m_accRotation{};
	glm::quat m_lastRotation{};
	
	glm::vec3 m_curRotationAxis{};
	glm::vec3 m_lastCubeMousePos;
	
	RubixCubeEnums::RotationAxis m_curAxisType;
	RubixCubeStructs::RotatorInitParams m_initialParams;
	RubixCubeEnums::AnimatorState m_state;
	
	glm::vec2 m_lastMousePos{};
	glm::vec2 m_lastVel{};
	
	float m_anglePerPixel;
	float m_accAngle{};
	bool m_isAxisSelected{false};
	bool m_wasAxisSelectedLastFrame{false};
};


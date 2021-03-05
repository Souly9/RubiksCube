#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(float fov, float zNear, float zFar, float aspectRatio, glm::vec4 pos, glm::quat rot);
	~Camera();

	// Updates all matrices
	void UpdateMatrices();

	void LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 upDirection);

	/**
	 * Getters for the camera's matrices
	 */
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix() const;

	/**
	 * Getters and Setters for camera variables needed to adjust the matrices
	 */
	void set_FoV(float fov);
	// Using the vector calling convention as we're pasÅLsing in a vector solely used for calculations
	void SetPositions(glm::vec4 position);
	glm::vec3 GetPosition() const;

	void SetRotation(const glm::quat& rotation);
	glm::quat GetRotation() const;

	void Translate(glm::vec4 trans);
	void Scroll(float val);
private:
	__declspec(align(16)) struct AlignedCameraData
	{
		glm::vec4 m_position;
		glm::vec4 m_target;
		glm::vec4 m_upDirection;
		glm::quat m_rotationQuat;

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;

		glm::mat4 m_viewProjMatrix;
	};

	std::unique_ptr<AlignedCameraData> m_pData;
	MouseRotation m_rotatorState;
	
	float m_FoV;
	float m_zNear;
	float m_zFar;
	float m_AspectRatio;

	bool m_isDirty;
};

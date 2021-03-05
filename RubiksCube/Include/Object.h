#pragma once
#include <glm/fwd.hpp>
#include <glm/gtx/quaternion.hpp>

class Object
{
protected:
	glm::mat4 m_model{};
	
	
	glm::vec3 m_scale{1,1,1};
	bool m_isDirty{true};
	glm::vec3 m_translation{0,0,0};
	glm::quat m_rotation{1,0,0,0};
	std::string m_name;
	Object* m_parent{nullptr};
public:
	
	
	void ResetMatrix()
	{
		m_translation = glm::vec3{0,0,0};
		m_rotation = glm::quat{1,0,0,0};
		m_scale = glm::vec3{1, 1, 1};
		m_isDirty = true;
	}

	glm::mat4 GetModel()
	{
		if (m_isDirty)
		{
			const glm::mat4 translationMatrix{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				m_translation.x, m_translation.y, m_translation.z, 1
			};
			const auto rotMatrix = toMat4(m_rotation);
			const glm::mat4 scaleMat = {
				m_scale.x, 0, 0, 0,
				0, m_scale.y, 0, 0,
				0, 0, m_scale.z, 0,
				0, 0, 0, 1
			};
			m_model = translationMatrix * scaleMat * rotMatrix;
			m_isDirty = false;
		}
		if (m_parent != nullptr)
			return m_parent->GetModel() * m_model;
		return m_model;
	}

	void Translate(glm::vec3 trans);
	void Scale(float scale);
	void Rotate(const glm::quat& rot);
	void AdditiveRotation(const glm::quat& rot);
	void SetName(std::string&& name);
	
	void AdditiveTranslation(glm::vec3 trans);
	void SetParent(Object* parent);
	void DisconnectParentAdditive(Object* newParent = nullptr);
	void WriteValuesAdditive(glm::vec3& translate, glm::quat& rot, glm::vec3& scale);
	glm::quat GetRotation() const;
	glm::vec3 GetTranslation() const;
	glm::vec3 GetScale() const;
};



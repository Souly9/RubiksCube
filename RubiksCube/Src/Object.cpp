#include "stdafx.h"
#include <Object.h>

 void Object::Translate(glm::vec3 trans)
{
	m_translation = trans;
	m_isDirty = true;
}

 void Object::Scale(float scale)
{
	m_scale = glm::vec3(scale, scale, scale);
	m_isDirty = true;
}

void Object::AdditiveRotation(const glm::quat& rot)
{
	m_rotation = rot * m_rotation;
 	m_rotation = normalize(m_rotation);
	m_isDirty = true;
}

void Object::SetName(std::string&& name)
{
 	m_name = name;
}

glm::quat Object::GetRotation() const
{
 	return m_rotation;
}

glm::vec3 Object::GetTranslation() const
{
 	return m_translation;
}

glm::vec3 Object::GetScale() const
{
 	return m_scale;
}

void Object::AdditiveTranslation(glm::vec3 trans)
{
	m_translation += trans;
	m_isDirty = true;
}

void Object::SetParent(Object* parent)
{
	m_parent = parent;
}

void Object::DisconnectParentAdditive(Object* newParent)
{
 	m_parent->WriteValuesAdditive(m_translation, m_rotation, m_scale);
 	m_parent = newParent;
 	m_isDirty = true;
}

void Object::WriteValuesAdditive(glm::vec3& translate, glm::quat& rot, glm::vec3& scale)
{
 	translate += m_translation;
 	rot = glm::normalize(m_rotation * rot);
 	scale *= m_scale;
}

void Object::Rotate(const glm::quat&  rot)
{
	m_rotation = rot;
	m_isDirty = true;
}

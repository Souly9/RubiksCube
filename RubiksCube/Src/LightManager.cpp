#include "stdafx.h"
#include "LightManager.h"

LightManager::LightManager(float ambientStr) :
	m_pBuffer(std::make_unique<LightData>(ambientStr)),
	m_bufferIndex(COMBUSTION_GENERIC_LIGHT_BUFFER_INDEX)
	, m_bufferHandle(0), m_isDirty(true)
	, m_isInitialized(false)
{
}

void LightManager::AddLight(PointLight& light)
{
	m_pBuffer->m_pointLights.push_back(light);
	m_isDirty = true;
}

void LightManager::AddLights(std::vector<PointLight> lights)
{
	for (int i = 0; i < lights.size(); ++i)
	{
		m_pBuffer->m_pointLights.push_back(lights[i]);
	}
	m_isDirty = true;
}

void LightManager::RemoveLight(PointLight light)
{
	for (int i = 0; i < m_pBuffer->m_pointLights.size(); ++i)
	{
		if (&m_pBuffer->m_pointLights.at(i) == &light)
		{
			m_pBuffer->m_pointLights.erase(m_pBuffer->m_pointLights.begin() + i);
		}
	}
	m_isDirty = true;
}

void LightManager::RemoveAtIndex(int index)
{
	m_pBuffer->m_pointLights.erase(m_pBuffer->m_pointLights.begin() + index);
	m_isDirty = true;
}


bool LightManager::SubmitLightBuffer()
{
	if (!m_isInitialized)
	{
		glGenBuffers(1, &m_bufferHandle);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_bufferIndex, m_bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight) * COMBUSTION_MAX_FORWARD_LIGHTS + 7, nullptr,
		             GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		m_isDirty = true;
		m_isInitialized = true;
	}

	if (m_isDirty)
	{
		const glm::vec3 c = DataHolder::Get()->GetCameraRaw()->GetPosition();
		glBindBufferBase(GL_UNIFORM_BUFFER, m_bufferIndex, m_bufferHandle);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &m_pBuffer->m_ambientStrength);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), (sizeof(float) * 3), &c);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + (sizeof(float) * 3), sizeof(PointLight) * m_pBuffer->m_pointLights.size(),
		                m_pBuffer->m_pointLights.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	return true;
}

void LightManager::MultiplyLights(glm::mat4 mat)
{
	m_pBuffer->m_pointLights.clear();
	for (int i = 0; i < m_pointLights.size(); ++i)
	{
		const PointLight& light = m_pointLights.at(i);
		m_pBuffer->m_pointLights.push_back(
			PointLight{mat * light.m_position, light.m_color});
	}
	m_isDirty = true;
}

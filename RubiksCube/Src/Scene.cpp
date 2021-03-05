#include "stdafx.h"
#include "Scene.h"

Scene::Scene(std::shared_ptr<Camera> cam) : m_pBuffer(std::make_unique<SceneUniformBuffer>()),
                                            m_pLightManager(std::make_unique<LightManager>(COMBUSTION_AMBIENT_STRENGTH)),
                                            m_pMainCamera(cam), m_isDirty(true), m_isInitialized(false),
                                            m_bufferIndex(COMBUSTION_SCENE_BUFFER_INDEX)
{
}

void Scene::BeginScene()
{
	if (!m_isInitialized)
	{
		glGenBuffers(1, &m_uniformBufferHandle);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_bufferIndex, m_uniformBufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(SceneUniformBuffer), nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		m_isDirty = true;
		m_isInitialized = true;
	}

	if (m_isDirty)
	{
		PrepareBuffer();
		glBindBufferBase(GL_UNIFORM_BUFFER, m_bufferIndex, m_uniformBufferHandle);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SceneUniformBuffer), m_pBuffer.get());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		m_pLightManager->SubmitLightBuffer();
	}
}

void Scene::PrepareBuffer()
{
	m_pBuffer->m_viewMatrix = m_pMainCamera->GetViewMatrix();
	m_pBuffer->m_projMatrix = m_pMainCamera->GetProjectionMatrix();

	DataHolder::Get()->GetTimes(m_pBuffer->m_totalTime, m_pBuffer->m_deltaTime);
}

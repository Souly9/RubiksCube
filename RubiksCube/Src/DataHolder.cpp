#include "stdafx.h"
#include <DataHolder.h>

static std::unique_ptr<DataHolder> m_pSingleton;

DataHolder::DataHolder() : m_pClock(std::make_unique<HighResClock>())
{
	m_pMainCamera = std::make_unique<Camera>(90.0f, 0.1f, 1000.0f,
	                                         WindowManager::Get().GetScreenWidth() / WindowManager::Get().GetScreenHeight(),
	                                         glm::vec4(0, 3, 50, 1),
	                                         glm::quat(1, 0, 0, 0));
}

DataHolder* DataHolder::Get()
{
	if (!m_pSingleton)
	{
		m_pSingleton = std::make_unique<DataHolder>();
	}
	return m_pSingleton.get();
}

void DataHolder::GetTimes(float& totalTime, float& deltaTime) const
{
	totalTime = static_cast<float>(m_pClock->GetTotalSeconds());
	deltaTime = static_cast<float>(m_pClock->GetDeltaSeconds());
}

std::shared_ptr<Camera> DataHolder::GetCameraShared() const
{
	return m_pMainCamera;
}

Camera* DataHolder::GetCameraRaw() const
{
	return m_pMainCamera.get();
}

void DataHolder::Update()
{
	m_pClock->Tick();
	m_pMainCamera->UpdateMatrices();
}

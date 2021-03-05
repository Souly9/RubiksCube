#include "stdafx.h"
#include "Program.h"

#include "ArcBall.h"
#include "RayCaster.h"

int Program::m_materialIndex;

Program::Program(const char* windowName) : m_pWindowManager(
	std::unique_ptr<WindowManager>(WindowManager::Create(1920, 1080, windowName)))
{
	ArcBall::HookEvents();
	RayCaster::Init(m_pWindowManager->GetScreenWidth(), m_pWindowManager->GetScreenHeight());	
}

Program::~Program()
{
	m_pWindowManager->Close();
}

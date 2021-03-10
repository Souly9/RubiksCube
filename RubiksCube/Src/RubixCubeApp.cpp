#include "stdafx.h"
#include "RubixCubeApp.h"
#include "ArcBall.h"
#include "Shapes.h"
#include "DeferredRenderer.h"

using namespace MaterialUtilities;

RubixCubeApp::RubixCubeApp(const char* windowName) : Program{windowName}
{
	ArcBall::UpdatePos
	(
		glm::vec2(m_pWindowManager->GetScreenWidth() / 2.0f, m_pWindowManager->GetScreenHeight() / 2.0f),
		1.0f *
		static_cast<float>(sqrt
			(
				m_pWindowManager->GetScreenWidth() * m_pWindowManager->GetScreenWidth() +
				m_pWindowManager->GetScreenHeight() * m_pWindowManager->GetScreenHeight()
			))
	);
	
	DataHolder::Get()->GetCameraShared()->LookAt(glm::vec4(0, 3, 5, 1), glm::vec4(0, 0, 0, 1), glm::vec3(0, 1, 0));
	Shader normalShader("SkyboxVert.vs", "SkyboxFragDeferred.frag");

	std::vector<UniformAttribute> uniforms;
	uniforms.push_back(UniformAttribute::CUBEMAP);

	Properties props(glm::vec3(1,1,1),
	                 glm::vec3(1,1,1),
	                 glm::vec3(1,1,1),
	                 0.3f * 32);

	std::shared_ptr<TextureData> tmp = std::make_unique<TextureData>();
	tmp->m_cubeMap = TextureManager::Get()->GetCubemapHandle(
		"Yokohama3/", true);
	std::vector<MaterialUtilities::ShaderAttribute> tmp2;
	const std::shared_ptr<Material> cubeMap = std::make_shared<Material>(MaterialPreset::Default, normalShader, m_materialIndex++, props, tmp, tmp2);

	const auto skyboxCube = std::make_shared<Mesh3D>(SkyboxCube::GetSkyBoxCube());
	GeometryManager::Get()->AddGeometry(skyboxCube, *cubeMap);

	const auto batch = std::make_shared<RenderBatch>(cubeMap, DataHolder::Get()->GetCameraShared());
	batch->CollectGeometry();
	m_renderBatches.push_back(batch);
	
	LoadModels();
	SetupGLStates();
	
	m_pDeferredRenderer = std::make_unique<DeferredRenderer>();
	
	m_pMainScene = std::make_unique<Scene>(DataHolder::Get()->GetCameraShared());
	PointLight light{glm::vec4(0, 0, 3, 1),
		glm::vec4(1, 1, 1, 1)};
	m_pMainScene->GetLightManager()->AddLight(light);
	light = PointLight{glm::vec4(0, 3, 0, 1),
		glm::vec4(1,1,1, 1)};
	m_pMainScene->GetLightManager()->AddLight(light);
	light = PointLight{glm::vec4(0, -3, 0, 1),
		glm::vec4(1, 1, 1, 1)};
	
	m_pMainScene->GetLightManager()->AddLight(light);
}

void RubixCubeApp::Run()
{
	while (!glfwWindowShouldClose(m_pWindowManager->GetWindow()))
	{
		m_pMainScene->BeginScene();

		Update();

		Render();

		glfwSwapBuffers(m_pWindowManager->GetWindow());
		glfwPollEvents();
	}
}

void RubixCubeApp::Update()
{
	DataHolder::Get()->Update();
	static auto frameCount = 0;
	static double elapsedTime = 0.0;

	float delta, d;
	DataHolder::Get()->GetTimes(d, delta);

	elapsedTime += delta;
	frameCount++;

	if (elapsedTime > 1.0f)
	{
		auto fps = frameCount / elapsedTime;
		
		std::cout << "FPS: " << fps << '\n';

		frameCount = 0;
		elapsedTime = 0.0;
	}
	SignalSystem::ApplicationEvents::Get()->onUpdate(delta);
}

void RubixCubeApp::Render()
{
	m_pDeferredRenderer->Clear();
	// Skybox cant have depth test enabled
	glDisable(GL_DEPTH_TEST);
	m_pDeferredRenderer->Render(m_renderBatches.at(0).get());
	glEnable(GL_DEPTH_TEST);
	
	for (auto i = 1; i < m_renderBatches.size() ; ++i)
	{
		m_pDeferredRenderer->Render(m_renderBatches.at(i).get());
	}
	m_pDeferredRenderer->Assemble();
}

void RubixCubeApp::SetupGLStates()
{
	// During init, enable debug output
	
#ifdef NDEBUG
  glDisable(GL_DEBUG_OUTPUT);
#else
    glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);
#endif
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glFrontFace(GL_CCW);
}

void RubixCubeApp::LoadModels()
{
	m_baseCubeMesh =  std::make_unique<Mesh3D>(Square::Create24VertCube());
	m_cube = std::make_unique<RubixCube>(m_baseCubeMesh.get(), m_renderBatches, glm::vec3(0,0,0), 1);
}

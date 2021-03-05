#include <stdafx.h>
#include <Combustion.h>

using namespace MaterialUtilities;

Combustion::Combustion(const char* windowName) : Program(windowName)
{
	InputManager::m_arcBall->UpdatePos
	(
		glm::vec2(m_pWindowManager->GetScreenWidth() / 2.0f, m_pWindowManager->GetScreenHeight() / 2.0f),
		0.5f *
		static_cast<float>(sqrt
			(
				m_pWindowManager->GetScreenWidth() * m_pWindowManager->GetScreenWidth() +
				m_pWindowManager->GetScreenHeight() * m_pWindowManager->GetScreenHeight()
			))
	);

	DataHolder::Get()->GetCamera()->LookAt(glm::vec4(0, 3, 50, 1), glm::vec4(0, 0, 0, 1), glm::vec3(0, 1, 0));

	LoadModels();
	SetupGLStates();

	m_pForwardRenderer = std::make_unique<ForwardRenderer>();
	std::shared_ptr<RenderBatch> teapot = std::make_shared<RenderBatch>(&m_materialMap[MaterialPreset::Default], 1,
	                                                                    DataHolder::Get()->GetCamera());
	// Required for the renderbatch to get all his geometry, should integrate that into the RenderBatch class tbh
	teapot->CollectGeometry();
	m_renderBatches.push_back(teapot);

	m_pMainScene = std::make_unique<Scene>(DataHolder::Get()->GetCamera());
	PointLight light(glm::vec4(0, 0, -50, 0),
	                 glm::vec4(1, 1, 1, 1));
	m_pMainScene->GetLightManager()->AddLight(light);
}

void Combustion::Run()
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

void Combustion::Update()
{
	DataHolder::Get()->Update();
	static uint64_t frameCount = 0;
	static double elapsedTime = 0.0;

	float delta, d;
	DataHolder::Get()->GetTimes(d, delta);

	elapsedTime += delta;
	frameCount++;

	if (elapsedTime > 1.0)
	{
		double fps = frameCount / elapsedTime;


		std::cout << fps << '\n';

		frameCount = 0;
		elapsedTime = 0.0;
	}
	glm::mat4 mat;
	InputManager::m_arcBall->GetRotationMatrix(mat);
	m_pMainScene->GetLightManager()->MultiplyLights(mat);
}

void Combustion::Render()
{
	for (int i = 0; i < m_renderBatches.size(); ++i)
	{
		RenderBatch* batch = m_renderBatches.at(i).get();
		m_pForwardRenderer->Render(batch);
	}
}

void Combustion::SetupGLStates()
{
	// During init, enable debug output
	glEnable              ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( MessageCallback, 0 );
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glEnable(GL_FRAMEBUFFER_SRGB);
}

void Combustion::LoadModels()
{
	ObjReader reader;
	std::shared_ptr<Mesh3D> teapot = std::make_shared<Mesh3D>(
		reader.readOBJ("D:/Programme/FH/CombustionOpenGL/input/utah-teapot.obj"));
	std::shared_ptr<Mesh3D> sphere = std::make_shared<Mesh3D>(
		reader.readOBJ("D:/Programme/FH/CombustionOpenGL/input/Sphere.obj"));
	//Shader class initialization for the different modes and to have easier access to the shaders in custom files
	//Shader displacement("Shaders/PlainVert.vs", "Shaders/Phong.frag");
	Shader normalShader("Shaders/PlainVertNormalMap.vs", "Shaders/PhongNormalMap.frag");

	uint64_t opaqueHandle = TextureManager::Get()->GetTextureHandle(
		"D:/Programme/FH/CombustionOpenGL/Rock/Rock035_2K_Color.jpg");
	uint64_t disp = TextureManager::Get()->GetTextureHandle(
		"D:/Programme/FH/CombustionOpenGL/Rock/Rock035_2K_Displacement.jpg");
	uint64_t normal = TextureManager::Get()->GetTextureHandle(
		"D:/Programme/FH/CombustionOpenGL/Rock/Rock035_2K_Normal.jpg");
	

	std::shared_ptr<TextureData> tmp = std::make_unique<TextureData>();
	tmp->m_albedo = opaqueHandle;
	tmp->m_displacement = disp;
	tmp->m_normalMap = normal;

	std::vector<ShaderAttribute> attbs;
	attbs.push_back(ShaderAttribute::NORMALS);
	attbs.push_back(ShaderAttribute::TEXCOORDS);
	attbs.push_back(ShaderAttribute::TANGENTSPACE);

	std::vector<UniformAttribute> uniforms;
	uniforms.push_back(UniformAttribute::ALBEDO);
	uniforms.push_back(UniformAttribute::DISPLACEMENTMAP);
	uniforms.push_back(UniformAttribute::NORMALMAP);

	Properties props(glm::vec3(0.05375f, 0.05f, 0.06625f),
	                 glm::vec3(0.18275f, 0.17f, 0.22525f),
	                 glm::vec3(0.337241f, 0.328634f, 0.346435f),
	                 0.3f * 128);

	Material phong(MaterialPreset::Default, normalShader, 1, props, tmp, attbs);
	//m_materialMap[MaterialPreset::Default] = phong;

	GeometryManager::Get()->AddGeometry(teapot, phong);
	GeometryManager::Get()->AddGeometry(sphere, phong);
	teapot->Translate(glm::vec3(30, 10, 0));
	sphere->Scale(2);
	sphere->Translate(glm::vec3(-10,0,0));
}

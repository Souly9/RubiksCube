#include "stdafx.h"

static std::unique_ptr<WindowManager> m_pSingleton;

WindowManager* WindowManager::Create(uint32_t width, uint32_t height, const char* title)
{
	if (!m_pSingleton)
	{
		m_pSingleton = std::make_unique<WindowManager>(width, height, title);
	}
	return m_pSingleton.get();
}

WindowManager::WindowManager(uint32_t width, uint32_t height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, COMBUSTION_GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, COMBUSTION_GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_pWindow = std::unique_ptr<GLFWwindow, DestroyglfwWin>(glfwCreateWindow(width, height, title, nullptr, nullptr));
	glfwMakeContextCurrent(m_pWindow.get());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glViewport(0, 0, width, height);
	m_screenWidth = width;
	m_screenHeight = height;

	glfwSetFramebufferSizeCallback(m_pWindow.get(), FramebufferSizeCallback);
	glfwSetMouseButtonCallback(m_pWindow.get(), m_pInputManager->OnMouseButton);
	glfwSetCursorPosCallback(m_pWindow.get(), m_pInputManager->OnMouseMove);
	glfwSetScrollCallback(m_pWindow.get(), m_pInputManager->OnMouseScroll);
	glfwSetKeyCallback(m_pWindow.get(), InputManager::OnKeyPress);
}

void WindowManager::Close() noexcept
{
	glfwTerminate();
	m_pSingleton.release();
}

void WindowManager::FramebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept
{
	MAYBE_UNUSED(window);
	SignalSystem::ApplicationEvents::Get()->onScreenResize(glm::vec2(width, height));
	glViewport(0, 0, width, height);
}

GLFWwindow* WindowManager::GetWindow() noexcept
{
	assert(m_pWindow.get());
	return m_pWindow.get();
}

WindowManager& WindowManager::Get() noexcept
{
	assert(m_pSingleton.get());
	return *m_pSingleton.get();
}

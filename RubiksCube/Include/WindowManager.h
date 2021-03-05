#pragma once
#include "MemoryUtilities.h"

struct GLFWwindow;

class WindowManager
{
public:
	static WindowManager* Create(uint32_t width, uint32_t height, const char* title);
	WindowManager(uint32_t width, uint32_t height, const char* title);

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept;

	GLFWwindow* GetWindow() noexcept;

	static WindowManager& Get() noexcept;

	void Close() noexcept;

	uint32_t GetScreenHeight() const noexcept { return m_screenHeight; }
	uint32_t GetScreenWidth() const noexcept { return m_screenWidth; }
private:
	WindowManager() = delete;
	WindowManager(const WindowManager& copy) = delete;
	WindowManager& operator=(const WindowManager& other) = delete;

	std::unique_ptr<GLFWwindow, DestroyglfwWin> m_pWindow;
	std::unique_ptr<InputManager> m_pInputManager;

	uint32_t m_screenHeight;
	uint32_t m_screenWidth;
};

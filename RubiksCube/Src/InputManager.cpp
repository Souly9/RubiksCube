#include "stdafx.h"
#include "InputManager.h"

bool InputManager::m_rightMouseButtonPressed = false;
bool InputManager::m_leftMouseButtonPressed = false;

void InputManager::OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	MAYBE_UNUSED(window);
	if (m_rightMouseButtonPressed)
	{
		SignalSystem::InputSignals::Get()->onMouseDrag(glm::vec2(ComputeX(xpos), ComputeY(ypos)));
	}
	if (m_leftMouseButtonPressed)
	{
		SignalSystem::InputSignals::Get()->onMouseLeftDrag(glm::vec2{xpos, ypos});
	}
}

void InputManager::OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	MAYBE_UNUSED(window);
	MAYBE_UNUSED(xoffset);
	SignalSystem::InputSignals::Get()->onMouseScroll(-yoffset);
}

void InputManager::OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	MAYBE_UNUSED(window);
	MAYBE_UNUSED(mods);
	MAYBE_UNUSED(scancode);
	    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			SignalSystem::InputSignals::Get()->onSpacePress();
}

double InputManager::ComputeY(double yPos)
{
	return WindowManager::Get().GetScreenHeight() - yPos;
}

double InputManager::ComputeX(double xPos)
{
	return WindowManager::Get().GetScreenWidth() - xPos;
}

void InputManager::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	MAYBE_UNUSED(mods);
	using namespace SignalSystem;
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		m_rightMouseButtonPressed = true;
		InputSignals::Get()->onMouseRightClick(glm::vec2(ComputeX(xpos), ComputeY(ypos)));
		InputSignals::Get()->onMouseStartRightDrag();
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		m_rightMouseButtonPressed = false;
		InputSignals::Get()->onMouseStopRightDrag();
	}
	
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		m_leftMouseButtonPressed = true;
		InputSignals::Get()->onMouseLeftClick(glm::vec2{xpos, ypos});
	}
	
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		m_leftMouseButtonPressed = false;
		InputSignals::Get()->onMouseStopLeftDrag(glm::vec2{xpos, ypos});
	}
}

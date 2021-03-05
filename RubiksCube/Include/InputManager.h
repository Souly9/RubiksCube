#pragma once

class GLFWwindow;
class Camera;

// Collection of glfw callbacks for input
class InputManager
{
public:
	static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
	static void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
	static void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	static void OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	static inline double ComputeY(double yPos);
	static inline double ComputeX(double xPos);
	static bool m_rightMouseButtonPressed;
	static bool m_leftMouseButtonPressed;
};

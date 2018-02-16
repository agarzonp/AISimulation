#ifndef INPUT_H
#define INPUT_H


// The main class that wants to be notified about input events must implement InputListener
class InputListener
{
public:

	virtual void OnKeyPressed(int key) {};
	virtual void OnKeyReleased(int key) {};
	virtual void OnMouseButtonPressed(int button, double x, double y) {};
	virtual void OnMouseButtonReleased(int button, double x, double y) {};
	virtual void OnMouseScroll(double xOffset, double yOffset) {};
	virtual void OnMouseMove(double x, double y) {};
};

// Input class
class Input
{
	static GLFWwindow* s_window;
	static InputListener* s_listener;

public:

	// set target window
	static void SetWindow(GLFWwindow* window)
	{
		s_window = window;
	}

	// set listener
	static void SetListener(InputListener* listener)
	{
		s_listener = listener;
	}

	// Keyboard events

	static bool isKeyPressed(int key)
	{
		return (glfwGetKey(s_window, key) == GLFW_PRESS);
	}

	static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			s_listener->OnKeyPressed(key);
		}
		else if (action == GLFW_RELEASE)
		{
			s_listener->OnKeyReleased(key);
		}
	}

	// Mouse events 

	static bool isMouseButtonPressed(int button)
	{
		return (glfwGetMouseButton(s_window, button) == GLFW_PRESS);
	}

	static void OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		if (action == GLFW_PRESS)
		{
			s_listener->OnMouseButtonPressed(button, x, y);
		}
		else if (action == GLFW_RELEASE)
		{
			s_listener->OnMouseButtonReleased(button, x, y);
		}
	}

	static void OnMouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		s_listener->OnMouseScroll(xOffset, yOffset);
	}

	static void OnMouseMoveCallback(GLFWwindow* window, double x, double y)
	{
		s_listener->OnMouseMove(x, y);
	}
};

GLFWwindow* Input::s_window = nullptr;
InputListener* Input::s_listener = nullptr;

#endif // !INPUT_H


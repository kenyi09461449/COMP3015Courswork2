#pragma once
#include <GLFW/glfw3.h>
#include <utility>
#include <iostream>

class Input {
public:
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		Instance().X_offset = xpos - windowWidth / 2;
		Instance().Y_offset = ypos - windowHeight / 2;
		glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
		/*if (Instance().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			int windowWidth, windowHeight;
			glfwGetWindowSize(window, &windowWidth, &windowHeight);
			Instance().X_offset = 0.1 * (xpos - windowWidth / 2);
			Instance().Y_offset = 0.1 * (ypos - windowHeight / 2);
		}
		else {
			Instance().X_offset = 0;
			Instance().Y_offset = 0;
		}*/
		//std::cout << xpos << " " << ypos << std::endl;
	};

	void Init(GLFWwindow* window) { 
		m_window = window; 
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPosCallback(window, &Input::cursorPosCallback);
		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
	};

	static Input& Instance()
	{
		static Input instance;
		return instance;
	}
	

	bool IsKeyPressed(int keycode) {
		auto state = glfwGetKey(m_window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool IsMouseButtonPressed(int button)
	{
		auto state = glfwGetMouseButton(m_window, button);
		return state == GLFW_PRESS;
	}
	std::pair<float, float> GetMousePosition()
	{
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}
	float GetMouseX_move()
	{
		//auto x = GetMousePosition();
		return X_offset;
	}
	float GetMouseY_move()
	{
		//auto x = GetMousePosition();
		return Y_offset;
	}
private:
	Input(){}
	~Input() {}
	//Input(const Input&) {};
	Input& operator=(const Input&) {};

private:
	GLFWwindow* m_window;
	double X_offset;
	double Y_offset;
};
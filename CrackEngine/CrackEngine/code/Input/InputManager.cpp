#include "InputManager.h"

InputManager::InputManager()
{
	init();
}

InputManager::InputManager(GLFWwindow* _window)
{
	window = _window;
	init();
}

bool InputManager::getButton(eInputs inputkey)
{
	if (inputMap.find(inputkey) != inputMap.end()) {
		InputButton& input = inputMap[inputkey];
		if (glfwGetKey(window, input.buttonIndex) == GLFW_PRESS)
		{
			return true;
		}
	}
	return false;
}

bool InputManager::getButtonDown(eInputs)
{
	return false;
}

bool InputManager::getButtonUp(eInputs)
{
	return false;
}

void InputManager::init()
{
	InputButton input_up = InputButton(GLFW_KEY_UP);
	inputMap[eInputs::NORTH] = input_up;
	InputButton input_down = InputButton(GLFW_KEY_DOWN);
	inputMap[eInputs::SOUTH] = input_down;
	InputButton input_left = InputButton(GLFW_KEY_LEFT);
	inputMap[eInputs::WEST] = input_left;
	InputButton input_right = InputButton(GLFW_KEY_RIGHT);
	inputMap[eInputs::EAST] = input_right;
	InputButton input_a = InputButton(GLFW_KEY_Z);
	inputMap[eInputs::A] = input_a;
	InputButton input_b = InputButton(GLFW_KEY_X);
	inputMap[eInputs::B] = input_b;
	InputButton input_c = InputButton(GLFW_KEY_C);
	inputMap[eInputs::C] = input_c;
	InputButton input_x = InputButton(GLFW_KEY_A);
	inputMap[eInputs::X] = input_x;
	InputButton input_y = InputButton(GLFW_KEY_S);
	inputMap[eInputs::Y] = input_y;
	InputButton input_z = InputButton(GLFW_KEY_D);
	inputMap[eInputs::Z] = input_z;
	InputButton input_s = InputButton(GLFW_KEY_SPACE);
	inputMap[eInputs::S] = input_s;
	InputButton input_confirm = InputButton(GLFW_KEY_Z);
	inputMap[eInputs::CONFIRM] = input_confirm;
	InputButton input_back = InputButton(GLFW_KEY_X);
	inputMap[eInputs::BACK] = input_back;
	InputButton input_pause = InputButton(GLFW_KEY_ENTER);
	inputMap[eInputs::PAUSE] = input_pause;
	return;
}

void InputManager::update()
{

}

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
	sinputFrame currentFrame = inputFrames[inputFrames.size() - 1];
	if (currentFrame.bActive)
	{
		switch (inputkey) {
		case NORTH:
			return currentFrame.bNorth;
			break;
		case SOUTH:
			return currentFrame.bSouth;
			break;
		case EAST:
			return currentFrame.bEast;
			break;
		case WEST:
			return currentFrame.bWest;
			break;
		case NORTHWEST:
			return currentFrame.bNorth && currentFrame.bWest;
			break;
		case NORTHEAST:
			return currentFrame.bNorth && currentFrame.bEast;
			break;
		case SOUTHWEST:
			return currentFrame.bSouth && currentFrame.bWest;
			break;
		case SOUTHEAST:
			return currentFrame.bSouth && currentFrame.bEast;
			break;
		case A:
			return currentFrame.bA;
			break;
		case B:
			return currentFrame.bB;
			break;
		case C:
			return currentFrame.bC;
			break;
		case X:
			return currentFrame.bX;
			break;
		case Y:
			return currentFrame.bY;
			break;
		case Z:
			return currentFrame.bZ;
			break;
		case S:
			return currentFrame.bS;
			break;
		case CONFIRM:
			return currentFrame.bConfirm;
			break;
		case BACK:
			return currentFrame.bBack;
			break;
		case PAUSE:
			return currentFrame.bPause;
			break;
		default:
			return false;
			break;
		};
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

std::vector<InputManager::sinputFrame> InputManager::getBuffer(int _frameCount)
{
	int frameCountDown = _frameCount;
	std::vector<sinputFrame> returnBuffer;
	for (int i = inputFrames.size() - 1; i >= 0; i--) {
		if (frameCountDown > inputFrames[i].frameCount) {
			if (inputFrames[i].bActive)
			{
				returnBuffer.push_back(inputFrames[i]);
				frameCountDown -= inputFrames[i].frameCount;
			}
			else
			{
				return returnBuffer;
			}
		}
		else {
			return returnBuffer;
		}
	}
	
	return std::vector<sinputFrame>();
}

void InputManager::init()
{
	InputButtonKeyboardKey* input_up = new InputButtonKeyboardKey(GLFW_KEY_UP);
	inputMap[eInputs::NORTH] = input_up;
	InputButtonKeyboardKey* input_down = new InputButtonKeyboardKey(GLFW_KEY_DOWN);
	inputMap[eInputs::SOUTH] = input_down;
	InputButtonKeyboardKey* input_left = new InputButtonKeyboardKey(GLFW_KEY_LEFT);
	inputMap[eInputs::WEST] = input_left;
	InputButtonKeyboardKey* input_right = new InputButtonKeyboardKey(GLFW_KEY_RIGHT);
	inputMap[eInputs::EAST] = input_right;
	InputButtonKeyboardKey* input_a = new InputButtonKeyboardKey(GLFW_KEY_Z);
	inputMap[eInputs::A] = input_a;
	InputButtonKeyboardKey* input_b = new InputButtonKeyboardKey(GLFW_KEY_X);
	inputMap[eInputs::B] = input_b;
	InputButtonKeyboardKey* input_c = new InputButtonKeyboardKey(GLFW_KEY_C);
	inputMap[eInputs::C] = input_c;
	InputButtonKeyboardKey* input_x = new InputButtonKeyboardKey(GLFW_KEY_A);
	inputMap[eInputs::X] = input_x;
	InputButtonKeyboardKey* input_y = new InputButtonKeyboardKey(GLFW_KEY_S);
	inputMap[eInputs::Y] = input_y;
	InputButtonKeyboardKey* input_z = new InputButtonKeyboardKey(GLFW_KEY_D);
	inputMap[eInputs::Z] = input_z;
	InputButtonKeyboardKey* input_s = new InputButtonKeyboardKey(GLFW_KEY_SPACE);
	inputMap[eInputs::S] = input_s;
	InputButtonKeyboardKey* input_confirm = new InputButtonKeyboardKey(GLFW_KEY_Z);
	inputMap[eInputs::CONFIRM] = input_confirm;
	InputButtonKeyboardKey* input_back = new InputButtonKeyboardKey(GLFW_KEY_X);
	inputMap[eInputs::BACK] = input_back;
	InputButtonKeyboardKey* input_pause = new InputButtonKeyboardKey(GLFW_KEY_ENTER);
	inputMap[eInputs::PAUSE] = input_pause;

	sinputFrame firstFrame = sinputFrame();
	firstFrame.frameCount = 0;
	inputFrames.push_back(firstFrame);
	return;
}

void InputManager::ClearBuffer()
{
	inputFrames[inputFrames.size() - 1].bActive = false;
}

void InputManager::update()
{
	sinputFrame currentFrame = pollCurrentInputs();
	if (inputFrames[inputFrames.size() - 1].compare(currentFrame) == true)
	{
		inputFrames[inputFrames.size() - 1].frameCount++;
	}
	else
	{
		inputFrames.push_back(currentFrame);
	}
}

InputManager::sinputFrame InputManager::pollCurrentInputs()
{
	sinputFrame currentFrame;
	if (evaluateButton(eInputs::NORTH))
	{
		currentFrame.bNorth = true;
	}
	if (evaluateButton(eInputs::SOUTH))
	{
		currentFrame.bSouth = true;
	}
	if (evaluateButton(eInputs::EAST))
	{
		currentFrame.bEast = true;
	}
	if (evaluateButton(eInputs::WEST))
	{
		currentFrame.bWest = true;
	}
	if (evaluateButton(eInputs::A))
	{
		currentFrame.bA = true;
	}
	if (evaluateButton(eInputs::B))
	{
		currentFrame.bB = true;
	}
	if (evaluateButton(eInputs::C))
	{
		currentFrame.bC = true;
	}
	if (evaluateButton(eInputs::X))
	{
		currentFrame.bX = true;
	}
	if (evaluateButton(eInputs::Y))
	{
		currentFrame.bY = true;
	}
	if (evaluateButton(eInputs::Z))
	{
		currentFrame.bZ = true;
	}
	if (evaluateButton(eInputs::S))
	{
		currentFrame.bS = true;
	}
	if (evaluateButton(eInputs::CONFIRM))
	{
		currentFrame.bConfirm = true;
	}
	if (evaluateButton(eInputs::BACK))
	{
		currentFrame.bBack = true;
	}
	if (evaluateButton(eInputs::PAUSE))
	{
		currentFrame.bPause = true;
	}
	// Impossibilities
	if (currentFrame.bNorth && currentFrame.bSouth)
	{
		currentFrame.bNorth = false;
		currentFrame.bSouth = false;
	}
	if (currentFrame.bEast && currentFrame.bWest)
	{
		currentFrame.bEast = false;
		currentFrame.bWest = false;
	}
	
	return currentFrame;
}

bool InputManager::evaluateButton(eInputs button)
{
	switch (button) {
	case NORTHEAST:
	{
		InputButton* inputNorth = inputMap[eInputs::NORTH];
		InputButton* inputEast = inputMap[eInputs::EAST];
		return (inputNorth->evaluate(window) && inputEast->evaluate(window));
		break;
	}
	case NORTHWEST:
	{
		InputButton* inputNorth = inputMap[eInputs::NORTH];
		InputButton* inputWest = inputMap[eInputs::WEST];
		return (inputNorth->evaluate(window) && inputWest->evaluate(window));
		break;
	}
	case SOUTHEAST:
	{
		InputButton* inputSouth = inputMap[eInputs::SOUTH];
		InputButton* inputEast = inputMap[eInputs::EAST];
		return (inputSouth->evaluate(window) && inputEast->evaluate(window));
		break;
	}
	case SOUTHWEST:
	{
		InputButton* inputSouth = inputMap[eInputs::SOUTH];
		InputButton* inputWest = inputMap[eInputs::WEST];
		return (inputSouth->evaluate(window) && inputWest->evaluate(window));
		break;
	}
	default:
	{
		if (inputMap.find(button) != inputMap.end()) {
			InputButton* input = inputMap[button];
			return input->evaluate(window);
		}
		break;
	}
	}
	return false;
}

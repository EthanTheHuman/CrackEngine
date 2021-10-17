#pragma once
#include "../Includes.h"

class InputButton
{
public:
	enum ButtonType
	{
		KEYBOARD = 0,
	};
	ButtonType buttonType;
	int buttonIndex;
	InputButton()
	{
		buttonType = ButtonType::KEYBOARD;
		buttonIndex = 0;
	}
	InputButton(int _index)
	{
		buttonType = ButtonType::KEYBOARD;
		buttonIndex = _index;
	}


private:
};

class InputManager
{
public:
	enum eInputs
	{
		NORTH = 0,
		NORTHEAST,
		EAST,
		SOUTHEAST,
		SOUTH,
		SOUTHWEST,
		WEST,
		NORTHWEST,
		A,
		B,
		C,
		X,
		Y,
		Z,
		S,
		CONFIRM,
		BACK,
		PAUSE,
		ANY
	};

	struct sButtonInfo
	{
		int iFrameCount;
		bool bButtonDown;
		bool bButton;
		bool bButtonUp;
	};

	struct sInputSnapshot
	{
		int iFrameCount;
		std::map<eInputs, sButtonInfo> inputs;
	};

public:
	InputManager();
	InputManager(GLFWwindow* _window);
	std::map<eInputs, InputButton> inputMap;
	void ResetBuffer();
	bool getButton(eInputs button);
	bool getButtonDown(eInputs button);
	bool getButtonUp(eInputs button);
	sInputSnapshot RollbackInputs(int _droppedFrames);

private:
	GLFWwindow* window;
	void init();
	void update();
	sInputSnapshot currentInputs;
	std::vector<sInputSnapshot> inputStack;
};


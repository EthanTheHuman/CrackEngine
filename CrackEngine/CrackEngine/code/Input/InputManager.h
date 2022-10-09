#pragma once
#include "../Includes.h"

class InputButton
{
public:
	InputButton()
	{
	}
	virtual bool evaluate(GLFWwindow* window)
	{
		return false;
	}

private:
};

class InputButtonKeyboardKey : public InputButton
{
public:
	InputButtonKeyboardKey(int _index)
	{
		buttonIndex = _index;
	}
	bool evaluate(GLFWwindow* window) override
	{
		if (glfwGetKey(window, buttonIndex) == GLFW_PRESS)
		{
			return true;
		}
		return false;
	}
protected:
	int buttonIndex;
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

	struct sinputFrame
	{
		bool bNorth = false;
		bool bSouth = false;
		bool bEast = false;
		bool bWest = false;
		bool bA = false;
		bool bB = false;
		bool bC = false;
		bool bX = false;
		bool bY = false;
		bool bZ = false;
		bool bS = false;
		bool bConfirm = false;
		bool bBack = false;
		bool bPause = false;
		int frameCount = 1;

		bool compare(sinputFrame _compareFrame) {
			if (bNorth == _compareFrame.bNorth
				&& bSouth == _compareFrame.bSouth
				&& bEast == _compareFrame.bEast
				&& bWest == _compareFrame.bWest
				&& bA == _compareFrame.bA
				&& bB == _compareFrame.bB
				&& bC == _compareFrame.bC
				&& bX == _compareFrame.bX
				&& bY == _compareFrame.bY
				&& bZ == _compareFrame.bZ
				&& bS == _compareFrame.bS
				&& bConfirm == _compareFrame.bConfirm
				&& bBack == _compareFrame.bBack) {
				return true;
			}
			return false;
		}
	};

	struct sInputSnapshot
	{
		int iFrameCount;
		std::map<eInputs, sButtonInfo> inputs;
	};

public:
	InputManager();
	InputManager(GLFWwindow* _window);
	std::map<eInputs, InputButton*> inputMap;
	void ResetBuffer();
	bool getButton(eInputs button);
	bool getButtonDown(eInputs button);
	bool getButtonUp(eInputs button);
	std::vector<sinputFrame> getBuffer(int _frameCount);
	sInputSnapshot RollbackInputs(int _droppedFrames);
	void update();

private:
	GLFWwindow* window;
	void init();
	sInputSnapshot currentInputs;
	std::vector<sInputSnapshot> inputStack;
	std::vector<sinputFrame> inputFrames;
	sinputFrame pollCurrentInputs();
	bool evaluateButton(eInputs button);
};


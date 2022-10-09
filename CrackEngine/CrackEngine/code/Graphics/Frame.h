#pragma once
#include "../../includes/json.hpp"
#include "../../includes/xml/pugixml.hpp"
#include "../../shader_m.h"
#include "../../stb_image.h"
#include <string>
#include "../Log.h"
using json = nlohmann::json;

class Frame
{
public:
	enum class InputCommand
	{
		EMPTY = -1,
		FORWARD = 0,
		BACK,
		UP,
		DOWN,
		UPFORWARD,
		UPBACK,
		NONE,
		ANY,
		DOUBLEFORWARD,
		DOUBLEBACK
	};

	enum class InputButton
	{
		EMPTY = -1,
		A,
		B,
		C,
		X,
		Y,
		Z,
		NONE,
		ANY
	};

	struct InputAction
	{
		int animChangeIndex = 0;
		Frame::InputCommand inputCommand = Frame::InputCommand::NONE;
		Frame::InputButton inputButton = Frame::InputButton::ANY;
	};
	
	enum class FrameActionType
	{
		ALWAYS = 0,
		INSTANT
	};

	struct FrameAction
	{
		FrameActionType frameActionType = FrameActionType::ALWAYS;
		float xDelta;
		float yDelta;
		float xVelocity;
		float yVelocity;
		float xAcceleration;
		float yAcceleration;
		int stepCount = 0;
	};

	Frame(json _frameData);
	Frame(pugi::xml_node _frameData);
	Frame(int _xScale = 1, int _yScale = 1, int _xPos = 0, int _yPos = 0, std::string _fileName = "", int _frameCount = 1, int _index = 0);
	int xScale;
	int yScale;
	int xPos;
	int yPos;
	unsigned int spriteImage;
	int spriteWidth, spriteHeight;
	int frameCount;
	int index;
	bool looping = false;
	std::vector<InputAction> inputActions;
	std::vector<FrameAction> frameActions;
	static std::vector<Frame::InputAction> readInputActionsFromXML(pugi::xml_node _data);
	static std::vector<Frame::FrameAction> readFrameActionsFromXML(pugi::xml_node _data);

protected:
	std::string spriteFileName;
private:
	static Frame::FrameAction readFrameActionFromXML(pugi::xml_node _data);
	// texture coordinates
	// texture width and height
};
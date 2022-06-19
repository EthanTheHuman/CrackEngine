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
	enum InputCommand
	{
		EMPTY = -1,
		FORWARD = 0,
		BACK,
		UP,
		DOWN,
		NONE
	};

	struct InputAction
	{
		int animChangeIndex = 0;
		Frame::InputCommand inputCommand = Frame::InputCommand::NONE;
	};

	struct FrameAction
	{
		int xDelta;
		int yDelta;
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

protected:
	std::string spriteFileName;
private:
	// texture coordinates
	// texture width and height
};
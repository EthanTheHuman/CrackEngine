#pragma once
#include "../../includes/json.hpp"
#include "../../includes/xml/pugixml.hpp"
#include "../../shader_m.h"
#include "../../stb_image.h"
#include <string>
#include "../Log.h"
using json = nlohmann::json;

// Forward declare anim actions
class AnimAction;
class Act_SetXVel;

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
	static std::vector<Frame::InputAction> readInputActionsFromXML(pugi::xml_node _data);
	std::vector<AnimAction*> animActions;
	std::vector<AnimAction*> readAnimActionsFromXML(pugi::xml_node _xml);
	AnimAction* readAnimActionFromXML(pugi::xml_node _xml);

protected:
	std::string spriteFileName;
};
#include "Animation.h"

Animation::Animation()
{
}

Animation::Animation(json _jsonData)
{
	index = _jsonData["index"];
	json jsonFrames = _jsonData["frames"];
}

Animation::Animation(pugi::xml_node _animData)
{
	frameList = std::map<int, Frame>();
	for (pugi::xml_node frame : _animData.child("frames").children("frame"))
	{
		Frame newFrame(frame);
		frameList.emplace(newFrame.index, newFrame);
	}
	frameList = frameList;
	index = 0;
	if (_animData.attribute("index").as_int() != NULL) index = _animData.attribute("index").as_int();
	if (_animData.attribute("fallback").as_int() != NULL) fallbackindex = _animData.attribute("fallback").as_int();
	for (pugi::xml_node action : _animData.child("inputactions").children("inputaction"))
	{
		int animIndex = -1;
		Frame::InputCommand command = Frame::InputCommand::EMPTY;
		Frame::InputButton button = Frame::InputButton::EMPTY;
		if (action.attribute("animset").as_int() != NULL) animIndex = action.attribute("animset").as_int();
		if (action.attribute("input").as_string() != NULL)
		{
			if ((std::string)(action.attribute("input").as_string()) == (std::string)"FORWARD")
			{
				command = Frame::InputCommand::FORWARD;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"NONE")
			{
				command = Frame::InputCommand::NONE;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"BACK")
			{
				command = Frame::InputCommand::BACK;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"DOWN")
			{
				command = Frame::InputCommand::DOWN;
			}
		}
		if (action.attribute("button").as_string() != NULL)
		{
			if ((std::string)(action.attribute("button").as_string()) == (std::string)"A")
			{
				button = Frame::InputButton::A;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"B")
			{
				button = Frame::InputButton::B;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"C")
			{
				button = Frame::InputButton::C;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"X")
			{
				button = Frame::InputButton::X;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"Y")
			{
				button = Frame::InputButton::Y;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"Z")
			{
				button = Frame::InputButton::Z;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"ANY")
			{
				button = Frame::InputButton::ANY;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"NONE")
			{
				button = Frame::InputButton::NONE;
			}
		}
		Frame::InputAction tempAction;
		tempAction.animChangeIndex = animIndex;
		tempAction.inputCommand = command;
		tempAction.inputButton = button;
		inputActions.push_back(tempAction);
	}
	for (pugi::xml_node action : _animData.child("animactions").children("animaction"))
	{
		int xDelta = 0;
		if (action.attribute("xdelta").as_int() != NULL) xDelta = action.attribute("xdelta").as_int();
		int yDelta = 0;
		if (action.attribute("ydelta").as_int() != NULL) yDelta = action.attribute("ydelta").as_int();
		Frame::FrameAction tempAction;
		tempAction.xDelta = xDelta;
		tempAction.yDelta = yDelta;
		frameActions.push_back(tempAction);
	}
}

Animation::Animation(std::map<int, Frame> _frameList, int _index)
{
	index = _index;
	frameList = _frameList;
}

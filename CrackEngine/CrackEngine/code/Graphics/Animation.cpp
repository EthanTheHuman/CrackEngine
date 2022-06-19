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
	for (pugi::xml_node action : _animData.child("inputactions").children("inputaction"))
	{
		int animIndex = -1;
		Frame::InputCommand command = Frame::InputCommand::EMPTY;
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
		Frame::InputAction tempAction;
		tempAction.animChangeIndex = animIndex;
		tempAction.inputCommand = command;
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

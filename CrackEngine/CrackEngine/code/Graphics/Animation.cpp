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
	inputActions = Frame::readInputActionsFromXML(_animData);
	frameActions = Frame::readFrameActionsFromXML(_animData);
}

Animation::Animation(std::map<int, Frame> _frameList, int _index)
{
	index = _index;
	frameList = _frameList;
}

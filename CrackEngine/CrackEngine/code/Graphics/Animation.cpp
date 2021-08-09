#include "Animation.h"

Animation::Animation()
{
}

Animation::Animation(json _jsonData)
{
	index = _jsonData["index"];
	json jsonFrames = _jsonData["frames"];
}

Animation::Animation(std::map<int, Frame> _frameList, int _index)
{
	index = _index;
	frameList = _frameList;
}

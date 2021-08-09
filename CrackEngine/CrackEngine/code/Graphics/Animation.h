#pragma once
#include "Frame.h"
#include <vector>
#include <map>

class Animation
{
public:
	Animation();
	Animation(json _jsonData);
	Animation(std::map<int, Frame> _frameList, int _index);
	int index;
	std::map<int, Frame> frameList;
};


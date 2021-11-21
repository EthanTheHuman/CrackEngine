#pragma once
#include "Frame.h"
#include <vector>
#include <map>

class Animation
{
public:
	Animation();
	Animation(json _jsonData);
	Animation(pugi::xml_node _animData);
	Animation(std::map<int, Frame> _frameList, int _index);
	int index;
	std::map<int, Frame> frameList;
};


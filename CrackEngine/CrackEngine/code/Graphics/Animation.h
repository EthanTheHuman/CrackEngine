#pragma once
#include "Frame.h"
#include <vector>
#include <map>

using namespace std;

class Animation
{
public:
	Animation();
	Animation(json _jsonData);
	Animation(map<int, Frame> _frameList, int _index);
	int index;
	map<int, Frame> frameList;
};


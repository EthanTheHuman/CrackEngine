#pragma once
#include "Frame.h"
#include <vector>
#include <map>

// Forward declare anim actions
class AnimAction;
class Act_SetXVel;

class Animation
{
public:
	Animation();
	Animation(json _jsonData);
	Animation(pugi::xml_node _animData);
	Animation(std::map<int, Frame> _frameList, int _index);
	~Animation();
	int index;
	std::map<int, Frame> frameList;
	std::vector<Frame::InputAction> inputActions;
	std::vector<AnimAction*> animActions;
	std::vector<AnimAction*> readAnimActionsFromXML(pugi::xml_node _xml);
	AnimAction* readAnimActionFromXML(pugi::xml_node _xml);
	int fallbackindex = 1;
	bool keepVelocity = false;
	bool keepAcceleration = false;
};


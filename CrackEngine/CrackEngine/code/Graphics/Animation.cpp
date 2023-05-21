#include "Animation.h"
// anim actions
#include "../Gameplay/Anim Actions/Act_SetXVel.h"
#include "../Gameplay/Anim Actions/Act_SetYVel.h"
#include "../Gameplay/Anim Actions/Act_AddXVel.h"
#include "../Gameplay/Anim Actions/Act_AddYVel.h"
#include "../Gameplay/Anim Actions/Act_SetXAccel.h"
#include "../Gameplay/Anim Actions/Act_SetYAccel.h"
#include "../Gameplay/Anim Actions/Act_AddXAccel.h"
#include "../Gameplay/Anim Actions/Act_AddYAccel.h"
#include "../Gameplay/Anim Actions/Act_MoveDelta.h"
#include "../Gameplay/Anim Actions/Act_PlaySound.h"

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
	if (_animData.attribute("keepVelocity").as_bool() != NULL) keepVelocity = _animData.attribute("keepVelocity").as_bool();
	if (_animData.attribute("keepAcceleration").as_bool() != NULL) keepAcceleration = _animData.attribute("keepAcceleration").as_bool();
	inputActions = Frame::readInputActionsFromXML(_animData);
	animActions = readAnimActionsFromXML(_animData);
}

Animation::Animation(std::map<int, Frame> _frameList, int _index)
{
	index = _index;
	frameList = _frameList;
}

Animation::~Animation()
{
	// delete everything in the animaction list
	for (AnimAction* action : animActions)
	{
		//delete action;
	}
}

std::vector<AnimAction*> Animation::readAnimActionsFromXML(pugi::xml_node _xml)
{
	std::vector<AnimAction*> returnActions;
	for (pugi::xml_node action : _xml.child("animactions").children("animaction"))
	{
		AnimAction* newAction = readAnimActionFromXML(action);
		if (newAction != nullptr)
		{
			returnActions.push_back(newAction);
		}
	}
	return returnActions;
}

AnimAction* Animation::readAnimActionFromXML(pugi::xml_node _xml)
{
	if (_xml.attribute("type").as_string() != NULL)
	{
		std::string type = _xml.attribute("type").as_string();
		// TODO: All the different animaction types
		if (type == "setxvel"  || type == "setXVel" || type == "SetXVel" || type == "setxVel")
		{
			return new Act_SetXVel(_xml);
		}
		if (type == "setyvel" || type == "setYVel" || type == "SetYVel" || type == "setyVel")
		{
			return new Act_SetYVel(_xml);
		}
		if (type == "addxvel" || type == "addXVel" || type == "AddXVel" || type == "addxVel")
		{
			return new Act_AddXVel(_xml);
		}
		if (type == "addyvel" || type == "addYVel" || type == "AddYVel" || type == "addyVel")
		{
			return new Act_AddYVel(_xml);
		}
		if (type == "setxaccel" || type == "setXAccel" || type == "SetXAccel" || type == "setxAccel")
		{
			return new Act_SetXAccel(_xml);
		}
		if (type == "setyaccel" || type == "setYAccel" || type == "SetYAccel" || type == "setyAccel")
		{
			return new Act_SetYAccel(_xml);
		}
		if (type == "addxaccel" || type == "addXAccel" || type == "AddXAccel" || type == "addxAccel")
		{
			return new Act_AddXAccel(_xml);
		}
		if (type == "addyaccel" || type == "addYAccel" || type == "AddYAccel" || type == "addyAccel")
		{
			return new Act_AddYAccel(_xml);
		}
		if (type == "moveDelta" || type == "MoveDelta" || type == "movedelta")
		{
			return new Act_MoveDelta(_xml);
		}
		if (type == "playSound" || type == "sound" || type == "PlaySound" || type == "playsound" || type == "Sound")
		{
			return new Act_PlaySound(_xml);
		}
	}
	return nullptr;
}

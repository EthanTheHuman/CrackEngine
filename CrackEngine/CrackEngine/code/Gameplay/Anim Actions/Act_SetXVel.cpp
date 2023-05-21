#include "./Act_SetXVel.h"
#include "../../Graphics/AnimManager.h"

Act_SetXVel::Act_SetXVel(pugi::xml_node _xmlData)
{
	init(_xmlData);
	type = "Set X Vel";
}

void Act_SetXVel::execute()
{
	if (frequency == animActionFrequency::always)
	{
		// TODO: Set xVel
	}
}

void Act_SetXVel::execute(int _stepCount)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			// TODO: Set XVel
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			// TODO: Set xVel
		}
		break;
	case animActionFrequency::always:
	default:
		// TODO: Set xVel
		break;
	}
}

void Act_SetXVel::execute(int _stepCount, AnimManager* _manager)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			_manager->setxVelocity(xVel, false);
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			_manager->setxVelocity(xVel, false);
		}
		break;
	case animActionFrequency::always:
	default:
		_manager->setxVelocity(xVel, false);
		break;
	}
}

void Act_SetXVel::init(pugi::xml_node _xmlData)
{
	// get data from xml
	frequency = animActionFrequency::always;
	if (_xmlData.attribute("frequency").as_string() && _xmlData.attribute("frequency").as_string() != "")
	{
		std::string freq = _xmlData.attribute("frequency").as_string();
		frequency = hashFrequency(freq);
	}
	xVel = 0;
	if (_xmlData.attribute("xVel").as_float() && _xmlData.attribute("xVel").as_float() != NULL)
	{
		xVel = _xmlData.attribute("xVel").as_float();
	}
	stepCount = 0;
	if (_xmlData.attribute("stepCount").as_int() && _xmlData.attribute("stepCount").as_int() != NULL)
	{
		stepCount = _xmlData.attribute("stepCount").as_int();
	}
}

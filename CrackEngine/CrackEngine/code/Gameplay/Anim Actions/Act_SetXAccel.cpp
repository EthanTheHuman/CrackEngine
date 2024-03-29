#include "./Act_SetXAccel.h"
#include "../../Graphics/AnimManager.h"

Act_SetXAccel::Act_SetXAccel(pugi::xml_node _xmlData)
{
	init(_xmlData);
	type = "Set X Accel";
}

void Act_SetXAccel::execute()
{
	if (frequency == animActionFrequency::always)
	{
		// TODO: Set xAccel
	}
}

void Act_SetXAccel::execute(int _stepCount)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			// TODO: Set xAccel
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			// TODO: Set xAccel
		}
		break;
	case animActionFrequency::always:
	default:
		// TODO: Set xAccel
		break;
	}
}

void Act_SetXAccel::execute(int _stepCount, AnimManager* _manager)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			_manager->setxAcceleration(xAccel, false);
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			_manager->setxAcceleration(xAccel, false);
		}
		break;
	case animActionFrequency::always:
	default:
		_manager->setxAcceleration(xAccel, false);
		break;
	}
}

void Act_SetXAccel::init(pugi::xml_node _xmlData)
{
	// get data from xml
	frequency = animActionFrequency::always;
	if (_xmlData.attribute("frequency").as_string() && _xmlData.attribute("frequency").as_string() != "")
	{
		std::string freq = _xmlData.attribute("frequency").as_string();
		frequency = hashFrequency(freq);
	}
	xAccel = 0;
	if (_xmlData.attribute("xAccel").as_float() && _xmlData.attribute("xAccel").as_float() != NULL)
	{
		xAccel = _xmlData.attribute("xAccel").as_float();
	}
	stepCount = 0;
	if (_xmlData.attribute("stepCount").as_int() && _xmlData.attribute("stepCount").as_int() != NULL)
	{
		stepCount = _xmlData.attribute("stepCount").as_int();
	}
}

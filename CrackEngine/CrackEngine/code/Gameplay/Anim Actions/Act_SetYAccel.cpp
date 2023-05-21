#include "./Act_SetYAccel.h"
#include "../../Graphics/AnimManager.h"

Act_SetYAccel::Act_SetYAccel(pugi::xml_node _xmlData)
{
	init(_xmlData);
	type = "Set Y Accel";
}

void Act_SetYAccel::execute()
{
	if (frequency == animActionFrequency::always)
	{
		// TODO: Set yAccel
	}
}

void Act_SetYAccel::execute(int _stepCount)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			// TODO: Set yAccel
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			// TODO: Set yAccel
		}
		break;
	case animActionFrequency::always:
	default:
		// TODO: Set yAccel
		break;
	}
}

void Act_SetYAccel::execute(int _stepCount, AnimManager* _manager)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			_manager->setyAcceleration(yAccel, false);
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			_manager->setyAcceleration(yAccel, false);
		}
		break;
	case animActionFrequency::always:
	default:
		_manager->setyAcceleration(yAccel, false);
		break;
	}
}

void Act_SetYAccel::init(pugi::xml_node _xmlData)
{
	// get data from xml
	frequency = animActionFrequency::always;
	if (_xmlData.attribute("frequency").as_string() && _xmlData.attribute("frequency").as_string() != "")
	{
		std::string freq = _xmlData.attribute("frequency").as_string();
		frequency = hashFrequency(freq);
	}
	yAccel = 0;
	if (_xmlData.attribute("yAccel").as_float() && _xmlData.attribute("yAccel").as_float() != NULL)
	{
		yAccel = _xmlData.attribute("yAccel").as_float();
	}
	stepCount = 0;
	if (_xmlData.attribute("stepCount").as_int() && _xmlData.attribute("stepCount").as_int() != NULL)
	{
		stepCount = _xmlData.attribute("stepCount").as_int();
	}
	type = "Set Y Accel; yAccel: " + std::to_string(yAccel) + "; stepCount: " + std::to_string(stepCount);
}

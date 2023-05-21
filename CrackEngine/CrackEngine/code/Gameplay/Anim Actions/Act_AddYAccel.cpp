#include "./Act_AddYAccel.h"
#include "../../Graphics/AnimManager.h"

Act_AddYAccel::Act_AddYAccel(pugi::xml_node _xmlData)
{
	init(_xmlData);
	type = "Add Y Accel";
}

void Act_AddYAccel::execute()
{
	if (frequency == animActionFrequency::always)
	{
		// TODO: Set yAccel
	}
}

void Act_AddYAccel::execute(int _stepCount)
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

void Act_AddYAccel::execute(int _stepCount, AnimManager* _manager)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			_manager->setxAcceleration(yAccel, true);
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			_manager->setxAcceleration(yAccel, true);
		}
		break;
	case animActionFrequency::always:
	default:
		_manager->setxAcceleration(yAccel, true);
		break;
	}
}

void Act_AddYAccel::init(pugi::xml_node _xmlData)
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
}

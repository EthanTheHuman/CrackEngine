#include "Act_AddYVel.h"
#include "../../Graphics/AnimManager.h"


Act_AddYVel::Act_AddYVel(pugi::xml_node _xmlData)
{
	init(_xmlData);
	type = "Add Y Vel";
}

void Act_AddYVel::execute()
{
	if (frequency == animActionFrequency::always)
	{
		// TODO: Set yVel
	}
}

void Act_AddYVel::execute(int _stepCount)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			// TODO: Set yVel
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			// TODO: Set yVel
		}
		break;
	case animActionFrequency::always:
	default:
		// TODO: Set yVel
		break;
	}
}

void Act_AddYVel::execute(int _stepCount, AnimManager* _manager)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			_manager->setyVelocity(yVel, true);
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			_manager->setyVelocity(yVel, true);
		}
		break;
	case animActionFrequency::always:
	default:
		_manager->setyVelocity(yVel, true);
		break;
	}
}

void Act_AddYVel::init(pugi::xml_node _xmlData)
{
	// get data from xml
	frequency = animActionFrequency::always;
	if (_xmlData.attribute("frequency").as_string() && _xmlData.attribute("frequency").as_string() != "")
	{
		std::string freq = _xmlData.attribute("frequency").as_string();
		frequency = hashFrequency(freq);
	}
	yVel = 0;
	if (_xmlData.attribute("yVel").as_float() && _xmlData.attribute("yVel").as_float() != NULL)
	{
		yVel = _xmlData.attribute("yVel").as_float();
	}
	stepCount = 0;
	if (_xmlData.attribute("stepCount").as_int() && _xmlData.attribute("stepCount").as_int() != NULL)
	{
		stepCount = _xmlData.attribute("stepCount").as_int();
	}
}

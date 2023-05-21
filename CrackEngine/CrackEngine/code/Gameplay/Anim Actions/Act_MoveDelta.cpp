#include "./Act_MoveDelta.h"
#include "../../Graphics/AnimManager.h"

Act_MoveDelta::Act_MoveDelta(pugi::xml_node _xmlData)
{
	init(_xmlData);
	type = "Move Delta";
}

void Act_MoveDelta::execute()
{
	if (frequency == animActionFrequency::always)
	{
		// TODO: Set yAccel
	}
}

void Act_MoveDelta::execute(int _stepCount)
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

void Act_MoveDelta::execute(int _stepCount, AnimManager* _manager)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			_manager->setPosition(glm::vec3(xDelta, yDelta, 0), true);
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			_manager->setPosition(glm::vec3(xDelta, yDelta, 0), true);
		}
		break;
	case animActionFrequency::always:
	default:
		_manager->setPosition(glm::vec3(xDelta, yDelta, 0), true);
	}
}

void Act_MoveDelta::init(pugi::xml_node _xmlData)
{
	// get data from xml
	frequency = animActionFrequency::always;
	if (_xmlData.attribute("frequency").as_string() && _xmlData.attribute("frequency").as_string() != "")
	{
		std::string freq = _xmlData.attribute("frequency").as_string();
		frequency = hashFrequency(freq);
	}
	xDelta = 0;
	if (_xmlData.attribute("xDelta").as_float() && _xmlData.attribute("xDelta").as_float() != NULL)
	{
		xDelta = _xmlData.attribute("xDelta").as_float();
	}
	yDelta = 0;
	if (_xmlData.attribute("yDelta").as_float() && _xmlData.attribute("yDelta").as_float() != NULL)
	{
		yDelta = _xmlData.attribute("yDelta").as_float();
	}
	stepCount = 0;
	if (_xmlData.attribute("stepCount").as_int() && _xmlData.attribute("stepCount").as_int() != NULL)
	{
		stepCount = _xmlData.attribute("stepCount").as_int();
	}
}

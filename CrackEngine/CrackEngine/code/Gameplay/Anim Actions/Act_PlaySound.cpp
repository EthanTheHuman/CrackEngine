#include "./Act_PlaySound.h"
#include "../../Graphics/AnimManager.h"

Act_PlaySound::Act_PlaySound(pugi::xml_node _xmlData) : sound(nullptr)
{
	delete sound;
	init(_xmlData);
	type = "Play Sound";
}

void Act_PlaySound::execute()
{
	if (frequency == animActionFrequency::always)
	{
		// TODO: Play Sound
	}
}

void Act_PlaySound::execute(int _stepCount)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			// TODO: Play Sound
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			// TODO: Play Sound
		}
		break;
	case animActionFrequency::always:
	default:
		// TODO: Play Sound
		break;
	}
}

void Act_PlaySound::execute(int _stepCount, AnimManager* _manager)
{
	switch (frequency) {
	case animActionFrequency::once:
		if (_stepCount == stepCount)
		{
			sound->play();
		}
		break;
	case animActionFrequency::perFrameChange:
		if (_stepCount == 0)
		{
			sound->play();
		}
		break;
	case animActionFrequency::always:
	default:
		if (_stepCount == 0)
		{
			sound->play();
		}
		break;
	}
}

void Act_PlaySound::init(pugi::xml_node _xmlData)
{
	// get data from xml
	frequency = animActionFrequency::always;
	if (_xmlData.attribute("frequency").as_string() && _xmlData.attribute("frequency").as_string() != "")
	{
		std::string freq = _xmlData.attribute("frequency").as_string();
		frequency = hashFrequency(freq);
	}
	filename = "";
	if (_xmlData.attribute("filename").as_string() && _xmlData.attribute("filename").as_string() != NULL)
	{
		filename = _xmlData.attribute("filename").as_string();
	}
	volume = 1.0f;
	if (_xmlData.attribute("volume").as_float() && _xmlData.attribute("volume").as_float() != NULL)
	{
		volume = _xmlData.attribute("volume").as_float();
	}
	stepCount = 0;
	if (_xmlData.attribute("stepCount").as_int() && _xmlData.attribute("stepCount").as_int() != NULL)
	{
		stepCount = _xmlData.attribute("stepCount").as_int();
	}
	sound = new AudioPlayer(filename, false, volume);
}

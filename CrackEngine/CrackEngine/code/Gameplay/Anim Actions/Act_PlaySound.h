#pragma once
#include "../AnimAction.h"
#include <string>
#include "../../sound/AudioPlayer.h"

class AnimAction;

class Act_PlaySound : public AnimAction
{
public:
	Act_PlaySound(pugi::xml_node _xmlData);
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
protected:
	void init(pugi::xml_node _xmlData) override;

	std::string filename;
	float volume;
	int stepCount;
	animActionFrequency frequency;
	AudioPlayer* sound;
};


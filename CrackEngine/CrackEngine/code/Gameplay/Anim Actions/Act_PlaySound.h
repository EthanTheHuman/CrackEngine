#pragma once
#include "../AnimAction.h"
#include <string>
#include "../../sound/AudioPlayer.h"

class AnimAction;

class Act_PlaySound : public AnimAction
{
public:
	Act_PlaySound(pugi::xml_node _xmlData);
	Act_PlaySound() : volume(1.0f), stepCount(0), sound(nullptr), frequency(animActionFrequency::always) { type = "PlaySound"; }
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
	std::string getFilename() const { return filename; }
	float getVolume() const { return volume; }
	std::string filename;
	float volume;
	int stepCount;
	animActionFrequency frequency;
	AudioPlayer* sound;
protected:
	void init(pugi::xml_node _xmlData) override;
};


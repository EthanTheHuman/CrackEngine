#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_SetXVel : public AnimAction
{
public:
	Act_SetXVel(pugi::xml_node _xmlData);
	Act_SetXVel() : xVel(0), stepCount(0), frequency(animActionFrequency::always) { type = "SetXVel"; }
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
	float getXVel() const { return xVel; }
	float xVel;
	int stepCount;
	animActionFrequency frequency;
protected:
	void init(pugi::xml_node _xmlData) override;
};


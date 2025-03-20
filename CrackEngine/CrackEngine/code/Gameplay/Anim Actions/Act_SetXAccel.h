#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_SetXAccel : public AnimAction
{
public:
	Act_SetXAccel(pugi::xml_node _xmlData);
	Act_SetXAccel() : xAccel(0), stepCount(0), frequency(animActionFrequency::always) { type = "SetXAccel"; }
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
	float getXAccel() const { return xAccel; }
	float xAccel;
	int stepCount;
	animActionFrequency frequency;
protected:
	void init(pugi::xml_node _xmlData) override;
};


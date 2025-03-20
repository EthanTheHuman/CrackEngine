#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_AddXAccel : public AnimAction
{
public:
	Act_AddXAccel(pugi::xml_node _xmlData);
	Act_AddXAccel() : xAccel(0), stepCount(0), frequency(animActionFrequency::always) { type = "AddXAccel"; }
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


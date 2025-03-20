#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_AddYAccel : public AnimAction
{
public:
	Act_AddYAccel(pugi::xml_node _xmlData);
	Act_AddYAccel() : yAccel(0), stepCount(0), frequency(animActionFrequency::always) { type = "AddYAccel"; }
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
	float getYAccel() const { return yAccel; }
	float yAccel;
	int stepCount;
	animActionFrequency frequency;
protected:
	void init(pugi::xml_node _xmlData) override;
};


#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_SetYAccel : public AnimAction
{
public:
	Act_SetYAccel(pugi::xml_node _xmlData);
	Act_SetYAccel() : yAccel(0), stepCount(0), frequency(animActionFrequency::always) { type = "SetYAccel"; }
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


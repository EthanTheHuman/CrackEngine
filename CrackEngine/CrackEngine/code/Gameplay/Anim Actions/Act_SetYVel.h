#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_SetYVel : public AnimAction
{
public:
	Act_SetYVel(pugi::xml_node _xmlData);
	Act_SetYVel() : yVel(0), stepCount(0), frequency(animActionFrequency::always) { type = "SetYVel"; }
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
	float getYVel() const { return yVel; }
	float yVel;
	int stepCount;
	animActionFrequency frequency;
protected:
	void init(pugi::xml_node _xmlData) override;
};


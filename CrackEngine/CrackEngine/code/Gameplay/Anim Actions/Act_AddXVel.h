#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_AddXVel : public AnimAction
{
public:
	Act_AddXVel(pugi::xml_node _xmlData);
	Act_AddXVel() : xVel(0), stepCount(0), frequency(animActionFrequency::always) { type = "AddXVel"; }
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


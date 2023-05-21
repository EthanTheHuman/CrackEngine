#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_SetYVel : public AnimAction
{
public:
	Act_SetYVel(pugi::xml_node _xmlData);
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
protected:
	void init(pugi::xml_node _xmlData) override;
	float yVel;
	int stepCount;
	animActionFrequency frequency;
};


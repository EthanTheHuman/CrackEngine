#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_AddXAccel : public AnimAction
{
public:
	Act_AddXAccel(pugi::xml_node _xmlData);
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
protected:
	void init(pugi::xml_node _xmlData) override;

	float xAccel;
	int stepCount;
	animActionFrequency frequency;
};


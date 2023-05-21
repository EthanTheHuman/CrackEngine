#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_MoveDelta : public AnimAction
{
public:
	Act_MoveDelta(pugi::xml_node _xmlData);
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
protected:
	void init(pugi::xml_node _xmlData) override;
	float xDelta;
	float yDelta;
	int stepCount;
	animActionFrequency frequency;
};


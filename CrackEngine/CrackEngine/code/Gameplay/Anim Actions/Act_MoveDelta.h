#pragma once
#include "../AnimAction.h"

class AnimAction;

class Act_MoveDelta : public AnimAction
{
public:
	Act_MoveDelta(pugi::xml_node _xmlData);
	Act_MoveDelta() : xDelta(0), yDelta(0), stepCount(0), frequency(animActionFrequency::always) { type = "MoveDelta"; }
	void execute() override;
	void execute(int _stepCount) override;
	void execute(int _stepCount, AnimManager* _manager) override;
	float getXDelta() const { return xDelta; }
	float getYDelta() const { return yDelta; }
	float xDelta;
	float yDelta;
	int stepCount;
	animActionFrequency frequency;
protected:
	void init(pugi::xml_node _xmlData) override;
};


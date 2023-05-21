#include "./AnimAction.h"

AnimAction::animActionFrequency AnimAction::hashFrequency(std::string _input)
{
	if (_input == "always" || _input == "Always")
	{
		return animActionFrequency::always;
	}
	if (_input == "once" || _input == "Once")
	{
		return animActionFrequency::once;
	}
	if (_input == "perframechange" || _input == "perFrameChange" || _input == "PerFrameChange")
	{
		return animActionFrequency::perFrameChange;
	}
	return animActionFrequency();
}

AnimAction::AnimAction()
{
}

AnimAction::AnimAction(pugi::xml_node _xmlData)
{
	init(_xmlData);
	type = "Blank AnimAction";
}

void AnimAction::execute()
{
}

void AnimAction::execute(int _stepCount)
{
}

void AnimAction::execute(int _stepCount, AnimManager* _manager)
{
}

void AnimAction::init(pugi::xml_node _xmlData)
{
}

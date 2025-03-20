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

void AnimAction::loadFromXml(pugi::xml_node& node) {
	// Load common attributes
	if (node.attribute("frequency").as_string() && node.attribute("frequency").as_string() != "") {
		frequency = hashFrequency(node.attribute("frequency").as_string());
	}
	if (node.attribute("stepCount").as_int() && node.attribute("stepCount").as_int() != NULL) {
		stepCount = node.attribute("stepCount").as_int();
	}
	if (node.attribute("type").as_string() && node.attribute("type").as_string() != "") {
		type = node.attribute("type").as_string();
	}
}

void AnimAction::saveToXml(pugi::xml_node& node) {
	// Save common attributes
	node.append_attribute("type") = type.c_str();
	
	// Save frequency string directly
	node.append_attribute("frequency") = frequency.c_str();
	node.append_attribute("stepCount") = stepCount;
}

#pragma once
#include "../../includes/xml/pugixml.hpp"
#include <map>
#include <string>

// forward declare AnimManager class
class AnimManager;

class AnimAction	// virtual class for animation actions
{
public:
	enum animActionFrequency {
		always,
		once,
		perFrameChange
	};
	animActionFrequency hashFrequency(std::string _input);
	AnimAction();
	AnimAction(pugi::xml_node _xmlData);
	virtual void execute();
	virtual void execute(int _stepCount);
	virtual void execute(int _stepCount, AnimManager* _manager);
	std::string type = "";
	int stepCount = 0;
	std::string frequency = "always";

	virtual ~AnimAction() = default;
	virtual void saveToXml(pugi::xml_node& node);
	virtual void loadFromXml(pugi::xml_node& node);
protected:
	virtual void init(pugi::xml_node _xmlData);
};


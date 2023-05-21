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
protected:
	virtual void init(pugi::xml_node _xmlData);
};


#pragma once
#include "../../includes/xml/pugixml.hpp"

class AnimAction	// virtual class for animation actions
{
public:
	AnimAction();
	virtual void execute();
protected:
	virtual void init(pugi::xml_node _xmlData);
};


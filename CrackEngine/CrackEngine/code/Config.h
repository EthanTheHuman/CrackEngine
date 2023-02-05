#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include "../includes/json.hpp"

class Config
{
public:
	static void Init();

	// Config.json
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;
	static bool vsync;
	static bool fullscreen;
	static bool resizableWindow;
	static unsigned int targetFPS;

	// ProjectSettings.json
	static std::string windowName;
	static unsigned int targetFramestep;
	static unsigned int pixelsPerUnit;
	static unsigned int internalHeight;
};


#include "Config.h"

using json = nlohmann::json;

unsigned int Config::SCR_WIDTH = 1536;
unsigned int Config::SCR_HEIGHT = 896;
bool Config::vsync = false;
bool Config::fullscreen = false;
bool Config::resizableWindow = true;
unsigned int Config::targetFPS = 0;

std::string Config::windowName = "Ethan's Engine";
unsigned int Config::targetFramestep = 60;
unsigned int Config::pixelsPerUnit = 384;

void Config::Init()
{
	// Read config.json
	std::ifstream configfile ("config.json", std::ios::out | std::ios::app | std::ios::binary);
	if (configfile.is_open())
	{
		// Proceed with output
		json config;
		configfile >> config;

		std::cout << "config:" << std::endl;
		std::cout << config.dump() << std::endl;
		SCR_WIDTH = config["screenWidth"];
		SCR_HEIGHT = config["screenHeight"];
		vsync = config["vsync"];
		fullscreen = config["fullscreen"];
		resizableWindow = config["resizableWindow"];
		targetFPS = config["targetFPS"];
		//cout << "json pi: " << config["pi"] << endl;
	}
	else
	{
		// Error opening file
		std::cout << "Unable to open file" << std::endl;
	}
	configfile.close();

	// Read config.json
	std::ifstream projectsettingsfile("data/config/ProjectSettings.json", std::ios::out | std::ios::app | std::ios::binary);
	if (projectsettingsfile.is_open())
	{
		// Proceed with output
		json config;
		projectsettingsfile >> config;

		std::cout << "project settings:" << std::endl;
		std::cout << config.dump() << std::endl;
		windowName = config["windowName"];
		targetFramestep = config["targetFramestep"];
		pixelsPerUnit = config["pixelsPerUnit"];
	}
	else
	{
		// Error opening file
		std::cout << "Unable to open file" << std::endl;
	}
	projectsettingsfile.close();

	return;
}
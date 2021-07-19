#include "Config.h"

using namespace std;
using json = nlohmann::json;

unsigned int Config::SCR_WIDTH = 1536;
unsigned int Config::SCR_HEIGHT = 896;
bool Config::vsync = false;
bool Config::fullscreen = false;
bool Config::resizableWindow = true;
unsigned int Config::targetFPS = 0;

std::string Config::windowName = "Ethan's Engine";
unsigned int Config::targetFramestep = 60;

void Config::Init()
{
	// Read config.json
	ifstream configfile ("config.json", ios::out | ios::app | ios::binary);
	if (configfile.is_open())
	{
		// Proceed with output
		json config;
		configfile >> config;

		cout << "config:" << endl;
		cout << config.dump() << endl;
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
		cout << "Unable to open file" << endl;
	}
	configfile.close();

	// Read config.json
	ifstream projectsettingsfile("data/config/ProjectSettings.json", ios::out | ios::app | ios::binary);
	if (projectsettingsfile.is_open())
	{
		// Proceed with output
		json config;
		projectsettingsfile >> config;

		cout << "project settings:" << endl;
		cout << config.dump() << endl;
		windowName = config["windowName"];
		targetFramestep = config["targetFramestep"];
	}
	else
	{
		// Error opening file
		cout << "Unable to open file" << endl;
	}
	projectsettingsfile.close();

	return;
}
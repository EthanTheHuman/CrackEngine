#include "Config.h"
#include <fstream>
#include <string>
#include <iostream>
#include "../includes/json.hpp"

using namespace std;
using json = nlohmann::json;

unsigned int Config::SCR_WIDTH = 1536;
unsigned int Config::SCR_HEIGHT = 896;
bool Config::vsync = false;
bool Config::fullscreen = false;
bool Config::resizableWindow = true;
unsigned int Config::targetFPS = 0;

void Config::Init()
{
	ifstream myfile ("config.json", ios::out | ios::app | ios::binary);
	if (myfile.is_open())
	{
		// Proceed with output
		json config;
		myfile >> config;

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
	myfile.close();
	return;
}
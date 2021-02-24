#pragma once
#include "Includes.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>  
#include <iomanip>

using namespace std;

class Log
{
public:  
	enum logMode { LOG = 0, WARNING, ERROR, IMPORTANT};
	static logMode filter;
	static void log(std::string _input, logMode _mode = logMode::LOG);
	static void log(std::string _input);
	static void warn(std::string _input);
	static void error(std::string _input) { log(_input, logMode::ERROR); };
	static void important(std::string _input) { log(_input, logMode::IMPORTANT); };
};


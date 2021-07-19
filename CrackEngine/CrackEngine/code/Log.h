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
};


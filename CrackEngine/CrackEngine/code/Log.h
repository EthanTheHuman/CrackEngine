#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>  
#include <iomanip>

class Log
{
public:  
	enum logMode { LOG = 0, WARNING, ANERROR, IMPORTANT};
	static logMode filter;
	static void log(std::string _input, logMode _mode = logMode::LOG);
};


#include "Log.h"
using namespace std;

Log::logMode Log::filter = Log::logMode::LOG;

void Log::log(std::string _input, logMode _mode)
{
	if (_mode >= filter)
	{
		time_t tt = time(NULL);

		switch (_mode)
		{
		case logMode::LOG:
			{
				cout << "LOG: " <<_input << endl;
				break;
			}
		case logMode::WARNING:
			{
				cout << "WARNING: " << _input << endl;
				break;
			}
		case logMode::ERROR:
			{
				cout << "ERROR: " << _input << endl;
				break;
			}
		case logMode::IMPORTANT:
			{
				cout << "IMPORTANT: " << _input << endl;
				break;
			}
		}
	}
}

void Log::log(std::string _input)
{
	log(_input, logMode::LOG);
}

void Log::warn(std::string _input)
{
	log(_input, logMode::WARNING);
}

void Log::error(std::string _input)
{
	log(_input, logMode::ERROR);
}

void Log::important(std::string _input)
{
	log(_input, logMode::IMPORTANT);
}

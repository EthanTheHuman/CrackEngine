#include "Log.h"

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
				std::cout << "LOG: " <<_input << std::endl;
				break;
			}
		case logMode::WARNING:
			{
				std::cout << "WARNING: " << _input << std::endl;
				break;
			}
		case logMode::ANERROR:
			{
				std::cout << "ERROR: " << _input << std::endl;
				break;
			}
		case logMode::IMPORTANT:
			{
				std::cout << "IMPORTANT: " << _input << std::endl;
				break;
			}
		}
	}
}


#pragma once
#include <string>

class Logger
{

private:
	static Logger* inst;

public:
	static Logger* Instance() { return inst; }

public:
	Logger();
	~Logger();

	static void PrintLog(const WCHAR* fmt, ...); // multiple WCHAR* arguments
	static std::wstring LogDirectory();
	static std::wstring LogFile();

	/* Print a line of '-' char's */
	static void PrintDebugSeperator();

	/* Check to see if MTail is already running */
	static BOOL IsMTailRunning();

	/* Start MTail Application */
	static void StartMTail(); // MTail is an app that runs alongside the project

};

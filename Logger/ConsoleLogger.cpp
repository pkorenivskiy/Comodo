#include "stdafx.h"

#include <stdio.h>

#include "ConsoleLogger.h"



CConsoleLogger::CConsoleLogger()
{
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}


CConsoleLogger::~CConsoleLogger()
{
}

void CConsoleLogger::Debug(const wchar_t * lpwszMessage, bool isComplete)
{
	log(LogLevel::Debug, lpwszMessage, isComplete);
}

void CConsoleLogger::Info(const wchar_t * lpwszMessage, bool isComplete)
{
	log(LogLevel::Info, lpwszMessage, isComplete);
}

void CConsoleLogger::Warn(const wchar_t * lpwszMessage, bool isComplete)
{
	log(LogLevel::Warn, lpwszMessage, isComplete);
}

void CConsoleLogger::Error(const wchar_t * lpwszMessage, bool isComplete)
{
	log(LogLevel::Error, lpwszMessage, isComplete);
}

void CConsoleLogger::log(LogLevel nLevel, const wchar_t * lpwszMessage, bool isComplete)
{
	WORD nColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	switch (nLevel)
	{
	case LogLevel::Info: nColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case LogLevel::Warn: nColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case LogLevel::Error: nColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	default:
		break;
	}

	SetConsoleTextAttribute(m_hConsole, nColor);
		
	if (isComplete)
		wprintf(L"%s\n", lpwszMessage);
	else
		wprintf(lpwszMessage);
}

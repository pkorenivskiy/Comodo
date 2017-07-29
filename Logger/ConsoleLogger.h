#pragma once
#include "Logger.h"

class CConsoleLogger :
	public CLogger
{
public:
	CConsoleLogger();
	~CConsoleLogger();

	// Inherited via CLogger
	virtual void Debug(const wchar_t * lpwszMessage, bool isComplete = true) override;
	virtual void Info(const wchar_t * lpwszMessage, bool isComplete = true) override;
	virtual void Warn(const wchar_t * lpwszMessage, bool isComplete = true) override;
	virtual void Error(const wchar_t * lpwszMessage, bool isComplete = true) override;

private:
	HANDLE m_hConsole;

private:
	void log(LogLevel nLevel, const wchar_t* lpwszMessage, bool isComplete = true);
};


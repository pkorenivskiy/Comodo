#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LOGGER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LOGGER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LOGGER_EXPORTS
#define LOGGER_API __declspec(dllexport)
#else
#define LOGGER_API __declspec(dllimport)
#endif

enum LogLevel
{
	Debug,
	Info,
	Warn,
	Error
};

// This class is exported from the Logger.dll
class CLogger
{
public:
	virtual void Debug(const wchar_t* lpwszMessage, bool isComplete = true) = 0;
	virtual void Info(const wchar_t* lpwszMessage, bool isComplete = true) = 0;
	virtual void Warn(const wchar_t* lpwszMessage, bool isComplete = true) = 0;
	virtual void Error(const wchar_t* lpwszMessage, bool isComplete = true) = 0;
};

LOGGER_API CAutoPtr<CLogger> CreateConsoleLogger();

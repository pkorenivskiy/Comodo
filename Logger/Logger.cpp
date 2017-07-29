// Logger.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Logger.h"
#include "ConsoleLogger.h"

LOGGER_API CAutoPtr<CLogger> CreateConsoleLogger()
{
	return CAutoPtr<CLogger>(new CConsoleLogger());
}


// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "scanlib.h"

void unloadSignature();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		unloadSignature();
		break;
	}
	return TRUE;
}

void unloadSignature()
{
	for (int i = 0; i < scandll::g_Malwares.GetCount(); ++i)
	{
		delete[] scandll::g_Malwares[i].HexData;
	}
}
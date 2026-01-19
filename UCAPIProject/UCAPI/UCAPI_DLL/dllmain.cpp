// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "ucapi_config.h"
#include "ucapi_logger.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ucapi::Logger::Initialize();
        ucapi::Config::Initialize();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        ucapi::Config::Shutdown();
        ucapi::Logger::Shutdown();
        break;
    }
    return TRUE;
}


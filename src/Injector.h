#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>


namespace Injector{

    static DWORD ID = -1;
    static HANDLE hProcessHandle;

    struct Process{
        DWORD ID;
        wchar_t Name[MAX_PATH];

        Process(DWORD id,const wchar_t* name)
       {
            ID = id;

            wcscpy_s(Name,name);
        };

    };



    std::vector<Process> Win32ReturnRunningProcceses();

    BOOL Win32InjectDLLToProcess(const std::wstring& myDllPath);

    BOOL Win32OpenProcess(DWORD pID);

}


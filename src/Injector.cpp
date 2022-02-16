#include "Injector.h"

BOOL Injector::Win32InjectDLLToProcess(const std::wstring& myDllPath) {

    DWORD fullPathLength;
    DWORD fullPathBufferSize;
    TCHAR fullPathBuffer[MAX_PATH] = { 0 };
    fullPathLength = GetFullPathNameW(myDllPath.c_str(), MAX_PATH, fullPathBuffer, NULL) + 1;
    fullPathBufferSize = fullPathLength * sizeof(TCHAR);

    HMODULE hKernelHandle = GetModuleHandleA("kernel32.dll");
    if (hKernelHandle == NULL) {
        CloseHandle(hProcessHandle);
        throw std::exception("Could not load library\n");
    }


    LPVOID lpLoadLib = reinterpret_cast<LPVOID>(GetProcAddress(hKernelHandle, "LoadLibraryW"));
    if (lpLoadLib == NULL) {
        CloseHandle(hProcessHandle);
        CloseHandle(hKernelHandle);
        throw std::exception("Could not load function\n");
    }


    LPVOID lpLoadLocation = VirtualAllocEx(hProcessHandle, NULL, fullPathBufferSize,
                    MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (lpLoadLocation == NULL) {
        CloseHandle(hProcessHandle);
        CloseHandle(hKernelHandle);
        throw std::exception("Could not allocate\n");
    }


    if (!WriteProcessMemory(hProcessHandle, lpLoadLocation, fullPathBuffer,
        fullPathBufferSize, NULL)) {
        CloseHandle(hProcessHandle);
        CloseHandle(hKernelHandle);
        throw std::exception("Could not write memory\n");
    }


    HANDLE hRemoteThread = CreateRemoteThread(hProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)lpLoadLib,
                                lpLoadLocation,0,NULL);
    if (hRemoteThread == NULL) {

        CloseHandle(hProcessHandle);
        CloseHandle(hKernelHandle);
        throw std::exception("Could not create thread\n");
    }

    WaitForSingleObject(hRemoteThread, INFINITE);


    CloseHandle(hRemoteThread);
    VirtualFreeEx(hProcessHandle, lpLoadLocation, 0, MEM_RELEASE);
    CloseHandle(hProcessHandle);
    CloseHandle(hKernelHandle);
    return TRUE;
}

BOOL Injector::Win32OpenProcess(DWORD pID)
{
    CloseHandle(hProcessHandle);
    hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (hProcessHandle == NULL) {
        throw std::exception("Could not open process\n");
    }
    return TRUE;

}


std::vector<Injector::Process> Injector::Win32ReturnRunningProcceses() {

    std::vector<Injector::Process> myVector;
    HANDLE hToolHelper = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0x0);
    PROCESSENTRY32 p32ProcessEntry = { 0 };
    p32ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

    if (hToolHelper == NULL) {
        throw std::exception("Couldn't take snapchot of running processes\n");
    }

    if (!Process32First(hToolHelper, &p32ProcessEntry)) {
        CloseHandle(hToolHelper);
        throw std::exception("Unexpected error\n");
    }

    do {
        myVector.emplace_back(Injector::Process(p32ProcessEntry.th32ProcessID,p32ProcessEntry.szExeFile));
    } while (Process32Next(hToolHelper, &p32ProcessEntry));

    CloseHandle(hToolHelper);
    return myVector;
}

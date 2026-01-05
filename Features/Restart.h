#pragma once
#include <Windows.h>

namespace GUI
{
    inline void FullRestart()
    {
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        STARTUPINFOA si = { 0 };
        PROCESS_INFORMATION pi = { 0 };
        si.cb = sizeof(si);

        if (CreateProcessA(
            exePath,
            NULL,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
        {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        ExitProcess(0);
    }
}

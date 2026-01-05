#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <winternl.h>
#include "AntiDetection.h"
// #include "SafeMemory.h" // Removed to fix crashes
// #include "VACBypass.h" // Removed to fix crash
#include <thread>
#include <chrono>

// NtReadVirtualMemory for lower-level access
typedef NTSTATUS(NTAPI* pNtReadVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    SIZE_T NumberOfBytesToRead,
    PSIZE_T NumberOfBytesReaded
    );

typedef NTSTATUS(NTAPI* pNtWriteVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    SIZE_T NumberOfBytesToWrite,
    PSIZE_T NumberOfBytesWritten
    );

class MemoryMgr
{
public:
    MemoryMgr();
    ~MemoryMgr();
    bool ConnectDriver(const LPCWSTR name); // Kept for compatibility, now just returns true
    bool DisconnectDriver();
    bool Attach(const DWORD pid);
    DWORD64 GetModuleBase(const wchar_t* moduleName);
    DWORD GetProcessID(const wchar_t* processName);
    bool BatchReadMemory(const std::vector<std::pair<DWORD64, SIZE_T>>& requests, void* output_buffer);
    
    // Enable debug privileges for memory access
    static bool EnableSeDebugPrivilege();

    template <typename ReadType>
    bool ReadMemory(DWORD64 address, ReadType& value, SIZE_T size = sizeof(ReadType))
    {
        if (m_hProcess == nullptr || m_hProcess == INVALID_HANDLE_VALUE || ProcessID == 0)
            return false;

        if (address == 0 || address >= 0x7FFFFFFFFFFF || size == 0 || size > 0x1000)
            return false;
            
        // Add anti-detection delay
        AntiDetection::RandomDelay();

        if (address + size < address)
            return false;

        SIZE_T bytesRead = 0;
        
        // Try ReadProcessMemory first
        BOOL result = ReadProcessMemory(m_hProcess, 
            reinterpret_cast<LPCVOID>(address), 
            &value, 
            size, 
            &bytesRead);
        
        if (result && bytesRead == size)
            return true;

        // If RPM fails, try NtReadVirtualMemory (lower level)
        static pNtReadVirtualMemory NtRVM = nullptr;
        if (NtRVM == nullptr)
        {
            HMODULE ntdll = GetModuleHandleA("ntdll.dll");
            if (ntdll)
                NtRVM = (pNtReadVirtualMemory)GetProcAddress(ntdll, "NtReadVirtualMemory");
        }

        if (NtRVM)
        {
            NTSTATUS status = NtRVM(m_hProcess, 
                reinterpret_cast<PVOID>(address), 
                &value, 
                size, 
                &bytesRead);
            
            return (status == 0 && bytesRead == size);
        }

        return false;
    }

    template<typename T>
    bool BatchReadStructured(const std::vector<DWORD64>& addresses, std::vector<T>& results) {
        if (addresses.empty()) return false;

        std::vector<std::pair<DWORD64, SIZE_T>> requests;
        requests.reserve(addresses.size());

        for (DWORD64 addr : addresses) {
            requests.emplace_back(addr, sizeof(T));
        }

        results.resize(addresses.size());
        return BatchReadMemory(requests, results.data());
    }

    template <typename T>
    bool WriteMemory(DWORD64 address, const T& value, SIZE_T size = sizeof(T))
    {
        if (m_hProcess == nullptr || m_hProcess == INVALID_HANDLE_VALUE || ProcessID == 0)
            return false;

        if (address == 0 || address >= 0x7FFFFFFFFFFF || size == 0 || size > 0x1000)
            return false;

        SIZE_T bytesWritten = 0;
        
        // Try WriteProcessMemory first
        BOOL result = WriteProcessMemory(m_hProcess, 
            reinterpret_cast<LPVOID>(address), 
            &value, 
            size, 
            &bytesWritten);
        
        if (result && bytesWritten == size)
            return true;

        // If WPM fails, try NtWriteVirtualMemory (lower level)
        static pNtWriteVirtualMemory NtWVM = nullptr;
        if (NtWVM == nullptr)
        {
            HMODULE ntdll = GetModuleHandleA("ntdll.dll");
            if (ntdll)
                NtWVM = (pNtWriteVirtualMemory)GetProcAddress(ntdll, "NtWriteVirtualMemory");
        }

        if (NtWVM)
        {
            NTSTATUS status = NtWVM(m_hProcess, 
                reinterpret_cast<PVOID>(address), 
                const_cast<T*>(&value), 
                size, 
                &bytesWritten);
            
            return (status == 0 && bytesWritten == size);
        }

        return false;
    }

public:
    DWORD ProcessID = 0;
    HANDLE m_hProcess = nullptr;

private:
    MODULEENTRY32W GetModule(const wchar_t* moduleName);
    PROCESSENTRY32W GetProcess(const wchar_t* processName);
};

inline MemoryMgr memoryManager;
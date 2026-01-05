#include "MemoryMgr.h"

MemoryMgr::MemoryMgr()
{
    ProcessID = 0;
    m_hProcess = nullptr;
}

MemoryMgr::~MemoryMgr()
{
    DisconnectDriver();
    ProcessID = 0;
    m_hProcess = nullptr;
}

bool MemoryMgr::EnableSeDebugPrivilege()
{
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return false;

    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
    {
        CloseHandle(hToken);
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    bool result = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL) != FALSE;
    
    CloseHandle(hToken);
    return result;
}

bool MemoryMgr::ConnectDriver(const LPCWSTR name)
{
    // Enable SeDebugPrivilege for memory access
    EnableSeDebugPrivilege();
    return true;
}

bool MemoryMgr::DisconnectDriver()
{
    if (m_hProcess != nullptr)
    {
        BOOL result = CloseHandle(m_hProcess);
        m_hProcess = nullptr;
        return result == TRUE;
    }
    return false;
}

bool MemoryMgr::Attach(const DWORD pid)
{
    if (pid == 0)
        return false;

    // Close existing handle if any
    if (m_hProcess != nullptr)
    {
        CloseHandle(m_hProcess);
        m_hProcess = nullptr;
    }

    // Try PROCESS_ALL_ACCESS first
    m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    // If failed, try with specific permissions (for protected processes)
    if (m_hProcess == nullptr || m_hProcess == INVALID_HANDLE_VALUE)
    {
        DWORD access = PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | 
                       PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION;
        m_hProcess = OpenProcess(access, FALSE, pid);
    }

    if (m_hProcess != nullptr && m_hProcess != INVALID_HANDLE_VALUE)
    {
        ProcessID = pid;
        return true;
    }

    m_hProcess = nullptr;
    return false;
}

PROCESSENTRY32W MemoryMgr::GetProcess(const wchar_t* processName)
{
    PROCESSENTRY32W entry = {};
    entry.dwSize = sizeof(PROCESSENTRY32W);
    
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return entry;

    if (Process32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(entry.szExeFile, processName) == 0)
            {
                CloseHandle(snapshot);
                return entry;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    entry.th32ProcessID = 0;
    return entry;
}

DWORD MemoryMgr::GetProcessID(const wchar_t* processName)
{
    PROCESSENTRY32W process = GetProcess(processName);
    return process.th32ProcessID;
}

MODULEENTRY32W MemoryMgr::GetModule(const wchar_t* moduleName)
{
    MODULEENTRY32W entry = {};
    entry.dwSize = sizeof(MODULEENTRY32W);

    if (ProcessID == 0)
        return entry;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
    if (snapshot == INVALID_HANDLE_VALUE)
        return entry;

    if (Module32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(entry.szModule, moduleName) == 0)
            {
                CloseHandle(snapshot);
                return entry;
            }
        } while (Module32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    entry.modBaseAddr = nullptr;
    return entry;
}

DWORD64 MemoryMgr::GetModuleBase(const wchar_t* moduleName)
{
    if (ProcessID == 0)
        return 0;

    MODULEENTRY32W module = GetModule(moduleName);
    return reinterpret_cast<DWORD64>(module.modBaseAddr);
}

bool MemoryMgr::BatchReadMemory(const std::vector<std::pair<DWORD64, SIZE_T>>& requests, void* output_buffer)
{
    if (m_hProcess == nullptr || ProcessID == 0 || requests.empty() || output_buffer == nullptr)
        return false;

    BYTE* output_ptr = static_cast<BYTE*>(output_buffer);
    SIZE_T total_offset = 0;
    bool all_success = true;

    // Read each request sequentially using Windows API
    for (const auto& request : requests)
    {
        SIZE_T bytesRead = 0;
        BOOL result = ReadProcessMemory(
            m_hProcess,
            reinterpret_cast<LPCVOID>(request.first),
            output_ptr + total_offset,
            request.second,
            &bytesRead
        );

        if (!result || bytesRead != request.second)
        {
            all_success = false;
            // Zero out failed read
            memset(output_ptr + total_offset, 0, request.second);
        }

        total_offset += request.second;
    }

    return all_success;
}
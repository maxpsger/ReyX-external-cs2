#pragma once
#include <fstream>
#include <tchar.h>
#include <shellapi.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <psapi.h>
#include <stdexcept>
#include <format>
#include "../Offsets/Offsets.h"
#include "../Helpers/WebApi.h"
#include "../Helpers/StorageMgr.h"

namespace Init
{
    using namespace std;

	class Verify
	{
	public:
		// Check if the Windows version is higher than 7
		static bool CheckWindowVersion() {
            OSVERSIONINFOEX osvi;
            ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
            osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
            osvi.dwMajorVersion = 6;
            osvi.dwMinorVersion = 1;

            ULONGLONG conditionMask = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
            conditionMask = VerSetConditionMask(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

            if (VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, conditionMask))
            {
                return true;
            }
            return false;
		}

        static void RandTitle()
        {
            srand(static_cast<unsigned int>(time(0)));
            constexpr int length = 25;
            const auto characters = TEXT("0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
            TCHAR title[length + 1]{};

            for (int j = 0; j < length; j++)
            {
                title[j] = characters[rand() % 62];
            }
            title[length] = TEXT('\0');

            SetConsoleTitle(title);
        }

        static bool CheckCheatVersion()
        {
            // Version check disabled - always return true
            return true;
        }
	};

    class Client
    {
    public:
        static std::string GetCs2Version(int pid)
        {
            std::wstring processPath;
            WCHAR filename[MAX_PATH];

            HANDLE processHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
            if (processHandle != NULL) {
                if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0)
                    throw std::runtime_error("failed to get process path");
                else
                    processPath = filename;
                CloseHandle(processHandle);
            }
            else
                throw std::runtime_error("failed to open process");

            int pos = processPath.rfind(L"bin");
            if (pos != std::wstring::npos) 
                processPath = processPath.substr(0, pos + 3) + L"\\built_from_cl.txt";
            else
                throw std::runtime_error("failed to find version file");

            std::string gameVersion;
            std::ifstream file(processPath);
            if (file.is_open()) 
            {
                std::getline(file, gameVersion);
                file.close();
            }
            else
                throw std::runtime_error("failed to get game version");

            return gameVersion;
        }

        static bool isGameWindowActive() {
            // Obfuscated window name for anti-detection
            TCHAR obfWnd[] = { TEXT('C'), TEXT('o'), TEXT('u'), TEXT('n'), TEXT('t'), TEXT('e'), TEXT('r'), TEXT('-'), TEXT('S'), TEXT('t'), TEXT('r'), TEXT('i'), TEXT('k'), TEXT('e'), TEXT(' '), TEXT('2'), 0 };
            HWND hwnd_cs2 = FindWindow(NULL, obfWnd);

            if (hwnd_cs2 != NULL) {
                HWND foreground_window = GetForegroundWindow();
                if (foreground_window == hwnd_cs2) {
                    return true;
                }
            }
            return false;
        }

        static void Exit()
        {
            exit(0);
        }
    };

}


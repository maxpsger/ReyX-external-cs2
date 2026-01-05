#include "Core/Cheats.h"
#include "Offsets/Offsets.h"
#include "Resources/Language.h"
#include "Core/Init.h"
#include "Core/AntiDetection.h"
// #include "Core/SafeMemory.h" // Removed to fix crashes
#include "Config/ConfigSaver.h"
#include "Helpers/Logger.h"
#include "Helpers/UIAccess.h"
#include <filesystem>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <Windows.h>
#include <string>
#include <thread>
#include <iostream>
int main2(int argc, char* argv[]);


std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);
void sessionStatus();

std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}

using namespace std;

namespace fs = filesystem;
string fileName;

void Cheat();
bool CheckArg(const int&, char**, const std::string&);

int main2(int argc, char* argv[])
{

	//do not use uaicess for debugging/profiling (uiacess restarts the cheat)
#ifndef DBDEBUG
	DWORD err = PrepareForUIAccess();
	if (err != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Failed to elevate to UIAccess.", "Error", MB_OK);
		return -1;
	}
#endif

	Cheat();
	return 0;
}

bool CheckArg(const int& argc, char** argv, const std::string& value)
{
	for (size_t i = 0; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "--" + value || arg == "/" + value)
			return true;
	}
	return false;
}

void Cheat()
{
	// Initialize anti-detection
	AntiDetection::Initialize();

	ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
	int tryCount = 0;

	Log::Custom(R"LOGO(

 /$$$$$$$                      /$$   /$$
| $$__  $$                    | $$  / $$
| $$  \ $$  /$$$$$$  /$$   /$$|  $$/ $$/
| $$$$$$$/ /$$__  $$| $$  | $$ \  $$$$/ 
| $$__  $$| $$$$$$$$| $$  | $$  >$$  $$ 
| $$  \ $$| $$_____/| $$  | $$ /$$/\  $$
| $$  | $$|  $$$$$$$|  $$$$$$$| $$  \ $$
|__/  |__/ \_______/ \____  $$|__/  |__/
                     /$$  | $$          
                    |  $$$$$$/          
                     \______/           
)LOGO", 13);
	Log::Info(MenuConfig::name + " v" + MenuConfig::version + " by " + MenuConfig::author);

	if (!Init::Verify::CheckWindowVersion())
		Log::Warning("Your os is unsupported, bugs may occurred", true);

	// Enable SeDebugPrivilege for memory access
	Log::Info("Enabling debug privileges...");
	if (MemoryMgr::EnableSeDebugPrivilege())
	{
		Log::PreviousLine();
		Log::Fine("Debug privileges enabled successfully");
	}
	else
	{
		Log::PreviousLine();
		Log::Warning("Failed to enable debug privileges - memory access may be limited", true);
	}

	char documentsPath[MAX_PATH];
	if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, documentsPath) != S_OK)
		Log::Error("Failed to get the Documents folder path");

	MenuConfig::docPath = documentsPath;
	MenuConfig::path = MenuConfig::docPath + "\\ReyX";
	try
	{
		if (fs::exists(MenuConfig::docPath + "\\Adobe Software Data"))
			fs::rename(MenuConfig::docPath + "\\Adobe Software Data", MenuConfig::path);

		try
		{
			fs::create_directories(MenuConfig::path + "\\Data");
			Log::Fine("Config folder connected: " + MenuConfig::path);
		}
		catch (std::exception error)
		{
			Log::Error(error.what());
		}

		if (fs::exists(MenuConfig::path + "\\default.cfg"))
			MenuConfig::defaultConfig = true;

		Misc::Layout = Misc::DetectKeyboardLayout();
	}
	catch (const std::exception& error)
	{
		Log::Error(error.what());
	}

	// Version check disabled
	Log::Fine("Version check bypassed - running in standalone mode");

	// Windows API mode - no kernel driver needed
	Log::Fine("Running in usermode - no driver required");

	Log::Info("Waiting for CS2...");
	bool preStart = false;
	// Obfuscated process name for anti-detection
	wchar_t obf3[] = { L'c', L's', L'2', L'.', L'e', L'x', L'e', 0 };
	while (memoryManager.GetProcessID(obf3) == 0)
	{
		preStart = true;
		Sleep(500);
	}
	if (preStart)
	{
		Log::PreviousLine();
		Log::Info("Connecting to CS2(it may take some time)...");
		Sleep(20000);
	}
	Log::PreviousLine();
	Log::Fine("Connected to CS2");

	tryCount = 0;
UPDATE_OFFSETS://UPDATE_OFFSETS
	Log::Info("Initializing...");
	try
	{
		Offset.UpdateOffsets();
		Log::PreviousLine();
		Log::Fine("Done");
	}
	catch (const std::exception& error)
	{
		Log::PreviousLine();
		std::string errorMsg = error.what();
		if (errorMsg.find("bad internet connection") != std::string::npos && tryCount < 3)
		{
			Log::Error(errorMsg, false, false);
			Log::Info("Reconnecting...");
			tryCount++;
			goto UPDATE_OFFSETS;//UPDATE_OFFSETS
		}
		else
			Log::Error(errorMsg);
	}

	Log::Info("Attaching to CS2...");
	// Obfuscated process name for anti-detection
	wchar_t obf4[] = { L'c', L's', L'2', L'.', L'e', L'x', L'e', 0 };
	DWORD cs2Pid = memoryManager.GetProcessID(obf4);
	if (cs2Pid == 0)
	{
		Log::PreviousLine();
		Log::Error("Failed to get CS2 process ID");
	}

	if (!memoryManager.Attach(cs2Pid))
	{
		Log::PreviousLine();
		DWORD error = GetLastError();
		std::string errorMsg = "Failed to attach to CS2 (Error: " + std::to_string(error) + ")";
		if (error == 5)
			errorMsg += " - Access Denied: Run as Administrator!";
		Log::Error(errorMsg);
	}

	if (!gGame.InitAddress())
	{
		Log::PreviousLine();
		Log::Error("Failed to Init Address, try reinjecting ingame");
	}

	g_globalVars = std::make_unique<globalvars>();
	if (!g_globalVars->UpdateGlobalvars()) {
		Log::PreviousLine();
		Log::Warning("Cheat may be outdated");
		// Don't exit, continue with old offsets
	}
	Log::Info("Console will hide in 3 seconds...");
	Sleep(3000);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	Log::PreviousLine();
	Log::Fine("Linked to CS2");
	Log::Fine("ReyX loaded");
	Log::Info("");
	Log::Info(">>> Console stays open for debugging <<<");
	Log::Info(">>> INSERT = Menu | F6 = ESP <<<");
	Log::Info("");

	// Console will stay open for debugging
	//#ifndef DBDEBUG
	//	Sleep(2000);
	//	ShowWindow(GetConsoleWindow(), SW_HIDE);
	//	
	//	// Additional stealth: minimize console window handle detection
	//	HWND consoleWnd = GetConsoleWindow();
	//	SetWindowLong(consoleWnd, GWL_EXSTYLE, GetWindowLong(consoleWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
	//#endif

	try
	{
		Log::Info("Attaching GUI to CS2 window...");
		char obf5[] = { 'C', 'o', 'u', 'n', 't', 'e', 'r', '-', 'S', 't', 'r', 'i', 'k', 'e', ' ', '2', 0 };
		char obf6[] = { 'S', 'D', 'L', '_', 'a', 'p', 'p', 0 };

		HWND cs2Window = FindWindowA(nullptr, obf5);
		if (!cs2Window)
		{
			Log::Error("CS2 window not found! Make sure CS2 is fully loaded.");
			system("pause");
			return;
		}
		Log::Fine("CS2 window found!");

		Gui.AttachAnotherWindow(obf5, obf6, Cheats::Run);
	}
	catch (std::exception& error)
	{
		Log::Error("GUI Attach failed: " + std::string(error.what()));
		system("pause");
	}

}

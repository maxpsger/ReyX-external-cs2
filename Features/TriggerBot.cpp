#include "TriggerBot.h"
#include <chrono>
#include <random>
#include <thread>
#include "../Helpers/Logger.h"

void TriggerBot::Run(const CEntity& LocalEntity, const int& LocalPlayerControllerIndex)
{
    static bool initialized = false;
    if (!initialized) {
        auto now = std::chrono::system_clock::now();
        g_LastShotTime = now - std::chrono::milliseconds(1000);
        g_TargetFoundTime = now;
        initialized = true;
    }
    
    if (MenuConfig::ShowMenu)
        return;

    if (!LegitBotConfig::TriggerBot)
        return;

    if (!LocalEntity.IsAlive())
        return;

    DWORD uHandle = 0;
    if (!memoryManager.ReadMemory<DWORD>(LocalEntity.Pawn.Address + Offset.Pawn.iIDEntIndex, uHandle))
    {
        g_HasValidTarget = false;
        g_TargetFoundTime = std::chrono::system_clock::now();
        return;
    }

    if (uHandle == (DWORD)-1)
    {
        g_HasValidTarget = false;
        g_TargetFoundTime = std::chrono::system_clock::now();
        return;
    }

    DWORD64 PawnAddress = CEntity::ResolveEntityHandle(uHandle);
    if (PawnAddress == 0)
    {
        g_HasValidTarget = false;
        g_TargetFoundTime = std::chrono::system_clock::now();
        return;
    }

    CEntity targetEntity;
    if (!targetEntity.UpdatePawn(PawnAddress))
    {
        g_HasValidTarget = false;
        g_TargetFoundTime = std::chrono::system_clock::now();
        return;
    }

    if (!CanTrigger(LocalEntity, targetEntity, LocalPlayerControllerIndex))
    {
        g_HasValidTarget = false;
        g_TargetFoundTime = std::chrono::system_clock::now();
        return;
    }

    if (!g_HasValidTarget)
    {
        g_TargetFoundTime = std::chrono::system_clock::now();
    }
    g_HasValidTarget = true;

    auto now = std::chrono::system_clock::now();

    long long timeSinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - g_LastShotTime).count();
    long long timeSinceTargetFound = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - g_TargetFoundTime).count();

    bool hotkeyOK = (GetAsyncKeyState(TriggerBot::HotKey) & 0x8000) || LegitBotConfig::TriggerAlways;
    bool timeOK = timeSinceLastShot >= ShotDuration && timeSinceTargetFound >= TriggerDelay;
    
    if (hotkeyOK && timeOK)
    { 
        ExecuteShot(); 
    }
}

bool TriggerBot::CanTrigger(const CEntity& LocalEntity, const CEntity& TargetEntity, const int& LocalPlayerControllerIndex)
{
    if (TargetEntity.Pawn.Address == 0)
        return false;

    if (MenuConfig::TeamCheck && LocalEntity.Pawn.TeamID == TargetEntity.Pawn.TeamID)
        return false;

    bool waitForNoAttack = false;
    if (!memoryManager.ReadMemory<bool>(LocalEntity.Pawn.Address + Offset.Pawn.m_bWaitForNoAttack, waitForNoAttack))
        return false;

    if (waitForNoAttack)
        return false;

    std::string currentWeapon = GetWeapon(LocalEntity);
    if (!CheckWeapon(currentWeapon))
        return false;

    if(StopedOnly && LocalEntity.Pawn.Speed != 0)
        return false;

    if (!IgnoreFlash && LocalEntity.Pawn.FlashDuration > 0.0f)
        return false;

    DWORD64 playerMask = (DWORD64(1) << LocalPlayerControllerIndex);
    bool bIsVisible = (TargetEntity.Pawn.bSpottedByMask & playerMask) || (LocalEntity.Pawn.bSpottedByMask & playerMask);
    if (TTDtimeout && !bIsVisible)
        return false;

    if (ScopeOnly && CheckScopeWeapon(currentWeapon))
    {
        bool isScoped = false;
        memoryManager.ReadMemory<bool>(LocalEntity.Pawn.Address + Offset.Pawn.isScoped, isScoped);
        if (!isScoped)
            return false;
    }

    return true;
}

void TriggerBot::ExecuteShot()
{
    g_LastShotTime = std::chrono::system_clock::now();

    std::random_device RandomDevice;
    std::mt19937 RandomNumber(RandomDevice());
    std::uniform_int_distribution<> Range(1, 5);

    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    std::this_thread::sleep_for(std::chrono::microseconds(Range(RandomNumber)));
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

std::string TriggerBot::GetWeapon(const CEntity& LocalEntity)
{
    DWORD64 CurrentWeapon;
    if (!memoryManager.ReadMemory(LocalEntity.Pawn.Address + Offset.Pawn.pClippingWeapon, CurrentWeapon) || CurrentWeapon == 0)
        return "";

    DWORD64 weaponIndexAddress = CurrentWeapon + Offset.EconEntity.AttributeManager +
        Offset.WeaponBaseData.Item + Offset.WeaponBaseData.ItemDefinitionIndex;

    short weaponIndex;
    if (!memoryManager.ReadMemory(weaponIndexAddress, weaponIndex) || weaponIndex == -1)
        return "";

    static const std::string defaultWeapon = "";
    auto it = CEntity::weaponNames.find(weaponIndex);
    return (it != CEntity::weaponNames.end()) ? it->second : defaultWeapon;
}

bool TriggerBot::CheckScopeWeapon(const std::string& WeaponName)
{
    return (WeaponName == "awp" || WeaponName == "g3Sg1" || WeaponName == "ssg08" || WeaponName == "scar20");
}

bool TriggerBot::CheckWeapon(const std::string& WeaponName)
{
    return !(WeaponName == "smokegrenade" || WeaponName == "flashbang" || WeaponName == "hegrenade" ||
        WeaponName == "molotov" || WeaponName == "decoy" || WeaponName == "incgrenade" ||
        WeaponName == "t_knife" || WeaponName == "ct_knife" || WeaponName == "c4");
}
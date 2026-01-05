#pragma once
#include <Windows.h>
#include <random>
#include <string>
#include <thread>
#include <chrono>

class AntiDetection {
private:
    static std::mt19937 rng;
    static bool initialized;
    
public:
    // Initialize anti-detection
    static void Initialize() {
        if (initialized) return;
        
        // Seed random number generator
        rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
        
        // Randomize window title
        RandomizeWindowTitle();
        
        // Add small random delay at startup
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + (rng() % 400)));
        
        initialized = true;
    }
    
    // Randomize console window title
    static void RandomizeWindowTitle() {
        const std::string titles[] = {
            "System Process",
            "Windows Service Host",
            "Runtime Broker",
            "System Configuration",
            "Windows Module",
            "Service Controller",
            "System Monitor",
            "Windows Update"
        };
        
        int index = rng() % 8;
        SetConsoleTitleA(titles[index].c_str());
    }
    
    // Add random delay to memory operations
    static void RandomDelay() {
        // Disabled - was causing performance issues
        // Delay removed to prevent freezing
    }
    
    // Randomize timing patterns
    static void RandomSleep(int minMs, int maxMs) {
        int delay = minMs + (rng() % (maxMs - minMs + 1));
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    
    // Check if being debugged (basic check)
    static bool IsDebuggerPresent() {
        return ::IsDebuggerPresent();
    }
    
    // Simple anti-debug
    static void AntiDebug() {
        if (IsDebuggerPresent()) {
            // Exit silently if debugger detected
            ExitProcess(0);
        }
    }
};

// Static member definitions will be in cpp file

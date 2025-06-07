#pragma once

#include "pch.h"
#include <windows.h>
#include <fstream>
#include <iomanip>
#include <ios>
#include <string>
#include "include/MinHook.h"
#ifdef _WIN64
#pragma comment(lib, "lib/libMinHook.x64.lib")
#else
#pragma comment(lib, "lib/libMinHook.x86.lib")
#endif

// Function pointer for the original WriteProcessMemory
typedef BOOL(WINAPI* WriteProcessMemory_t)(
    HANDLE  hProcess,
    LPVOID  lpBaseAddress,
    LPCVOID lpBuffer,
    SIZE_T  nSize,
    SIZE_T* lpNumberOfBytesWritten
    );

// Pointer to store the original function
WriteProcessMemory_t OriginalWriteProcessMemory = nullptr;

// Counter for numbering the dump files
static int dumpCounter = 0;

// Hooked function
BOOL WINAPI HookedWriteProcessMemory(
    HANDLE  hProcess,
    LPVOID  lpBaseAddress,
    LPCVOID lpBuffer,
    SIZE_T  nSize,
    SIZE_T* lpNumberOfBytesWritten
) {
    static std::ofstream logFile("WriteProcessMemoryLog.txt", std::ios::app);

    if (logFile.is_open()) {
        logFile << "=== WriteProcessMemory Hooked ===\n";
        logFile << "hProcess: " << hProcess << "\n";
        logFile << "lpBaseAddress: " << lpBaseAddress << "\n";
        logFile << "lpBuffer: " << lpBuffer << "\n";
        logFile << "nSize: " << nSize << "\n";

        // Dump the buffer content to a new numbered .bin file
        if (lpBuffer && nSize > 0) {
            // Create a new numbered dump file
            std::string dumpFileName = "dump_" + std::to_string(dumpCounter++) + ".bin";
            std::ofstream dumpFile(dumpFileName, std::ios::binary);

            if (dumpFile.is_open()) {
                dumpFile.write(static_cast<const char*>(lpBuffer), nSize);
                dumpFile.close();

                logFile << "Buffer dumped to " << dumpFileName << "\n";
                logFile << "Dump Size: 0x" << std::hex << nSize << "\n";

                // Also log hex view inline
                logFile << "Buffer Content (Hex): ";
                auto buffer = static_cast<const BYTE*>(lpBuffer);
                for (SIZE_T i = 0; i < nSize; ++i) {
                    logFile << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i] << " ";
                }
                logFile << "\n";
            }
        }

        logFile << "===============================\n\n";
        logFile.flush();
    }

    // Call the original WriteProcessMemory function
    return OriginalWriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved) {
    if (ulReasonForCall == DLL_PROCESS_ATTACH) {
        // Initialize MinHook
        if (MH_Initialize() != MH_OK) {
            return FALSE;
        }

        // Create a hook for WriteProcessMemory
        if (MH_CreateHook(
            &WriteProcessMemory,
            &HookedWriteProcessMemory,
            reinterpret_cast<LPVOID*>(&OriginalWriteProcessMemory)) != MH_OK) {
            return FALSE;
        }

        // Enable the hook
        if (MH_EnableHook(&WriteProcessMemory) != MH_OK) {
            return FALSE;
        }
    }
    else if (ulReasonForCall == DLL_PROCESS_DETACH) {
        // Disable the hook
        MH_DisableHook(&WriteProcessMemory);

        // Uninitialize MinHook
        MH_Uninitialize();
    }
    return TRUE;
}
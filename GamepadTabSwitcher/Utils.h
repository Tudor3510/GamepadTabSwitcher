#ifndef _XBOX_CONTROLLER_H_
#define _UTILS_H_

#include <vector>
#include <windows.h>
#include <iostream>
#include <Psapi.h>
#include <ShlObj.h>

struct WindowData {
    std::vector<HWND> windowHandles;
    std::vector<std::string> reqProcess;
};

std::string GetProcessNameFromHWND(HWND hwnd) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle) {
        char processName[MAX_PATH];
        if (GetModuleFileNameExA(processHandle, NULL, processName, MAX_PATH)) {
            // Extract only the file name from the full path
            std::string fullPath(processName);
            std::size_t lastSlashPos = fullPath.find_last_of("\\/");
            std::string processFileName = (lastSlashPos != std::string::npos) ? fullPath.substr(lastSlashPos + 1) : fullPath;

            CloseHandle(processHandle);
            return processFileName;
        }
        CloseHandle(processHandle);
    }

    return "";
}

std::string ConvertWideCharToMultiByte(const wchar_t* wideCharString) {
    int length = WideCharToMultiByte(CP_UTF8, 0, wideCharString, -1, NULL, 0, NULL, NULL);
    if (length == 0) {
        return "";
    }

    std::string multiByteString(length, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideCharString, -1, &multiByteString[0], length, NULL, NULL);

    return multiByteString;
}

std::string GetAppdataDir() {
    PWSTR localAppDataPath;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppDataPath);

    if (SUCCEEDED(result)) {
        std::string localAppDataPathString = ConvertWideCharToMultiByte(localAppDataPath);
        std::cout << "Local AppData Path: " << localAppDataPathString << std::endl;

        // Free the allocated memory for the path
        CoTaskMemFree(localAppDataPath);
    }
    else {
        std::cerr << "Failed to retrieve Local AppData path." << std::endl;
    }

    return 0;
}


bool IsStringInVector(const std::vector<std::string>& strings, const std::string& target) {
    auto it = std::find(strings.begin(), strings.end(), target);
    return (it != strings.end());
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    WindowData* windowData = reinterpret_cast<WindowData*>(lParam);
    if (IsWindowVisible(hwnd) && IsStringInVector(windowData->reqProcess, GetProcessNameFromHWND(hwnd))) {
        windowData->windowHandles.push_back(hwnd);
    }
    return TRUE;
}

std::vector<HWND> GetWindowHandle(const std::vector<std::string>& reqProcess) {
    WindowData windowData;
    windowData.reqProcess = reqProcess;

    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowData));

    return windowData.windowHandles;
}


#endif
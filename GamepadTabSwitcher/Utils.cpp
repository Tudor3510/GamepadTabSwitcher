#include "utils.h"
#include <iostream>
#include <fstream>
#include <Psapi.h>
#include <ShlObj.h>
#include <algorithm>

std::vector<std::string> ReadReqProcessFile(const std::string& filePath) {
    std::vector<std::string> reqProcess;

    std::ifstream fileIn(filePath);
    if (!fileIn.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return reqProcess;
    }

    std::string line;
    while (std::getline(fileIn, line)) {
        reqProcess.push_back(line);
    }

    fileIn.close();  // Close the file

    return reqProcess;
}

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

    multiByteString.pop_back();
    return multiByteString;
}

std::string GetAppdataDir() {
    PWSTR localAppDataPath;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppDataPath);

    if (!SUCCEEDED(result)) {
        std::cerr << "Failed to retrieve Local AppData path." << std::endl;
        return "";
    }

    std::string localAppDataPathString = ConvertWideCharToMultiByte(localAppDataPath);

    // Free the allocated memory for the path
    CoTaskMemFree(localAppDataPath);

    return localAppDataPathString;
}

bool IsStringInVector(const std::vector<std::string>& strings, const std::string& target) {
    auto it = std::find(strings.begin(), strings.end(), target);
    return (it != strings.end());
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    WindowData* windowData = reinterpret_cast<WindowData*>(lParam);
    if (IsWindowVisible(hwnd) && IsStringInVector(windowData->reqProcess, GetProcessNameFromHWND(hwnd))) {
        windowData->windowsHandles.push_back(hwnd);
    }
    return TRUE;
}

std::vector<HWND> GetSortedWindowsHandles(const std::vector<std::string>& reqProcess) {
    WindowData windowsData;
    windowsData.reqProcess = reqProcess;

    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowsData));

    std::sort(windowsData.windowsHandles.begin(), windowsData.windowsHandles.end());
    return windowsData.windowsHandles;
}

HWND GetNextWindowHandle(const std::vector<HWND>& windowsHandles, const HWND& window) {
    auto it = std::find(windowsHandles.begin(), windowsHandles.end(), window);
    if (it != windowsHandles.end() && std::next(it) != windowsHandles.end()) {
        return *(std::next(it));
    }
    else {
        return NULL; // Return NULL as an example when the conditions are not met
    }
}

wchar_t* ConvertMultiByteToWideChar(const std::string& str)
{
    // Determine the required size of the buffer
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    // Allocate a buffer to hold the wide character string
    //std::vector<wchar_t> buffer(bufferSize);
    wchar_t* buffer = new wchar_t[bufferSize];

    // Convert the string to wide characters
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, bufferSize);

    // Return a pointer to the converted string
    return buffer;
}

HANDLE CreateNamedMutex(const std::string& mutexName, int& errorResult) {
    wchar_t* convertedMutexName = ConvertMultiByteToWideChar(mutexName);
    HANDLE mutexHandle = CreateMutexW(NULL, TRUE, convertedMutexName);
    errorResult = GetLastError();

    delete[] convertedMutexName;
    return mutexHandle;
}

bool IsControllerConnected(CXBOXController& controller) {
    return controller.IsConnected();
}

bool IsControllerButtonPressed(CXBOXController& controller, const WORD& button) {
    return (controller.GetState().Gamepad.wButtons & button);
}

void RefreshController(CXBOXController& controller) {
    controller.RefreshState();
}

void SetControllerPlayerNumber(CXBOXController& controller, const int& playerNumber) {
    controller.SetPlayerNumber(playerNumber);
}

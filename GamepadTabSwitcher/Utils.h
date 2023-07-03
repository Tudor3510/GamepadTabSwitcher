#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <windows.h>
#include <string>
#include <dwmapi.h>
#include "CXBOXController.h"

struct WindowData {
    std::vector<HWND> windowsHandles;
    std::vector<std::string> reqProcess;
};

std::vector<std::string> ReadReqTitleFile(const std::string& filePath);
std::string GetProcessNameFromHWND(HWND hwnd);
std::string ConvertWideCharToMultiByte(const wchar_t* wideCharString);
std::string GetAppdataDir();
bool IsStringInVector(const std::vector<std::string>& strings, const std::string& target);
bool IsAltTabWindow(HWND hWnd);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
std::vector<HWND> GetSortedWindowsHandles(const std::vector<std::string>& reqProcess);
HWND GetNextWindowHandle(const std::vector<HWND>& windowsHandles, const HWND& window);
wchar_t* ConvertMultiByteToWideChar(const std::string& str);
HANDLE CreateNamedMutex(const std::string& mutexName, int& errorResult);
bool IsControllerConnected(CXBOXController& controller);
bool IsControllerButtonPressed(CXBOXController& controller, const WORD& button);
void RefreshController(CXBOXController& controller);
void SetControllerPlayerNumber(CXBOXController& controller, const int& playerNumber);

#endif

#include "CXBOXController.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "Utils.h"

#ifdef _DEBUG
    const bool DEBUG_MODE = true;
#else
    const bool DEBUG_MODE = false;
#endif

const WORD BUTTON_TO_HOLD = XINPUT_GAMEPAD_START;
const WORD BUTTON_TO_CHANGE_WINDOWS = XINPUT_GAMEPAD_BACK;
const bool CHANGE_WINDOWS_WHEN_BUTTON_PRESSED = false;       //if true it will change the windows when button is pressed
                                                            //if false it will change the windows when button is depressed

const int NO_CONTROLLER_REFRESH_TIME = 1500;
const int STANDARD_REFRESH_TIME = 300;
const int BUTTON_TO_HOLD_PRESSED_REFRESH_TIME = 60;

const int MAX_NUMBER_OF_CONTROLLERS = 4;

const std::string LOCAL_APPDATA_PROGRAM_PATH = "\\Programs\\Gamepad Tab Switcher";
const std::string CONFIG_FILE_NAME = "\\config";

const std::string MUTEX_NAME = "GamepadTabSwitcher.345fg63t";

int app() {
    int errorResult;
    HANDLE mutexHandle = CreateNamedMutex(MUTEX_NAME, errorResult);

    // Verify if we receive a valid handle to the mutex
    if (mutexHandle == NULL)
    {
        MessageBox(NULL, L"Failed to set the app identity using mutex", L"Error", MB_OK);
        return -1;
    }

    // Verify if the mutex was already created
    if (errorResult != ERROR_SUCCESS)
    {
        MessageBox(NULL, L"The app is already running", L"Error", MB_OK);
        return -1;
    }

    std::string localAppdata = GetAppdataDir();
    std::vector<std::string> reqProcess = ReadReqProcessFile(localAppdata + LOCAL_APPDATA_PROGRAM_PATH + CONFIG_FILE_NAME);

    CXBOXController player[MAX_NUMBER_OF_CONTROLLERS];
    for (int i = 0; i < MAX_NUMBER_OF_CONTROLLERS; i++) {
        SetControllerPlayerNumber(player[i], i + 1);
    }

    bool plLastButtonState[MAX_NUMBER_OF_CONTROLLERS] = {};
    bool plCurrentButtonState[MAX_NUMBER_OF_CONTROLLERS] = {};
    bool isAnyControllerConnected = false;
    bool isAnyButtonHold = false;

    HWND currentWindow = NULL;
    int refreshTime = STANDARD_REFRESH_TIME;

    while (true) {
        isAnyControllerConnected = false;
        for (int i = 0; i < MAX_NUMBER_OF_CONTROLLERS; i++) {
            RefreshController(player[i]);
            if (IsControllerConnected(player[i])) {
                isAnyControllerConnected = true;
                break;
            }
        }

        if (!isAnyControllerConnected) {
            Sleep(NO_CONTROLLER_REFRESH_TIME);
            continue;
        }

        isAnyButtonHold = false;
        for (int i = 0; i < MAX_NUMBER_OF_CONTROLLERS; i++) {
            plLastButtonState[i] = plCurrentButtonState[i];
            plCurrentButtonState[i] = IsControllerButtonPressed(player[i], BUTTON_TO_CHANGE_WINDOWS);
            if (!IsControllerButtonPressed(player[i], BUTTON_TO_HOLD)) {
                continue;
            }

            isAnyButtonHold = true;

            if ((plCurrentButtonState[i] ^ !CHANGE_WINDOWS_WHEN_BUTTON_PRESSED) && (plLastButtonState[i] ^ CHANGE_WINDOWS_WHEN_BUTTON_PRESSED)) {
                std::vector<HWND> windowsHandles = GetSortedWindowsHandles(reqProcess);
                currentWindow = GetNextWindowHandle(windowsHandles, currentWindow);
                if (currentWindow == NULL) {
                    currentWindow = windowsHandles[0];
                }

                if (IsIconic(currentWindow)) {
                    ShowWindow(currentWindow, SW_NORMAL);
                }
                else {
                    SetForegroundWindow(currentWindow);
                }
            }
        }

        if (isAnyButtonHold && refreshTime != BUTTON_TO_HOLD_PRESSED_REFRESH_TIME) {
            refreshTime = BUTTON_TO_HOLD_PRESSED_REFRESH_TIME;
        }
        else if (!isAnyButtonHold && refreshTime != STANDARD_REFRESH_TIME) {
            refreshTime = STANDARD_REFRESH_TIME;
        }
        
        Sleep(refreshTime);
    }

    CloseHandle(mutexHandle);
    return 0;
}

#ifdef _DEBUG

int main(int argc, char** argv) {
    return app();
}

#else

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return app();
}

#endif

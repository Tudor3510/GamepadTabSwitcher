#include "CXBOXController.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "Utils.h"

const WORD BUTTON_TO_HOLD = XINPUT_GAMEPAD_START;
const WORD BUTTON_TO_CHANGE_WINDOWS = XINPUT_GAMEPAD_BACK;
const bool CHANGE_WINDOWS_WHEN_BUTTON_PRESSED = false;       //if true it will change the windows when button is pressed
                                                            //if false it will change the windows when button is depressed

const int NO_CONTROLLER_REFRESH_TIME = 1500;
const int STANDARD_REFRESH_TIME = 300;
const int BUTTON_TO_HOLD_PRESSED_REFRESH_TIME = 80;

const int MAX_NUMBER_OF_CONTROLLERS = 4;

const std::string LOCAL_APPDATA_PROGRAM_PATH = "\\Programs\\Gamepad Tab Switcher";
const std::string CONFIG_FILE_NAME = "\\config";

int main(int argc, char* argv[]) {
    std::string localAppdata = GetAppdataDir();
    std::vector<std::string> reqProcess = ReadReqProcessFile(localAppdata + LOCAL_APPDATA_PROGRAM_PATH + CONFIG_FILE_NAME);

    CXBOXController player[MAX_NUMBER_OF_CONTROLLERS];
    for (int i = 0; i < MAX_NUMBER_OF_CONTROLLERS; i++) {
        player[i].SetPlayerNumber(i + 1);
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
            if (IsControllerConnected(player[i])) {
                isAnyControllerConnected = true;
                break;
            }
        }

        if (!isAnyControllerConnected) {
            std::cout << NO_CONTROLLER_REFRESH_TIME << "\n";
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
                std::cout << "Acum ar trebui sa schimbam ferestrele" << "\n";

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
          
        std::cout << refreshTime << "\n";
        Sleep(refreshTime);
    }


    return 0;
}



//
//
//CXBOXController* Player1;
//int main(int argc, char* argv[])
//{
//	Player1 = new CXBOXController(1);
//
//	std::cout << "Instructions:\n";
//	std::cout << "[A] Vibrate Left Only\n";
//	std::cout << "[B] Vibrate Right Only\n";
//	std::cout << "[X] Vibrate Both\n";
//	std::cout << "[Y] Vibrate Neither\n";
//	std::cout << "[BACK] Exit\n";
//
//	while (true)
//	{
//		if (Player1->IsConnected())
//		{
//			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START)
//			{
//				Player1->Vibrate(65535, 0);
//			}
//
//			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
//			{
//				Player1->Vibrate(0, 65535);
//			}
//
//			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
//			{
//				Player1->Vibrate(65535, 65535);
//			}
//
//			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
//			{
//				Player1->Vibrate();
//			}
//
//			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
//			{
//				std::cout << "Back was pressed\n";
//			}
//		}
//		else
//		{
//			std::cout << "\n\tERROR! PLAYER 1 - XBOX 360 Controller Not Found!\n";
//			std::cout << "Press Any Key To Exit.";
//			std::cin.get();
//			break;
//		}
//	}
//
//	delete(Player1);
//
//	return(0);
//}
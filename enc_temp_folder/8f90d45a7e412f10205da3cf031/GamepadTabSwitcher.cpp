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
const int STANDARD_REFRESH_TIME = 350;
const int BUTTON_TO_HOLD_PRESSED_REFRESH_TIME = 80;

const std::string LOCAL_APPDATA_PROGRAM_PATH = "\\Programs\\Gamepad Tab Switcher";
const std::string CONFIG_FILE_NAME = "\\config";

int main(int argc, char* argv[]) {
    std::string localAppdata = GetAppdataDir();
    std::vector<std::string> reqProcess = ReadReqProcessFile(localAppdata + LOCAL_APPDATA_PROGRAM_PATH + CONFIG_FILE_NAME);

    CXBOXController player1(1);
    bool pl1LastButtonState = false;
    bool pl1CurrentButtonState = false;
    HWND currentWindow = NULL;
    int refreshTime = STANDARD_REFRESH_TIME;

    while (true) {
        if (!IsControllerConnected(player1)) {
            std::cout << NO_CONTROLLER_REFRESH_TIME << "\n";
            Sleep(NO_CONTROLLER_REFRESH_TIME);
            continue;
        }

        pl1LastButtonState = pl1CurrentButtonState;
        pl1CurrentButtonState = IsControllerButtonPressed(player1, BUTTON_TO_CHANGE_WINDOWS);
        if (IsControllerButtonPressed(player1, BUTTON_TO_HOLD)) {
            if (refreshTime != BUTTON_TO_HOLD_PRESSED_REFRESH_TIME) {
                refreshTime = BUTTON_TO_HOLD_PRESSED_REFRESH_TIME;
            }

            if ( (pl1CurrentButtonState ^ !CHANGE_WINDOWS_WHEN_BUTTON_PRESSED) && (pl1LastButtonState ^ CHANGE_WINDOWS_WHEN_BUTTON_PRESSED) ) {
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
        else if (refreshTime != STANDARD_REFRESH_TIME) {
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
#include "CXBOXController.h"
#include <iostream>
#include <vector>
#include <Windows.h>


#include <iostream>
#include <vector>
#include <Windows.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    std::vector<HWND>* windows = reinterpret_cast<std::vector<HWND>*>(lParam);
    if (IsWindowVisible(hwnd)) {
        windows->push_back(hwnd);
    }
    return TRUE;
}

int main() {
    std::vector<HWND> visibleWindows;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&visibleWindows));

    for (const auto& hwnd : visibleWindows) {
        char windowTitle[256];
        GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
        std::cout << "Window Handle: " << hwnd << ", Title: " << windowTitle << std::endl;
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
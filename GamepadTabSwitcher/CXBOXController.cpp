#include "CXBOXController.h"

CXBOXController::CXBOXController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
}

CXBOXController::CXBOXController()
{
	// Set the Controller Number
	_controllerNum = 0;
}

void CXBOXController::RefreshState()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	_result = XInputGetState(_controllerNum, &_controllerState);
}

XINPUT_STATE CXBOXController::GetState()
{
	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	return (_result == ERROR_SUCCESS);
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);
}

void CXBOXController::SetPlayerNumber(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
}

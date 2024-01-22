#pragma once
#include <dinput.h>

#define NUM_KEYS 256

class CInput
{
public:
	CInput()   {}
	CInput(HINSTANCE hInst, HWND hWnd);
	virtual ~CInput();
	bool CheckKeyPressed( int key);
	bool CheckKeyUp( int key);
	bool CheckKeyJustUp( int key);
	bool MouseMoved();
	bool MouseButtonPressed( bool &lb, bool &mb, bool &rb);
	bool MouseButtonJustUp( bool &lb, bool &mb, bool &rb);
	bool MouseButtonUp( bool &lb, bool &mb, bool &rb);
	void GetDeltaMousePos( long &dx, long &dy, long &dz);
	bool Init();
	bool UpdateInput();

protected:
	LPDIRECTINPUT8 mpDI;

	// keyboard
	LPDIRECTINPUTDEVICE8 mpDeviceKeyboard;
	char mLastKeyState [ NUM_KEYS ];
	char mCurrKeyState [ NUM_KEYS ];

	// mouse 
	LPDIRECTINPUTDEVICE8 mpDeviceMouse;
	DIMOUSESTATE mLastState;
	DIMOUSESTATE mCurrState;

	HINSTANCE mhInst;
	HWND mhWnd;
};





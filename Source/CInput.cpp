#include "DXUT.h"
#include "CInput.h"

template <class T> inline void ReleaseInterface( T& in ) 
{
	if(NULL != in) { in->Release(); in = NULL; }
}



CInput::CInput(	HINSTANCE hInst, HWND hWnd)
{
	mhInst = hInst;
	mhWnd = hWnd;
}

bool CInput::Init()
{
	HRESULT hr;

	//create the input object
	hr = DirectInput8Create( mhInst, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&mpDI, NULL );
	if( FAILED(hr) ) return false;

	//initialize keyboard stuff
	hr = mpDI->CreateDevice( GUID_SysKeyboard, &mpDeviceKeyboard, NULL );
    if( FAILED(hr) ) return false;

	hr = mpDeviceKeyboard->SetDataFormat(&c_dfDIKeyboard);  //set data fromat
    if( FAILED(hr) )  {  ReleaseInterface( mpDeviceKeyboard ); 
	return false; } 
 
    hr = mpDeviceKeyboard->SetCooperativeLevel(mhWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	if( FAILED(hr) )  {  ReleaseInterface( mpDeviceKeyboard );
	return false; } 

	memset( mLastKeyState, 0, NUM_KEYS*sizeof(char) );
	memset( mCurrKeyState, 0, NUM_KEYS*sizeof(char) );


	//initialize mouse stuff
	hr = mpDI->CreateDevice( GUID_SysMouse, &mpDeviceMouse, NULL );
	if( FAILED( hr )) return false;

	hr = mpDeviceMouse->SetDataFormat(&c_dfDIMouse); //data format
 	if( FAILED( hr )) { ReleaseInterface( mpDeviceMouse ); return false; }

	hr = mpDeviceMouse->SetCooperativeLevel( mhWnd, 
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if( FAILED( hr )) { ReleaseInterface( mpDeviceMouse ); return false; }

	memset(&mLastState, 0, sizeof(DIMOUSESTATE));
	memset(&mCurrState, 0, sizeof(DIMOUSESTATE));

	return true;
}

bool CInput::UpdateInput()
{
	HRESULT hr;
	//TCHAR debugMsg[100];

	//update keyboard
	memcpy(&mLastKeyState, &mCurrKeyState, (sizeof(char)*NUM_KEYS));
    hr = mpDeviceKeyboard->Poll(); 
    hr = mpDeviceKeyboard->GetDeviceState((sizeof(char)*NUM_KEYS),
		(LPVOID)&mCurrKeyState); 

    if( FAILED(hr) )
    { 
		hr = mpDeviceKeyboard->Acquire();
		if( FAILED( hr ) ) return false;

	    hr = mpDeviceKeyboard->Poll(); 
	    hr = mpDeviceKeyboard->GetDeviceState((sizeof(char)*NUM_KEYS),
			(LPVOID)&mCurrKeyState); 
		if( FAILED( hr ) ) return false;
    } 


	//update mouse
	memcpy(&mLastState, &mCurrState, sizeof(DIMOUSESTATE));
    hr = mpDeviceMouse->Poll(); 
	hr = mpDeviceMouse->GetDeviceState( sizeof(DIMOUSESTATE), 
		(void*)&mCurrState );

    if( FAILED(hr) )
    { 
		hr = mpDeviceMouse->Acquire();
		if( FAILED( hr ) ) return false;

	    hr = mpDeviceMouse->Poll(); 
	    hr = mpDeviceMouse->GetDeviceState( sizeof(DIMOUSESTATE),
			(void*)&mCurrState ); 
		if( FAILED( hr ) ) return false;
    }

	//StringCchPrintf(debugMsg, 100, L"%ld %ld %ld\n",mCurrState.lX, 
	//mCurrState.lY, mCurrState.lZ);
	//OutputDebugString(debugMsg);


	return true;

}


CInput::~CInput()
{
	if( mpDeviceKeyboard )
	{
		mpDeviceKeyboard->Unacquire();
		ReleaseInterface( mpDeviceKeyboard );

		mpDeviceMouse->Unacquire();
		ReleaseInterface(mpDeviceMouse);
	}

}
	
bool CInput::CheckKeyPressed(int key)
{
	return ( (mCurrKeyState[key] & 0x80) > 0 );
}

bool CInput::CheckKeyJustUp(int key)
{
	return ( (!(mCurrKeyState[key] & 0x80)) && (mLastKeyState[key] 
	& 0x80) );
}

bool CInput::CheckKeyUp(int key)
{
	return  (!(mCurrKeyState[key] & 0x80)) ;
}

	
bool CInput::MouseMoved()
{
	return (mCurrState.lX || mCurrState.lY || mCurrState.lZ);
}
	
bool CInput::MouseButtonPressed(bool& lb, bool& mb, bool& rb)
{
	lb = (mCurrState.rgbButtons[0] & 0x80) > 0;
	mb = (mCurrState.rgbButtons[1] & 0x80) > 0;
	rb = (mCurrState.rgbButtons[2] & 0x80) > 0;

	return (lb || mb || rb);
}

bool CInput::MouseButtonJustUp(bool& lb, bool& mb, bool& rb)
{
	lb = (!(mCurrState.rgbButtons[0] & 0x80)) 
		&& (mLastState.rgbButtons[0] & 0x80);
	
	mb = (!(mCurrState.rgbButtons[1] & 0x80)) 
		&& (mLastState.rgbButtons[1] & 0x80);
	
	rb = (!(mCurrState.rgbButtons[2] & 0x80)) 
		&& (mLastState.rgbButtons[2] & 0x80);

	return (lb || mb || rb);
}

bool CInput::MouseButtonUp(bool& lb, bool& mb, bool& rb)
{
	lb = (!(mCurrState.rgbButtons[0] & 0x80)) ;
	mb = (!(mCurrState.rgbButtons[1] & 0x80)) ;
	rb = (!(mCurrState.rgbButtons[2] & 0x80)) ;

	return (lb || mb || rb);
}



void CInput::GetDeltaMousePos(long& dx, long& dy, long& dz)
{
	dx = mCurrState.lX;
	dy = mCurrState.lY;
	dz = mCurrState.lZ;
}

//--------------------------------------------------------------------------------------
// File: SimpleSample.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include "resource.h"

#include "CMesh.h"
#include "CCuboid.h"
//#include "CCoordAxis.h"
#include "CTerrain.h"

#include "CInput.h"
#include "CCamera.h"

#include "ThirdPersonCam.h"

#include "CPlayer.h"
#include "Water.h"
#include "frustum.h"
#include "CSkyBox.h"
#include "CSphere.h"
#include "CSphereObject.h"
#include "CPlanet.h"
#include "CMarker.h"

#include "Data.h"




//#define DEBUG_VS   // Uncomment this line to debug D3D9 vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug D3D9 pixel shaders 


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
//CModelViewerCamera          g_Camera;               // A model viewing camera
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg             g_SettingsDlg;          // Device settings dialog
CDXUTTextHelper*            g_pTxtHelper = NULL;
CDXUTDialog                 g_HUD;                  // dialog for standard controls
CDXUTDialog                 g_SampleUI;             // dialog for sample specific controls

// Direct3D 9 resources
ID3DXFont*                  g_pFont9 = NULL;
ID3DXSprite*                g_pSprite9 = NULL;
//ID3DXEffect*                g_pEffect9 = NULL;

CInput* gInput = 0;
CCamera* gCam = 0;
CCamera *gEarthCam = 0;
CCamera *gMoonCam = 0;
CCamera *gCamTowardBangalore = 0;


CSkyBox * skyBox = 0;
CSphere *gSphere = 0;
CSphereObject *gSun = 0;
CPlanet *gEarth= 0;
CPlanet *gMoon = 0;

CPlanet *gAtmosphere = 0;

CMarker *gMarkerMoon = 0;
CMarker *gMarkerEarth = 0;



enum ECamMode
{
	FLYCAM		 = 0,
	CAM_ON_EARTH = 1,
	CAM_ON_MOON	 = 2,
	CAM_TOWARDS_BANGALORE = 3,
	FORCEDWORD = 0x7fffffff
};

bool bLockCursor = true;

#define NUMLIGHTS 3
//D3DLIGHT9 gLights[NUMLIGHTS];
D3DMATERIAL9 gTerrMat;
D3DLIGHT9 gLightFromSun;

float gLightPosSinAng = 0;
bool bMovePointLight = true;
bool bMoveSpotLight = true;

bool bDetailTextureOnOff = true;

//bool bCheckDiffuse = false;
//bool bCheckSpecular = false;
bool Spec=false;
bool Diff=false;


int gWinWidth = 1024;
int gWinHeight = 800;

ECamMode gCamMode = CAM_TOWARDS_BANGALORE;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN	    1
#define IDC_TOGGLEREF				2
#define IDC_CHANGEDEVICE			3
#define IDC_EXIT					9


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
						 void* pUserContext );
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );

bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
									 bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									void* pUserContext );
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
								   void* pUserContext );
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnD3D9LostDevice( void* pUserContext );
void CALLBACK OnD3D9DestroyDevice( void* pUserContext );

void InitApp();
void RenderText();

void LoadScene(LPDIRECT3DDEVICE9 pd3dDevice);
void DestroyScene();

void SetupSceneMaterialsAndLights(IDirect3DDevice9* pd3dDevice);
//void setDiffuse (bool bCheckDiffuse );
//void setSpecular (bool bCheckSpecular );
void changeSpecularity(  );

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// DXUT will create and use the best device (either D3D9 or D3D10) 
	// that is available on the system depending on which D3D callbacks are set below

	// Set DXUT callbacks
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( OnKeyboard );
	DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

	DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
	DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
	DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
	DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
	DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );

	gCam		= new CCamera(30.0f);
	gEarthCam	= new CCamera(30.0f);
	gMoonCam	= new CCamera(30.0f);
	gCamTowardBangalore = new CCamera ( 30.0f );

	InitApp();
	DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
	DXUTSetCursorSettings( true, true );
	DXUTCreateWindow( L"SimpleSample",0,0,0,0,0 );
	DXUTCreateDevice( true, gWinWidth, gWinHeight );

	gInput = new CInput(hInstance,DXUTGetHWND());

	if(false == gInput->Init())
		return 0;


	DXUTMainLoop(0,true,gInput); // Enter into the DXUT render loop

	DestroyScene();

	return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
	g_SettingsDlg.Init( &g_DialogResourceManager );
	g_HUD.Init( &g_DialogResourceManager );
	g_SampleUI.Init( &g_DialogResourceManager );

	g_HUD.SetCallback( OnGUIEvent ); int iY = 10;
	g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
	g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22, VK_F3 );
	g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

	g_HUD.AddButton( IDC_EXIT, L"EXIT (F12)", 35, iY += 24, 125, 22, VK_F12 );

	g_SampleUI.SetCallback( OnGUIEvent ); iY = 10;
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
	g_pTxtHelper->Begin();
	g_pTxtHelper->SetInsertionPos( 5, 5 );
	g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( true ) ); //DXUTIsVsyncEnabled()
	g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );
	g_pTxtHelper->DrawTextLine( L"Function Buttons" );
	g_pTxtHelper->DrawTextLine( L"================" );
	g_pTxtHelper->DrawTextLine( L"F1 - Lock/Unlock Cursor" );
	g_pTxtHelper->DrawTextLine( L"F5 - Point Light On/Off" );
	g_pTxtHelper->DrawTextLine( L"F6 - Spot Light On/Off" );
	g_pTxtHelper->DrawTextLine( L"F8 - Camera Pointing at Bangalore on Earth" );
	g_pTxtHelper->DrawTextLine( L"F9 - Flycam" );
	g_pTxtHelper->DrawTextLine( L"F10 - Camera on Earth Looking towards Sky" );
	g_pTxtHelper->DrawTextLine( L"F11 - Camera on Moon" );
	g_pTxtHelper->DrawTextLine( L"w/s a/d - Flycam Movements (F9)" );
	g_pTxtHelper->DrawTextLine( L"Mouse (if Unlocked - F1) Yaw/Pitch/Roll" );


	g_pTxtHelper->End();
}


//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
									 D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	// Skip backbuffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	// No fallback defined by this app, so reject any device that 
	// doesn't support at least ps2.0
	if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
		return false;

	return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	if( pDeviceSettings->ver == DXUT_D3D9_DEVICE )
	{
		IDirect3D9* pD3D = DXUTGetD3D9Object();
		D3DCAPS9 Caps;
		pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal, pDeviceSettings->d3d9.DeviceType, &Caps );


		//set the mutisample to 2 samples
	//	pDeviceSettings->d3d9.pp.MultiSampleType =  D3DMULTISAMPLE_2_SAMPLES;

		// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
		// then switch to SWVP.
		if( ( Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
			Caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
		{
			pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		// Debugging vertex shaders requires either REF or software vertex processing 
		// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
		if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
		{
			pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
			pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
			pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
#endif
#ifdef DEBUG_PS
		pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif
	}

	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if( s_bFirstTime )
	{
		s_bFirstTime = false;
		if( ( DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF ) ||
			( DXUT_D3D10_DEVICE == pDeviceSettings->ver &&
			pDeviceSettings->d3d10.DriverType == D3D10_DRIVER_TYPE_REFERENCE ) )
			DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
	}

	return true;
}




//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									void* pUserContext )
{
	HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
	V_RETURN( g_SettingsDlg.OnD3D9CreateDevice( pd3dDevice ) );

	V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Arial", &g_pFont9 ) );

	// Read the D3DX effect file
	//    WCHAR str[MAX_PATH];
	//    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif
	/*V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"SimpleSample.fx" ) );
	V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
	NULL, &g_pEffect9, NULL ) );*/

	// Setup the camera's view parameters
	D3DXVECTOR3 vecEye( 0.0f, 0.0f, 100.0f );
	D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
	D3DXVECTOR3 vecUp ( 0.0f, 1.0f, -0.0f );
	//g_Camera.SetViewParams( &vecEye, &vecAt );
	gCam->UpdateViewMatrixParams(vecEye, vecAt, vecUp,true);
	gEarthCam->UpdateViewMatrixParams(vecEye, vecAt, vecUp,true);
	gMoonCam->UpdateViewMatrixParams(vecEye, vecAt, vecUp,true);
	gCamTowardBangalore->UpdateViewMatrixParams(vecEye, vecAt, vecUp,true);


	LoadScene(pd3dDevice);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice,
								   const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );
	V_RETURN( g_SettingsDlg.OnD3D9ResetDevice() );

	if( g_pFont9 ) V_RETURN( g_pFont9->OnResetDevice() );
	//if( g_pEffect9 ) V_RETURN( g_pEffect9->OnResetDevice() );

	V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pSprite9 ) );
	g_pTxtHelper = new CDXUTTextHelper( g_pFont9, g_pSprite9, NULL, NULL, 15 );

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	//g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
	//g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );


	float fov = D3DX_PI / 4 ;
	float zn = 1.0f;
	float zf = 1000.f;

	gCam->UpdateProjMatrix( fov, fAspectRatio, zn, zf, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	gEarthCam->UpdateProjMatrix( fov, fAspectRatio, zn, zf, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	gMoonCam->UpdateProjMatrix( fov, fAspectRatio, zn, zf, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	gCamTowardBangalore->UpdateProjMatrix( fov, fAspectRatio, zn, zf, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
	g_HUD.SetSize( 170, 170 );
	g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 350 );
	g_SampleUI.SetSize( 170, 300 );

	SetupSceneMaterialsAndLights ( pd3dDevice );
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	// Update the camera's position based on user input 
	//g_Camera.FrameMove( fElapsedTime );
	//gInput->UpdateInput();

	if(gInput->CheckKeyJustUp(DIK_F1))
	{
		bLockCursor = !bLockCursor;
	}
	if(gInput->CheckKeyJustUp(DIK_F5))
	{
		bMovePointLight = !bMovePointLight;
	}
	if(gInput->CheckKeyJustUp(DIK_F6))
	{
		bMoveSpotLight = !bMoveSpotLight;
	}

	if(gInput->CheckKeyJustUp(DIK_F8))
	{
		gCamMode = CAM_TOWARDS_BANGALORE;
	}
	else if(gInput->CheckKeyJustUp(DIK_F9))
	{
		gCamMode = FLYCAM;
	}
	else if(gInput->CheckKeyJustUp(DIK_F10))
	{
		gCamMode = CAM_ON_EARTH;
	}
	else if(gInput->CheckKeyJustUp(DIK_F11))
	{
		gCamMode = CAM_ON_MOON;
	}

	if(bLockCursor)
	{
		SetCursorPos(gWinWidth/2, gWinHeight/2);
	}

	//if player is moving, only then Upadate his position on terrain and fp/tp cam params
	//if(gPlayer->IsInMotion()) { } -- did that but screen flickers...




	//update the pt light and spot light pos
	gLightPosSinAng += fElapsedTime;

	if(gLightPosSinAng > 2.0f*D3DX_PI)
		gLightPosSinAng -= 2.0f*D3DX_PI;
	/*
	// Move Point light
	if(bMovePointLight)
	gLights[1].Position.x = 30.0f*sin(gLightPosSinAng); 

	// Move Spot Loght
	if(bMoveSpotLight)
	gLights[2].Position.z = 30.0f*sin(gLightPosSinAng);
	*/

	D3DXMATRIXA16 temp;
	gSun->GetWorldMatrixWithOutScale ( temp );
	gEarth->SetParentWorldMat ( temp );
	gEarth->UpdatePosition ( fElapsedTime );//, temp   );
	gAtmosphere->SetParentWorldMat ( temp );
	gAtmosphere->UpdatePosition ( fElapsedTime );//, temp   );

	gAtmosphere->GetWorldMatrix ( temp );
	gMarkerEarth->UpdateWorldMatrix ( temp );

	gMoon->SetParentWorldMat ( temp );
	gMoon->UpdatePosition ( fElapsedTime );

	gMoon->GetWorldMatrix ( temp );
	gMarkerMoon->UpdateWorldMatrix ( temp );

	if(gCamMode == FLYCAM)
	{
		gCam->UpdateParamsOnInput(gInput, fElapsedTime);
	}

	D3DXVECTOR3 bangCoord;
	D3DXVECTOR3 vecEye, lookAt, normal;
	D3DXMATRIXA16 worldMat;
	D3DXVECTOR4 tempVec4;
	if ( gCamMode == CAM_ON_EARTH )
	{
		bangCoord = gSphere->GetBangCoord ( );
		vecEye = bangCoord;
		gEarth->GetWorldMatrix ( worldMat );
		float scale = gEarth->GetScale();
		vecEye *= scale;

		normal =  vecEye - D3DXVECTOR3 (0,0,0);
		normal = 2 * normal;

		D3DXVec3Transform ( &tempVec4, &vecEye, &worldMat );
		vecEye = D3DXVECTOR3 ( tempVec4.x, tempVec4.y, tempVec4.z);

		D3DXVec3Transform ( &tempVec4, &normal, &worldMat );
		normal = D3DXVECTOR3 ( tempVec4.x, tempVec4.y, tempVec4.z);
		

		gEarthCam->UpdateViewMatrixParams ( vecEye, normal, D3DXVECTOR3 ( 0, 1, 0) );
	}
	else if ( gCamMode == CAM_ON_MOON)
	{
		bangCoord = gSphere->GetBangCoord ( );
		vecEye = bangCoord;
		gMoon->GetWorldMatrix ( worldMat );
		float scale = gMoon->GetScale();
		vecEye *= scale;

		normal =  vecEye - D3DXVECTOR3 (0,0,0);
		normal = 2 * normal;

		D3DXVec3Transform ( &tempVec4, &vecEye, &worldMat );
		vecEye = D3DXVECTOR3 ( tempVec4.x, tempVec4.y, tempVec4.z);

		D3DXVec3Transform ( &tempVec4, &normal, &worldMat );
		normal = D3DXVECTOR3 ( tempVec4.x, tempVec4.y, tempVec4.z);
		

		gMoonCam->UpdateViewMatrixParams ( vecEye, normal, D3DXVECTOR3 ( 0, 1, 0) );
	}
	else if ( gCamMode == CAM_TOWARDS_BANGALORE)
	{
		bangCoord = gSphere->GetBangCoord ( );
		lookAt = bangCoord;
		gEarth->GetWorldMatrix ( worldMat );
		float scale = gEarth->GetScale();
		lookAt *= scale;

		normal =  lookAt - D3DXVECTOR3 (0,0,0);
		normal = 10 * normal;

		D3DXVec3Transform ( &tempVec4, &lookAt, &worldMat );
		vecEye = D3DXVECTOR3 ( tempVec4.x, tempVec4.y, tempVec4.z);

		D3DXVec3Transform ( &tempVec4, &normal, &worldMat );
		normal = D3DXVECTOR3 ( tempVec4.x, tempVec4.y, tempVec4.z);
		

		gCamTowardBangalore->UpdateViewMatrixParams ( normal, vecEye, D3DXVECTOR3 ( 0, 1, 0) );
		gCam->UpdateViewMatrixParams( normal, vecEye, D3DXVECTOR3 ( 0, 1, 0) );
	}


}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;
	D3DXMATRIXA16 mWorldIdentity;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;



	// If the settings dialog is being shown, then render it instead of rendering the app's scene
	if( g_SettingsDlg.IsActive() )
	{
		g_SettingsDlg.OnRender( fElapsedTime );
		return;
	}

	pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	//pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,  TRUE );
	pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW ); //D3DCULL_NONE); //
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	// Set Specularity On
	pd3dDevice->SetRenderState( D3DRS_SPECULARMATERIALSOURCE,  D3DMCS_MATERIAL);
	if ( Spec )
		pd3dDevice->SetRenderState ( D3DRS_SPECULARENABLE, TRUE );
	else pd3dDevice->SetRenderState ( D3DRS_SPECULARENABLE, FALSE );


	// Clear the render target and the zbuffer 
	//	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ) , 1.0f, 0 ) ); // D3DCOLOR_ARGB( 255, 255, 255, 255 )
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ) , 1.0f, 0 ) ); // D3DCOLOR_ARGB( 255, 255, 255, 255 )

	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{

		pd3dDevice->SetMaterial ( &gTerrMat );
		pd3dDevice->SetLight(0, &gLightFromSun);


		//		gSphere->SetFVFVBIBOnDevice(pd3dDevice);
		//		gSphere->Render ( pd3dDevice );
		V (gSun->Render(pd3dDevice) );

		pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
		pd3dDevice->LightEnable ( 0, TRUE );
		pd3dDevice->SetMaterial ( &gTerrMat );

		V ( gEarth->Render(pd3dDevice) );
		V ( gAtmosphere->Render ( pd3dDevice ) );

		V ( gMoon->Render ( pd3dDevice ) );
		pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);

		// Render Markers
		V ( gMarkerEarth->SetFVF ( pd3dDevice ));
		V ( gMarkerEarth->SetStreamSource ( pd3dDevice, 0, 0 ));
		V ( gMarkerEarth->Render ( pd3dDevice ) );

		V ( gMarkerMoon->SetFVFVBIBOnDevice ( pd3dDevice ));
		V ( gMarkerMoon->Render ( pd3dDevice ) );


		D3DXMatrixIdentity(&mWorldIdentity);

		if(gCamMode == FLYCAM)
		{
			mProj = *gCam->GetProjMatrix(); //*g_Camera.GetProjMatrix();
			mView = *gCam->GetViewMatrix(); //*g_Camera.GetViewMatrix();
		}
		else if ( gCamMode == CAM_ON_EARTH )
		{
			mProj = *gEarthCam->GetProjMatrix(); 
			mView = *gEarthCam->GetViewMatrix(); 
		}
		else if ( gCamMode == CAM_ON_MOON )
		{
			mProj = *gMoonCam->GetProjMatrix(); 
			mView = *gMoonCam->GetViewMatrix(); 
		}
		else if ( gCamMode == CAM_TOWARDS_BANGALORE )
		{
			mProj = *gCamTowardBangalore->GetProjMatrix(); 
			mView = *gCamTowardBangalore->GetViewMatrix(); 
		}

		mWorldViewProjection = mWorldIdentity * mView * mProj;

		// Update the effect's variables.  Instead of using strings, it would 
		// be more efficient to cache a handle to the parameter by calling 
		// ID3DXEffect::GetParameterByName
		/*V( g_pEffect9->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
		V( g_pEffect9->SetMatrix( "g_mWorld", &mWorld ) );
		V( g_pEffect9->SetFloat( "g_fTime", ( float )fTime ) );*/


		// Setup our matrices
		pd3dDevice->SetTransform( D3DTS_VIEW, &mView );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &mProj );



		DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); // These events are to help PIX identify what the code is doing

		//render coordaxis
		pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldIdentity );
		//		V(gCAx->SetFVFVBIBOnDevice(pd3dDevice));
		//		V(gCAx->Render(pd3dDevice));

		/*//render cube
		V(gCubeAc->SetFVFVBIBOnDevice(pd3dDevice));
		V(gCubeAc->Render(pd3dDevice));*/

		//render player
		//		V(gPlayer->Render(pd3dDevice));

		// Rendering Terrain

		pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldIdentity );
		//		V(gTerr->SetFVFVBIBOnDevice(pd3dDevice));



		//setDiffuse ( bCheckDiffuse );
		//setSpecular ( bCheckSpecular );





		// Lightning Effect
		/*

		for ( int i = 0; i < NUMLIGHTS ; i++)
		{
		pd3dDevice->SetLight ( i, &gLights [ i ] );
		pd3dDevice->LightEnable (i, true );
		} 
		*/

		/*  
		if(Diff)
		{
		gLights[0].Diffuse.a = gLights[0].Diffuse.r = 1.0f;
		gLights[0].Diffuse.g = gLights[0].Diffuse.b = 1.0f;

		}
		else
		{
		gLights[0].Diffuse.a = gLights[0].Diffuse.r = 0.0f;
		gLights[0].Diffuse.g = gLights[0].Diffuse.b = 0.0f;
		}

		if(Spec)
		{
		gLights[0].Specular.a = gLights[0].Specular.r = 1.0f;
		gLights[0].Specular.g = gLights[0].Specular.b = 0.0f;
		}
		else
		{
		gLights[0].Specular.a = gLights[0].Specular.r = 0.0f;
		gLights[0].Specular.g = gLights[0].Specular.b = 0.0f;
		}
		*/


		//		  V(gTerr->Render(pd3dDevice));

		pd3dDevice->SetRenderState( D3DRS_LIGHTING,FALSE) ;


		// Render Frustum

		//		gFrustum->Render ( pd3dDevice );

		// Render water

		pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldIdentity );
		//		V ( gWater->Render ( pd3dDevice ) );


		// render SkyBox..
		pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldIdentity );
		skyBox->SetFVFVBIBOnDevice(pd3dDevice);
		skyBox->Render(pd3dDevice);





		RenderText();
		V( g_HUD.OnRender( fElapsedTime ) );
		V( g_SampleUI.OnRender( fElapsedTime ) );


		DXUT_EndPerfEvent();

		V( pd3dDevice->EndScene() );
	}
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
						 void* pUserContext )
{
	// Pass messages to dialog resource manager calls so GUI state is updated correctly
	*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	// Pass messages to settings dialog if its active
	if( g_SettingsDlg.IsActive() )
	{
		g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
		return 0;
	}

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;
	*pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	// Pass all remaining windows messages to camera so it can respond to user input
	//g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

	return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case IDC_TOGGLEFULLSCREEN:
		DXUTToggleFullScreen(); break;
	case IDC_TOGGLEREF:
		DXUTToggleREF(); break;
	case IDC_CHANGEDEVICE:
		g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
	case IDC_EXIT:
		PostQuitMessage (0 );
	}
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D9LostDevice();
	g_SettingsDlg.OnD3D9LostDevice();
	if( g_pFont9 ) g_pFont9->OnLostDevice();
	//if( g_pEffect9 ) g_pEffect9->OnLostDevice();
	SAFE_RELEASE( g_pSprite9 );
	SAFE_DELETE( g_pTxtHelper );
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D9DestroyDevice();
	g_SettingsDlg.OnD3D9DestroyDevice();
	//SAFE_RELEASE( g_pEffect9 );
	SAFE_RELEASE( g_pFont9 );
}


void SetupSceneMaterialsAndLights(IDirect3DDevice9* pd3dDevice)
{
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE);


	DWORD maxLights;
	D3DCAPS9 deviceCaps;
	pd3dDevice->GetDeviceCaps (&deviceCaps);
	maxLights = deviceCaps.MaxActiveLights; //just to check

	ZeroMemory(&gTerrMat, sizeof(D3DMATERIAL9));
	gTerrMat.Diffuse.a = 1.0f;
	gTerrMat.Diffuse.r = 1.0f;
	gTerrMat.Diffuse.g = 1.0f;
	gTerrMat.Diffuse.b = 1.0f;

	gTerrMat.Specular.a= 1.0f;
	gTerrMat.Specular.r= 1.0f;
	gTerrMat.Specular.g= 1.0f;
	gTerrMat.Specular.b= 1.0f;

	gTerrMat.Power = 0.0f;

	//gTerrMat.Ambient.a = gTerrMat.Ambient.r = gTerrMat.Ambient.g = gTerrMat.Ambient.b = 1.0f;

	//gTerrMat.Emissive.a = gTerrMat.Emissive.r = gTerrMat.Emissive.g = 1.0f;
	//gTerrMat.Emissive.b = 0.0f;
	//	ZeroMemory(&gLights, NUMLIGHTS*sizeof(D3DLIGHT9));
	ZeroMemory(&gLightFromSun, sizeof(D3DLIGHT9));

	gLightFromSun.Type = D3DLIGHT_POINT;
	gLightFromSun.Attenuation0 = 1.0f;
	gLightFromSun.Attenuation1 = 0.0f;
	gLightFromSun.Attenuation2 = 0.0f;
	gLightFromSun.Diffuse.a = gLightFromSun.Diffuse.r = gLightFromSun.Diffuse.g = gLightFromSun.Diffuse.b = 10.0f;
	gLightFromSun.Range = 1000.0f;
	gLightFromSun.Specular.a = gLightFromSun.Specular.r = gLightFromSun.Specular.g = gLightFromSun.Specular.b = 1.0f;
	gLightFromSun.Position = D3DXVECTOR3 (0, 0, 0 );

	/*
	//Directional
	gLights[0].Type = D3DLIGHT_DIRECTIONAL;
	gLights[0].Direction = D3DXVECTOR3(-0.57735f,-0.57735f,-0.57735f);
	gLights[0].Diffuse.a = gLights[0].Diffuse.r = 1.0f;
	gLights[0].Diffuse.g = gLights[0].Diffuse.b = 1.0f;

	gLights[0].Specular.a = gLights[0].Specular.r = 0.0f;
	gLights[0].Specular.g = gLights[0].Specular.b = 0.0f;


	//point 
	gLights[1].Type = D3DLIGHT_POINT;
	gLights[1].Attenuation0 = 1.0f;
	gLights[1].Attenuation1 = 0.02f;
	gLights[1].Attenuation2 = 0.002f;

	gLights[1].Diffuse.a = gLights[1].Diffuse.r = gLights[1].Diffuse.g = 0.0f;
	gLights[1].Diffuse.b = 1.0f;

	gLights[1].Range = 70.0f;
	gLights[1].Specular.a = gLights[1].Specular.r = 0.0f;
	gLights[1].Specular.g = 0.0f;
	gLights[1].Specular.b = 1.0f;
	gLights[1].Position = D3DXVECTOR3(0.0f, 10.0f, 1.0f);



	//spot
	gLights[2].Type = D3DLIGHT_SPOT;
	gLights[2].Attenuation0 = 1.0f;
	gLights[2].Attenuation1 = 0.02f;
	gLights[2].Attenuation2 = 0.002f;
	gLights[2].Diffuse.a = gLights[2].Diffuse.r = gLights[2].Diffuse.b = 0.0f;
	gLights[2].Diffuse.g = 1.0f;
	gLights[2].Range = 70.0f;
	//gLights[2].Specular.a
	//= gLights[2].Specular.r = gLights[2].Specular.g = gLights[2].Specular.b = 0.5f;
	gLights[2].Position = D3DXVECTOR3(0.0f, 15.0f, 1.0f);

	D3DXVECTOR3 tempDir = D3DXVECTOR3(0.0f,-0.57735f,-0.57735f); 
	D3DXVec3Normalize(&tempDir, &tempDir);
	gLights[2].Direction = tempDir;
	gLights[2].Falloff = 1.0f;
	gLights[2].Phi = D3DXToRadian(60);
	gLights[2].Theta = D3DXToRadian(30);
	*/

}




void LoadScene(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	float fEarthRadius = 1.0f;
	float fEarthSunDistance = 550.0f;
	float fEarthRevolutionTime = 365.0f;
	float fEarthRotationTime = 1.0f;
	
	// skybox construct
	float fSkyBoxLength = 1500.0f;
	skyBox=new CSkyBox(D3DXVECTOR3(-fSkyBoxLength/2,-fSkyBoxLength/2,-fSkyBoxLength/2),D3DXVECTOR3(fSkyBoxLength/2,fSkyBoxLength/2,fSkyBoxLength/2));
	skyBox->Construct(pd3dDevice);

	gSphere =  new CSphere ( "Data/latlon.jpg", 12.0f, 180.0 + 77.0f);
	//gSphere =  new CSphere ( "Data/latlon.jpg", 12.77f, 180.0 + 73.98f);
	
	gSphere->Construct ( pd3dDevice );

	gSun = new CSphereObject (gSphere, fEarthRadius * 49.1f, "Data/suntex1.bmp");
	gSun->Construct ( pd3dDevice );

	gEarth = new CPlanet ( gSphere, fEarthSunDistance, fEarthRevolutionTime, fEarthRotationTime, fEarthRadius , "Data/earthtex.dds");
	gEarth->Construct ( pd3dDevice );
	gEarth->setTilt ( 23.4f);

	gAtmosphere = new CPlanet ( gSphere, fEarthSunDistance , fEarthRevolutionTime, fEarthRotationTime * -5.5f, fEarthRadius * 1.625f, "Data/earth_clouds.dds");
	gAtmosphere->Construct ( pd3dDevice );
	gAtmosphere->SetAtmosphere ( true );

	gMoon = new CPlanet ( gSphere, 30, 27, 27, fEarthRadius / 3.67f, "Data/MoonMap_2500x1250.jpg");
	gMoon->Construct ( pd3dDevice );
	gMoon->setTilt ( 35.145f );

	gMarkerEarth = new CMarker ( D3DXVECTOR3 ( -2, 0, 0),D3DXVECTOR3 ( 2, 5, 0), "Data/earthlocatortex.jpg" );
	gMarkerEarth->Construct ( pd3dDevice );

	gMarkerMoon = new CMarker ( D3DXVECTOR3 ( -2,0, 0),D3DXVECTOR3 ( 2,5, 0),"Data/moonlocatortex.jpg");
	gMarkerMoon->Construct ( pd3dDevice );
	

}

void DestroyScene()
{

	delete skyBox;
	delete gSphere;
	delete gSun;
	delete gEarth;
	delete gAtmosphere;
	delete gMoon;

	delete gMarkerEarth;
	delete gMarkerMoon;

	delete gInput;
	delete gCam;
	delete gEarthCam;
	delete gMoonCam;
	delete gCamTowardBangalore;
}
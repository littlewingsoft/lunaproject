//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Empty starting point for new Direct3D applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#include "resource.h"
//#include "gameswf.h"
//#include "gswf.h"
#include "zflash.h"

//INFOGSWF info;
DWORD* pkBuffer = 0;
ID3DXSprite* g_pkSprite = 0;
IDirect3DTexture9* g_pTexture = 0;
IDirect3DSurface9* g_pDirtySurface=0;
const int FLASH_W =1600;
const int FLASH_H =1200;

UINT AdapterToUse=D3DADAPTER_DEFAULT; 
D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

//--------------------------------------------------------------------------------------
// Rejects any devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Typically want to skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
	for (UINT Adapter=0;Adapter< pD3D->GetAdapterCount();Adapter++) 
	{ 
		D3DADAPTER_IDENTIFIER9 Identifier; 
		HRESULT Res; 
		Res = pD3D->GetAdapterIdentifier(Adapter,0,&Identifier); 
		if (strstr(Identifier.Description,"PerfHUD") != 0) 
		{ 
			AdapterToUse=Adapter; 
			DeviceType=D3DDEVTYPE_REF; 
			wchar_t buff[64]={0,};
			wsprintf( buff, L"Nv :%d", AdapterToUse );
			//MessageBox(0,buff, L"Profile",MB_OK );
			break; 
		} 
	}


    if( FAILED( pD3D->CheckDeviceFormat( AdapterToUse, DeviceType,//pCaps->DeviceType, pCaps->AdapterOrdinal
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
	IDirect3D9* pD3D = DXUTGetD3DObject(); 

	pDeviceSettings->DeviceType = DeviceType;
    return true;
}



//--------------------------------------------------------------------------------------
// Create any D3DPOOL_MANAGED resources here 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
//	gameswf::render_handler* pkHandler = gameswf::create_render_handler_d3d( pd3dDevice );
//	gameswf::set_render_handler( pkHandler );

//	DestroyGSWF();
//	CreateGSWF( pd3dDevice, info );
	
	ZFLASH::Init( 0x60000 );

	D3DXCreateSprite( pd3dDevice, &g_pkSprite );
	
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3DPOOL_DEFAULT resources here 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	g_pkSprite->OnResetDevice();
	
	SAFE_RELEASE( g_pTexture );
	HRESULT  hr = DXUTGetD3DDevice()->CreateTexture( 2048,2048, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT , &g_pTexture, NULL);//D3DPOOL_DEFAULT

	SAFE_RELEASE( g_pDirtySurface );

	DXUTGetD3DDevice()->CreateOffscreenPlainSurface( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height, 
		D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &g_pDirtySurface,0 );//D3DPOOL_SCRATCH
	

	pd3dDevice->EvictManagedResources();

	pkBuffer  = ZFLASH::LoadMovie( L"./test.swf", pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	
//    wchar_t* url = L"http://pann.nate.com/images/movieUpload/PannPlayer.swf?mov_id=3578117_1486157&vs_id=pann&vs_keys=3578117_1486157&autoPlay=0&externalPlayer=1&scrap=1" ;
//	pkBuffer  = ZFLASH::LoadMovie( url, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	

	ZFLASH::Loop(true);
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{

	HRESULT hr = D3D_OK;
	hr = pd3dDevice->TestCooperativeLevel();
	if( pkBuffer && hr == D3D_OK && ZFLASH::DrawFrame() && g_pTexture )
	{
/*		D3DLOCKED_RECT lrt;
		HRESULT hr = g_pTexture->LockRect( 0, &lrt,NULL, D3DLOCK_DISCARD);
		if( hr == S_OK )
		{
			BYTE* pkDestBegin = (BYTE*)lrt.pBits;

			int lineByte = ZFLASH::GetWidth()*sizeof(DWORD);
			for( int y = ZFLASH::GetHeight()-1; y>=0; y--)
			{
				DWORD* pkSrc = (DWORD*) &pkBuffer[ ZFLASH::GetWidth() * y ];
				memcpy( pkDestBegin, pkSrc,  lineByte );//
				pkDestBegin += lrt.Pitch;
			}


			//memset( lrt.pBits,0xffffffff,lrt.Pitch* m_pTexture->Height );//* sizeof(DWORD) 
			g_pTexture->UnlockRect(0);
		}
*/


		D3DLOCKED_RECT lrt;
		hr = g_pDirtySurface->LockRect( &lrt, NULL, D3DLOCK_DISCARD );
		if( hr == S_OK )
		{
			BYTE* pkDestBegin = (BYTE*)lrt.pBits;
			//memcpy( pkDestBegin, pkBuffer,  ZFLASH::GetWidth()* ZFLASH::GetHeight()*4 );//
			int lineByte = ZFLASH::GetWidth()*sizeof(DWORD);
			for( int y = ZFLASH::GetHeight()-1; y>=0; y--)
			{
				DWORD* pkSrc = (DWORD*) &pkBuffer[ ZFLASH::GetWidth() * y ];
				memcpy( pkDestBegin, pkSrc,  lineByte );//
				pkDestBegin += lrt.Pitch;
			}

			g_pDirtySurface->UnlockRect();
		}


	IDirect3DSurface9* pkDestSurface = 0;
	g_pTexture->GetSurfaceLevel( 0, &pkDestSurface );
	pd3dDevice->UpdateSurface( g_pDirtySurface,NULL,  pkDestSurface,NULL );

	SAFE_RELEASE( pkDestSurface );
		
	}

	
	//UpdateGSWF( fElapsedTime );
}


//--------------------------------------------------------------------------------------
// Render the scene 
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0) );

	D3DXVECTOR3 pos(0,0,0);
    // Render the scene

    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		//RenderGSWF();
		RECT srcRt={ 0,0,ZFLASH::GetWidth(), ZFLASH::GetHeight() };
		g_pkSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
		g_pkSprite->Draw( g_pTexture, &srcRt,0,&pos, 0xffffffff );
		g_pkSprite->End();
		
			//pkBuffer

        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	//MsgProcGSWF( uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// Release resources created in the OnResetDevice callback here 
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
	SAFE_RELEASE( g_pTexture );
	g_pkSprite->OnLostDevice();
}


//--------------------------------------------------------------------------------------
// Release resources created in the OnCreateDevice callback here
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	SAFE_RELEASE(g_pDirtySurface);
	SAFE_RELEASE( g_pTexture );
	SAFE_RELEASE( g_pkSprite );
	ZFLASH::Exit();
}



//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 368);
#endif
//	info.infile = "test_button_functions.swf";

    // Set the callback functions
    DXUTSetCallbackDeviceCreated( OnCreateDevice );
    DXUTSetCallbackDeviceReset( OnResetDevice );
    DXUTSetCallbackDeviceLost( OnLostDevice );
    DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameRender( OnFrameRender );
    DXUTSetCallbackFrameMove( OnFrameMove );
   
    // TODO: Perform any application-level initialization here


    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"EmptyProject" );

   IDirect3D9* pD3D = DXUTGetD3DObject(); 
	for (UINT Adapter=0;Adapter< pD3D->GetAdapterCount();Adapter++) 
	{ 
		D3DADAPTER_IDENTIFIER9 Identifier; 
		HRESULT Res; 
		Res = pD3D->GetAdapterIdentifier(Adapter,0,&Identifier); 
		if (strstr(Identifier.Description,"PerfHUD") != 0) 
		{ 
			AdapterToUse=Adapter; 
			DeviceType=D3DDEVTYPE_REF; 
			wchar_t buff[64]={0,};
			wsprintf( buff, L"Nv :%d", AdapterToUse );
			//MessageBox(0,buff, L"Profile",MB_OK );
			break; 
		} 
	}

    DXUTCreateDevice( AdapterToUse, false, FLASH_W, FLASH_H, IsDeviceAcceptable, ModifyDeviceSettings );
	
    // Start the render loop
    DXUTMainLoop();


    // TODO: Perform any application-level cleanup here


    return DXUTGetExitCode();
}



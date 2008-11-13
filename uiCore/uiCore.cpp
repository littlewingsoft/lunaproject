#include "StdAfx.h"
#include "resource.h"

namespace uiCore
{

static IDirect3DDevice9* g_pkD3DDev = 0;
static IDirect3DTexture9*  g_pkWaterMarkTexture = 0; //
static ID3DXSprite*          g_pkSprite = 0;          // Sprite used for drawing
static ID3DXFont*			g_pkFont =0 ;
DWORD  g_dwKillTimer = 0;

void SetDevice( IDirect3DDevice9* pkDev )
{
	assert( pkDev && "pkDev" );

	g_pkD3DDev = pkDev; 
	HRESULT hr = D3DXCreateSprite( pkDev, &g_pkSprite);
	if( hr != D3D_OK )
	{
		MessageBox( 0, L"mainEntry::SetDevice::D3DXCreateSprite", L"error", MB_OK);
		return;
	}

	D3DXCreateFont( pkDev, -12, 0, 0, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"System", &g_pkFont );

	HMODULE hmod = LoadLibrary( L"../sdk/uiRes.dll" );

	D3DXCreateTextureFromResource( pkDev, hmod , MAKEINTRESOURCE(IDB_BITMAP1),&g_pkWaterMarkTexture);

	if( hmod == NULL || g_pkWaterMarkTexture == NULL )
	{//timer발동.불법사용으로 간주하고 무조건다운
		g_dwKillTimer = GetTickCount();
		
	}

	BOOL bREt = FreeLibrary(hmod);
	


}

void Release()
{
	SAFE_RELEASE( g_pkFont );
	SAFE_RELEASE( g_pkSprite )
	SAFE_RELEASE( g_pkWaterMarkTexture );
}

int LoadLua(const std::wstring& LuaFileName)
{
	/*
		mainEntry가 되는 lua 파일을 로딩후 텍스쳐 로딩과 갖가지 작업을 여기서 시작함.
	*/
	//ui프로젝트로딩.
	
	//textureList 로딩.

	return 0;
}

void Render(float fElapsedTime)
{
	if( g_pkD3DDev&&g_pkD3DDev->TestCooperativeLevel() == S_OK )
	{
		if( g_pkSprite )
		{
			if( g_pkSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE ) == S_OK )
			{
				if( g_pkWaterMarkTexture )
				g_pkSprite->Draw( g_pkWaterMarkTexture, 0,0, &D3DXVECTOR3(1024-64,768-32,0),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));

				if( g_dwKillTimer != 0 )
				{
				RECT rt= {0, 0, 1000, 100};
				g_pkFont->DrawText( g_pkSprite, L"bye bye~",-1,&rt,DT_LEFT,0xFFFF0000);
				}
				g_pkSprite->End();
			}
		}

	}
}

//lua로 시간, 마우스입력,키보드입력 이벤트 전달.
//lua에서 넘겨져온 이벤트 콜벡 처리.
//에니메이션,이벤트콜벡호출.

void Process( float fElapsedTime )
{
	if( g_dwKillTimer != 0 ) // 허접하지만 임시로 부정 사용자를 방지함. 만일 dll을 변조한다면,,?
	{
		if( GetTickCount() - g_dwKillTimer > 3000 )
		{

			MessageBox(0, L"you must don't Cracking this uiEngine",L"BYEBYE",MB_OK);
			exit(-1);
		}
	}

}

//int MainLoop(float fElapsedTime)
//{
//	Process( fElapsedTime );
//
//	Render( fElapsedTime );
//	//d3d로 rhw로 출력하기. 일단 Sprite 를 쓰자.
//	return 0;
//}

}
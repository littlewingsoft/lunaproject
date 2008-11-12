#include "StdAfx.h"
#include "resource.h"

namespace uiCore
{

static ID3DXSprite*          g_pkSprite = 0;          // Sprite used for drawing
static IDirect3DDevice9* g_pkD3DDev = 0;
static IDirect3DTexture9*  g_pkWaterMarkTexture = 0; //


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

	HMODULE hmod = GetModuleHandle( L"uiCore_r.lib");
//	D3DXCreateTextureFromResource( pkDev, hmod , MAKEINTRESOURCE(IDB_BITMAP1),&g_pkWaterMarkTexture);
	D3DXCreateTextureFromFile( pkDev, L"watermark.bmp",&g_pkWaterMarkTexture);
	

}

void Release()
{
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
		if( g_pkSprite && g_pkWaterMarkTexture)
		{
			if( g_pkSprite->Begin( 0 ) == S_OK )
			{
				g_pkSprite->Draw( g_pkWaterMarkTexture, 0,0, &D3DXVECTOR3(1024-64,768-32,0),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));
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
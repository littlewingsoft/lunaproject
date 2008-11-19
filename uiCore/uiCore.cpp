#include "StdAfx.h"
#include "resource.h"

extern "C" 
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
};

#include "lua_tinker.h"

using namespace std;

namespace uiCore
{

namespace
{
	IDirect3DDevice9*	g_pkDev = 0;
	IDirect3DTexture9*  g_pkWaterMarkTexture = 0; //
	ID3DXSprite*        g_pkSprite = 0;          // Sprite used for drawing
	ID3DXFont*			g_pkFont =0 ;
	lua_State*			g_luaState=0 ;
	DWORD				g_dwKillTimer = 0;
}


void printOut_native(const char* str)
{
	::OutputDebugString( L"18" );//str.c_str()
	::OutputDebugString( L"\n" );
}

void InitLua()
{
	g_luaState = lua_open();

	// Lua �⺻ �Լ����� �ε��Ѵ�.- print() ���
	luaopen_base(g_luaState);

	// sample1.lua ������ �ε�/�����Ѵ�.

	// LuaTinker �� �̿��ؼ� �Լ��� ����Ѵ�.
	lua_tinker::def( g_luaState, "printOut_native", printOut_native );

	lua_tinker::dofile(g_luaState, "../sdk/uiCore.lua");

	lua_tinker::call<void>( g_luaState, "printOut_native", "a1818" );


}
	// sample1.lua �� �Լ��� ȣ���Ѵ�.
	

void UnInitLua()
{

	// ���α׷� ����
	lua_close(g_luaState);
}


void SetDevice( IDirect3DDevice9* pkDev )
{
	assert( pkDev && "pkDev" );

	g_pkDev = pkDev; 
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
	{//timer�ߵ�.�ҹ�������� �����ϰ� �����Ǵٿ�
		g_dwKillTimer = GetTickCount();
		
	}

	BOOL bREt = FreeLibrary(hmod);
	
	InitLua();

}

std::map< std::wstring,IDirect3DTexture9* > g_texList;

/*
	texture �� 2D ���̹Ƿ� ���͸��� ����. mipMap�� �� �⺻���� �ɼ����θ� �ε���.
*/
void LoadTexture( const std::wstring& fName  )
{
	IDirect3DTexture9* pkTexture=0;
	D3DXCreateTextureFromFileEx( g_pkDev, fName.c_str(),D3DX_DEFAULT,D3DX_DEFAULT,1, 
									0, D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
									0,0,0, &pkTexture );
	g_texList[fName]= pkTexture;
}

void ReleaseTextureAll()
{
	std::map< wstring,IDirect3DTexture9* >::iterator it = g_texList.begin();
	while( it != g_texList.end() )
	{
		IDirect3DTexture9* pkTex = (*it).second;
		pkTex->Release();
		it++;
	}

	g_texList.clear();
}

void Render(float fElapsedTime)
{
	if( g_pkDev&& g_pkDev->TestCooperativeLevel() == S_OK )
	{
		if( g_pkSprite )
		{
			if( g_pkSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE ) == S_OK )
			{
				if( g_pkWaterMarkTexture )
				g_pkSprite->Draw( g_pkWaterMarkTexture, 0,0, &D3DXVECTOR3(1024-64,768-32,0),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));
				//if( g_dwKillTimer != 0 )
				//{
				//RECT rt= {0, 0, 1000, 100};
				//g_pkFont->DrawText( g_pkSprite, L"bye bye~",-1,&rt,DT_LEFT,0xFFFF0000);
				//}
				g_pkSprite->End();
			}
		}

	}
}

void CheckInvalidUsing()
{
	if( g_dwKillTimer != 0 ) // ���������� �ӽ÷� ���� ����ڸ� ������. ���� dll�� �����Ѵٸ�,,?
	{
		if( GetTickCount() - g_dwKillTimer > 3000 )
		{
			MessageBox(0, L"you must don't Cracking this uiEngine",L"BYEBYE",MB_OK);
			exit(-1);
		}
	}

}


//lua�� �ð�, ���콺�Է�,Ű�����Է� �̺�Ʈ ����.
//lua���� �Ѱ����� �̺�Ʈ �ݺ� ó��.
//���ϸ��̼�,�̺�Ʈ�ݺ�ȣ��.

void Process( float fElapsedTime )
{
	CheckInvalidUsing();
	

}


void Release()
{
	UnInitLua();
	SAFE_RELEASE( g_pkWaterMarkTexture );
	ReleaseTextureAll();

	SAFE_RELEASE( g_pkFont );
	SAFE_RELEASE( g_pkSprite )
	
}

}
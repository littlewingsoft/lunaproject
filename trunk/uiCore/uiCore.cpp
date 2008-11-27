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

namespace localScope
{
	HWND				g_hWnd;
	IDirect3DDevice9*	g_pkDev = 0;
	IDirect3DTexture9*  g_pkWaterMarkTexture = 0; //
	ID3DXSprite*        g_pkSprite = 0;          // Sprite used for drawing
	ID3DXFont*			g_pkFont =0 ;
	lua_State*			g_luaState=0 ;
	DWORD				g_dwKillTimer = 0;

	struct text_t
	{
		int x,y;
		DWORD clr;
		wstring str;
		
	};

	map<string, text_t > g_strTable;
}
using namespace localScope;

// multibyte�� ���� 
//#include <stdio.h>

inline void W2M( string& destMultibyteString, const wstring& srcUniString  )
{
	static char multiBuff[1024] ={0,}; 
	// unicode�� ���� 
	WideCharToMultiByte( 932, 0,//WC_NO_BEST_FIT_CHARS | WC_COMPOSITECHECK | WC_DEFAULTCHAR
						(LPCWSTR)srcUniString.c_str(), (int)srcUniString.length(), multiBuff, 1024, NULL, NULL); 

	destMultibyteString = multiBuff;
}


	// unicode�� ���� 
inline wchar_t* M2W(  const string& srcMultiStr  )
{
	static wchar_t uniBuff[1024] ={0,}; 


	int euckr_uniLen = MultiByteToWideChar(949, 0,	(LPCSTR)srcMultiStr.c_str(), (int)srcMultiStr.length(), uniBuff, sizeof(uniBuff)/sizeof(wchar_t));
	uniBuff[euckr_uniLen] = 0;

	//destUniStr = uniBuff;
	return uniBuff;
}

void print( const char* str )
{
	OutputDebugStringA( str );
	OutputDebugStringA( "\n" );
}

void printOut_native(const char* szDesc, int x, int y,DWORD clr, const char* str)
{
	text_t tt;
	tt.str = M2W(str);
	tt.x = x;
	tt.y = y;
	tt.clr = clr;
	
	g_strTable[szDesc] = tt ;
}

void control_clr( const char* desc, BYTE a, BYTE r, BYTE g, BYTE b )
{
	map<string,text_t>::iterator it = g_strTable.find( desc );
	if( it != g_strTable.end() )
	{
		(*it).second.clr = D3DCOLOR_ARGB(a,r,g,b) ;
	}
}
void control_textPos( const char* desc,int x, int y )
{
	map<string,text_t>::iterator it = g_strTable.find( desc );
	if( it != g_strTable.end() )
	{
		(*it).second.x = x;
		(*it).second.y = y;
	}

}

int GetMousePositionX()
{
	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( g_hWnd, &pt );
	return pt.x;
}

int GetMousePositionY()
{
	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( g_hWnd, &pt );
	
	return pt.y;
}

void InitLua()
{
	g_luaState = lua_open();

	// Lua �⺻ �Լ����� �ε��Ѵ�.- print() ���
	int ret = luaopen_base( g_luaState );
	luaopen_os(g_luaState );
	luaopen_table(g_luaState );
	luaopen_string(g_luaState );
	luaopen_math(g_luaState );

	// LuaTinker �� �̿��ؼ� �Լ��� ����Ѵ�.
	lua_tinker::def( g_luaState, "luaprintOut_native", printOut_native );
	lua_tinker::def( g_luaState, "control_clr", control_clr );
	lua_tinker::def( g_luaState, "control_textPos", control_textPos );
	
	lua_tinker::def( g_luaState, "dbgprint", print );
	lua_tinker::def( g_luaState, "GetMousePositionX", GetMousePositionX );
	lua_tinker::def( g_luaState, "GetMousePositionY", GetMousePositionY );

	RefreshLua();

}
	// sample1.lua �� �Լ��� ȣ���Ѵ�.
	
void RefreshLua()
{
	lua_tinker::dofile( g_luaState, "uiCore.lua" );
}

void UnInitLua()
{

	// ���α׷� ����
	lua_close(g_luaState);
}

void SetHWND( HWND hWnd )
{
	g_hWnd = hWnd;
}

void SetDevice( IDirect3DDevice9* pkDev )
{
	assert( pkDev && "pkDev" );
	g_pkDev = pkDev; 
}

bool LoadResource()
{
	assert( g_pkDev != NULL );

	HRESULT hr = D3DXCreateSprite( g_pkDev, &g_pkSprite);
	if( hr != D3D_OK )
	{
		MessageBox( 0, L"mainEntry::SetDevice::D3DXCreateSprite", L"error", MB_OK);
		return false;
	}

	D3DXCreateFont( g_pkDev, -12, 0, 0, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"System", &g_pkFont );

	//HMODULE hmod = LoadLibrary( L"uiRes.dll" );
	//assert( hmod != NULL );
	
	//D3DXCreateTextureFromResource( g_pkDev, hmod , MAKEINTRESOURCE(IDB_BITMAP1),&g_pkWaterMarkTexture);

	//if( hmod == NULL || g_pkWaterMarkTexture == NULL )
	//{//timer�ߵ�.�ҹ�������� �����ϰ� �����Ǵٿ�
	//	g_dwKillTimer = GetTickCount();
	//	
	//}

	//BOOL bREt = FreeLibrary(hmod);
	
	InitLua();

	return true;
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



//#include <algorithm>
void _drawText( const std::pair<string,text_t>& tt )
{
	RECT rt = {0, 0, 0,0};
	V(g_pkFont->DrawText( g_pkSprite, tt.second.str.c_str(), (INT)tt.second.str.length(),&rt,DT_CALCRECT,0xFFFF0000) );
	OffsetRect( &rt,tt.second.x,tt.second.y );
	V( g_pkFont->DrawText( g_pkSprite, tt.second.str.c_str(), (INT)tt.second.str.length(),&rt,DT_LEFT,tt.second.clr ) ); //
}


void DrawTextAll()
{
	std::for_each( g_strTable.begin(), g_strTable.end(), _drawText );

}

void DrawWaterMark()
{
	D3DVIEWPORT9 view;
	g_pkDev->GetViewport( &view );
	
	if( g_pkWaterMarkTexture )
	g_pkSprite->Draw( g_pkWaterMarkTexture, 0,0, &D3DXVECTOR3(view.Width-64.0f,view.Height-32.0f,0),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));
}

void Render(float fElapsedTime)
{
	if( g_pkDev&& g_pkDev->TestCooperativeLevel() == S_OK )
	{
		if( g_pkSprite )
		{
			if( g_pkSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE ) == S_OK )
			{
				DrawWaterMark();


				DrawTextAll();
			
				g_pkSprite->End();
			}
		}
	}
}

void OnResetDevice()
{
	if( g_pkFont )
	g_pkFont->OnResetDevice();
	if( g_pkSprite )
	g_pkSprite->OnResetDevice();
}

void OnLoseDevice()
{
	if( g_pkFont )
	g_pkFont->OnLostDevice();

	if( g_pkSprite )
	g_pkSprite->OnLostDevice();
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

void ProcessString()
{
//	lua_tinker::get<const char*>( g_luaState, "str");
	
}

//lua�� �ð�, ���콺�Է�,Ű�����Է� �̺�Ʈ ����.
//lua���� �Ѱ����� �̺�Ʈ �ݺ� ó��.
//���ϸ��̼�,�̺�Ʈ�ݺ�ȣ��.

void Process( float fElapsedTime )
{
	CheckInvalidUsing();
	ProcessString();
	lua_tinker::call<void>( g_luaState, "wakeCheck" );

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
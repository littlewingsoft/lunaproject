#include "StdAfx.h"
#include "resource.h"


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
		RECT rt;
		DWORD clr;
		wstring Text;
		
	};
	struct img_t
	{
		RECT srcRt;
		D3DXVECTOR3 Pos;
		DWORD modulateClr;
		wstring imgFullPath;
		IDirect3DTexture9* pkTexture;
	};

	map<string, text_t > gtextTable;
	map<string, img_t > gimgTable;
}
using namespace localScope;

// multibyte로 변경 
//#include <stdio.h>

inline char* W2M(  const wstring& srcUniString  )
{
	static char multiBuff[1024] ={0,}; 
	// unicode로 변경 
	int Len = WideCharToMultiByte( 932, 0,//WC_NO_BEST_FIT_CHARS | WC_COMPOSITECHECK | WC_DEFAULTCHAR
						(LPCWSTR)srcUniString.c_str(), (int)srcUniString.length(), multiBuff, 1024, NULL, NULL); 

	//destMultibyteString = multiBuff;
	multiBuff[Len]=0;
	return multiBuff;
}


	// unicode로 변경 
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

//void native_textInsert( lua_tinker::table tb)
//	const char* szName = tb.get<const char*>("name");
//	const char* szText= tb.get<const char*>("Text");
//const char* szDesc, int x, int y,BYTE alpha, DWORD clr, const char* str

void native_textInsert( const char* szName, const char* szText )
{
	text_t tt;
	tt.Text = M2W(szText);
	
	RECT rt = {0, 0, 0,0};
	V(g_pkFont->DrawText( g_pkSprite, tt.Text.c_str(), (INT)tt.Text.length(),&rt,DT_CALCRECT,0xFFFF0000) );
	tt.rt = rt;
	gtextTable[ szName ] = tt ;
}

void native_textClr( const char* desc, BYTE a, BYTE r, BYTE g, BYTE b )
{
	map<string,text_t>::iterator it = gtextTable.find( desc );
	if( it != gtextTable.end() )
	{
		(*it).second.clr = D3DCOLOR_ARGB(a,r,g,b) ;
	}
}
void native_textPosition( const char* desc,int x, int y )
{
	map<string,text_t>::iterator it = gtextTable.find( desc );
	if( it != gtextTable.end() )
	{
		(*it).second.x = x;
		(*it).second.y = y;
	}

}

void native_GetMousePosition()
{
	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( g_hWnd, &pt );

	// Lua 테이블을 생성하고 스택에 푸쉬한다.
	lua_tinker::table mousePos( localScope::g_luaState, "mousePos");

	mousePos.set("x", pt.x);
	mousePos.set("y", pt.y);
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




/*
	texture 는 2D 용이므로 필터링은 안함. mipMap도 끔 기본적인 옵션으로만 로딩함.
*/
void native_ImgLoad( const char* szKeyName, const char* szFullPathName  )
{

	map<string,img_t>::iterator it = gimgTable.find( szKeyName );
	if( it != gimgTable.end() )
	{
		assert( (*it).second.pkTexture );
		SAFE_RELEASE( (*it).second.pkTexture );
		gimgTable.erase( it );
	}


	wstring wFullPathName = M2W( szFullPathName );

	D3DXIMAGE_INFO imgInfo;
	IDirect3DTexture9* pkTexture=0;
	D3DXCreateTextureFromFileEx( g_pkDev, wFullPathName.c_str() ,D3DX_DEFAULT,D3DX_DEFAULT,1, 
									0, D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
									0, &imgInfo,0, &pkTexture );

	img_t tt;
	SetRect( &tt.srcRt, 0,0, imgInfo.Width, imgInfo.Height );
	tt.Pos = D3DXVECTOR3(0.0f,0.0f,1.0f);

	tt.modulateClr = 0xffffffff;
	tt.imgFullPath = wFullPathName;
	tt.pkTexture = pkTexture;


	gimgTable[szKeyName] = tt;
}

void native_ImgPosition( const char* szKey, float x, float y ,float z )
{
	map<string,img_t>::iterator it = gimgTable.find( szKey );
	if( it != gimgTable.end() )
	{
		img_t& tt = (*it).second;
		tt.Pos.x = x;
		tt.Pos.y = y;
		tt.Pos.z = z;

		//OffsetRect( &tt.destRt, x,y );
	}
}
void ReloadImgAll()
{
	std::map< string, img_t >::iterator it = gimgTable.begin();
	while( it != gimgTable.end() )
	{
		img_t& tt = (*it).second;
		native_ImgLoad( (*it).first.c_str() , W2M( tt.imgFullPath.c_str() ) );
		it++;
	}
}

// 게임이 완전히 종료될때 호출되어야함.
void ShutDown_ReleaseTextureAll(bool bClearTable)
{
	std::map< string, img_t >::iterator it = gimgTable.begin();
	while( it != gimgTable.end() )
	{
		img_t& tt = (*it).second;
		IDirect3DTexture9* pkTex = tt.pkTexture;
		SAFE_RELEASE( pkTex );
		it++;
	}

	if( bClearTable )
		gimgTable.clear();
	
}

void _drawText( const std::pair<string,text_t>& tt )
{
	const text_t& tmpT  = tt.second;
	static RECT rt;
	rt = tmpT.rt;
	OffsetRect( &rt, tmpT.x,tmpT.y );
	V( g_pkFont->DrawTextW( g_pkSprite,tmpT.Text.c_str(), (INT)tmpT.Text.length(),&rt,DT_LEFT,tmpT.clr ) ); //
}


void DrawTextAll()
{
	std::for_each( gtextTable.begin(), gtextTable.end(), _drawText );
}

void _DrawImg( const std::pair<string,img_t>& tt )
{
	const img_t& tmp = tt.second;
	V( g_pkSprite->Draw( tmp.pkTexture, &tmp.srcRt, 0, &tmp.Pos, tmp.modulateClr ) );
}

void DrawImgAll()
{
	std::for_each( gimgTable.begin(), gimgTable.end(), _DrawImg );
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
				DrawImgAll();
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

	ReloadImgAll();

}

void OnLoseDevice()
{
	if( g_pkFont )
	g_pkFont->OnLostDevice();

	if( g_pkSprite )
	g_pkSprite->OnLostDevice();

	ShutDown_ReleaseTextureAll(false);	
}

void CheckInvalidUsing()
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

//lua로 시간, 마우스입력,키보드입력 이벤트 전달.
//lua에서 넘겨져온 이벤트 콜벡 처리.
//에니메이션,이벤트콜벡호출.

void Process( float fElapsedTime )
{
	//CheckInvalidUsing();

	lua_tinker::call<void>( g_luaState, "UpdateControl" );

}



void InitLua()
{
	g_luaState = lua_open();

	// Lua 기본 함수들을 로드한다.- print() 사용
	int ret = luaopen_base( g_luaState );
	luaopen_os(g_luaState );
	luaopen_table(g_luaState );
	luaopen_string(g_luaState );
	luaopen_math(g_luaState );

	// LuaTinker 를 이용해서 함수를 등록한다.
	lua_tinker::def( g_luaState, "dbgprint", print );

	lua_tinker::def( g_luaState, "native_textInsert", native_textInsert );
	lua_tinker::def( g_luaState, "native_textClr", native_textClr );
	lua_tinker::def( g_luaState, "native_textPosition", native_textPosition );
	lua_tinker::def( g_luaState, "native_GetMousePosition", native_GetMousePosition );

	lua_tinker::def( g_luaState, "native_ImgLoad", native_ImgLoad);
	lua_tinker::def( g_luaState, "native_ImgPosition", native_ImgPosition );

	RefreshLua();
}
	// sample1.lua 의 함수를 호출한다.
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
                         L"Arial", &g_pkFont );

	//HMODULE hmod = LoadLibrary( L"uiRes.dll" );
	//assert( hmod != NULL );
	
	//D3DXCreateTextureFromResource( g_pkDev, hmod , MAKEINTRESOURCE(IDB_BITMAP1),&g_pkWaterMarkTexture);

	//if( hmod == NULL || g_pkWaterMarkTexture == NULL )
	//{//timer발동.불법사용으로 간주하고 무조건다운
	//	g_dwKillTimer = GetTickCount();
	//	
	//}

	//BOOL bREt = FreeLibrary(hmod);
	
	InitLua();

	return true;
}
	
void RefreshLua()
{
	ReloadImgAll();
	lua_tinker::dofile( g_luaState, "uiCore.lua" );
}

void UnInitLua()
{

	// 프로그램 종료
	lua_close(g_luaState);
}

void Release()
{
	UnInitLua();
	SAFE_RELEASE( g_pkWaterMarkTexture );
	ShutDown_ReleaseTextureAll(true);

	SAFE_RELEASE( g_pkFont );
	SAFE_RELEASE( g_pkSprite )
	
}



}

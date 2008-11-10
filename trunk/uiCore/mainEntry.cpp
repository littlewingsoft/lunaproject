#include "StdAfx.h"


static ID3DXSprite*          g_pkSprite = 0;          // Sprite used for drawing


mainEntry::mainEntry(void)
{
}

mainEntry::~mainEntry(void)
{
}
void mainEntry::SetDevice( IDirect3DDevice9* pkDev )
{
	assert( pkD3DDev && "pkD3DDev" );
	D3DXCreateSprite( pkDev, &g_pkSprite);
}

void mainEntry::Release()
{
	SAFE_RELEASE( g_pkSprite )
}

int mainEntry::LoadLua(const std::wstring& LuaFileName)
{
	/*
		mainEntry가 되는 lua 파일을 로딩후 텍스쳐 로딩과 갖가지 작업을 여기서 시작함.
	*/
	//ui프로젝트로딩.
	
	//textureList 로딩.

	return 0;
}

void mainEntry::Render(float fElapsedTime)
{

}

//lua로 시간, 마우스입력,키보드입력 이벤트 전달.
//lua에서 넘겨져온 이벤트 콜벡 처리.
//에니메이션,이벤트콜벡호출.

void mainEntry::Process( float fElapsedTime )
{

}

int mainEntry::MainLoop(float fElapsedTime)
{
	Process( fElapsedTime );

	Render( fElapsedTime );
	//d3d로 rhw로 출력하기. 일단 Sprite 를 쓰자.
}
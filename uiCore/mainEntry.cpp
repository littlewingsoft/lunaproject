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
		mainEntry�� �Ǵ� lua ������ �ε��� �ؽ��� �ε��� ������ �۾��� ���⼭ ������.
	*/
	//ui������Ʈ�ε�.
	
	//textureList �ε�.

	return 0;
}

void mainEntry::Render(float fElapsedTime)
{

}

//lua�� �ð�, ���콺�Է�,Ű�����Է� �̺�Ʈ ����.
//lua���� �Ѱ����� �̺�Ʈ �ݺ� ó��.
//���ϸ��̼�,�̺�Ʈ�ݺ�ȣ��.

void mainEntry::Process( float fElapsedTime )
{

}

int mainEntry::MainLoop(float fElapsedTime)
{
	Process( fElapsedTime );

	Render( fElapsedTime );
	//d3d�� rhw�� ����ϱ�. �ϴ� Sprite �� ����.
}
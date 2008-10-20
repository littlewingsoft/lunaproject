#include "StdAfx.h"

mainEntry::mainEntry(void)
{
}

mainEntry::~mainEntry(void)
{
}
void mainEntry::SetDevice( IDirect3DDevice9* pkD3DDev )
{
	
}

int mainEntry::LoadLua(const std::string& name)
{
	

	return 0;
}


int mainEntry::MainLoop()
{
	//시간갱신.

	//lua로 시간, 마우스입력,키보드입력 이벤트 전달.

	//lua에서 넘겨져온 이벤트 콜벡 처리.
	//에니메이션,이벤트콜벡호출.

	//d3d로 적절히 출력하기. 셰이더따위 쓰지않기에 그냥...
	//rhw로 출력.
}
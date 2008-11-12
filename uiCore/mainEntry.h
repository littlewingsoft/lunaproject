#pragma once

#include <string>

namespace uiCore
{
	int LoadLua(const std::wstring& LuaFileName);
	void SetDevice( IDirect3DDevice9* pkD3DDev );
	void Release();

	void Render(float fElapsedTime);
	void Process( float fElapsedTime );
	int MainLoop(float fElapsedTime );
};

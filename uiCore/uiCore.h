#pragma once

#include <string>

namespace uiCore
{
	void SetDevice( IDirect3DDevice9* pkD3DDev );
	
	void ReleaseTextureAll();
	void Release();

	void Render(float fElapsedTime);
	void Process( float fElapsedTime );
	int MainLoop(float fElapsedTime );
};

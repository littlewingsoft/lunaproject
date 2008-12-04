#pragma once

#include <string>

namespace uiCore
{
	void SetHWND( HWND hWnd );
	void SetDevice( IDirect3DDevice9* pkD3DDev );
	bool LoadResource();

	void RefreshLua();

	void ShutDown_ReleaseTextureAll(bool bClearTable);
	void Release();

	void OnResetDevice();
	void OnLoseDevice();

	void Render(float fElapsedTime);
	void Process( float fElapsedTime );
	int MainLoop(float fElapsedTime );
};
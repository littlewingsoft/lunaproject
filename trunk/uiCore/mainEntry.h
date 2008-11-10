#pragma once

class mainEntry
{
	IDirect3DDevice9* m_pkD3DDev;


public:
	mainEntry(void);
	~mainEntry(void);
	int LoadLua(const std::wstring& LuaFileName);
	void SetDevice( IDirect3DDevice9* pkD3DDev );
	void Release();

	void Render(float fElapsedTime);
	void Process( float fElapsedTime );
	int MainLoop(float fElapsedTime );
};

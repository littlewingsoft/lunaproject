#pragma once

class IDirect3DDevice9;

class mainEntry
{
public:
	mainEntry(void);
	~mainEntry(void);
	int LoadLua(const std::string& name);
	void SetDevice( IDirect3DDevice9* pkD3DDev );

	int MainLoop();
};

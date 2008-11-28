#ifndef _GSWF_H_
#define _GSWF_H_

#pragma comment(lib, "lib/zdll.lib")
#pragma comment(lib, "lib/libjpeg.lib")
#pragma comment(lib, "lib/fmodex_vc.lib")
#pragma comment(lib, "lib/gswf.lib")

typedef void (*pAction)(const char* command, const char* args); 
typedef void (*pLog)(bool error, const char* message);

struct INFOGSWF
{
	bool bVerboseAction;
	bool bVerboseParse;
	bool bVisibleBack;
	char* infile;
	pAction pAction_callback;
	pLog	pLog_callback;
};

// 플래쉬 관련
void	CreateGSWF( const LPDIRECT3DDEVICE9 pd3dDevice, INFOGSWF& info );
void	UpdateGSWF( float fElapsed );
void	RenderGSWF();
LRESULT MsgProcGSWF( UINT uMsg, WPARAM wParam, LPARAM lParam );
void	GetVarGSWF( char* value, const char* variable );
void	SetVarGSWF( const char* variable, const wchar_t* value );
void	CallMethodGSWF(char* value, const char* method_name, const char* method_arg_fmt, ...);
void	DestroyGSWF();



#endif
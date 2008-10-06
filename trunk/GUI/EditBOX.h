
#pragma once
#pragma comment(lib, "imm32.lib"   )

#define		ID_CHATEDIT 3333


enum COMPSTATE{	CECW_STARTCOMP,	CECW_DOCOMP,	CECW_ENDCOMP };// 한글 조합 테스트...
extern WNDPROC		IWnd_Proc  ;
LRESULT CALLBACK InputProc(HWND hWnd,UINT msg, WPARAM wParam,LPARAM lParam);
void ToggleKoreanChar(HWND hWnd);
void ToggleEngChar(HWND hWnd );
SIZE GetStrPixel(HWND hWnd, HFONT hFont, const TCHAR* const Str ); // 스트링의 픽셀값을 반환해줍니다.

extern bool g_bKoreanCharOn;
extern COMPSTATE	g_CompState;
extern HFONT ChooseFONTSYLE(void);


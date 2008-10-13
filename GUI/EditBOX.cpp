//#include <stdafx.h>
//#include <windows.h>
#include <tchar.h>

#include "scene.h"
#include "EditBox.h"


WNDPROC		IWnd_Proc = NULL;
COMPSTATE	g_CompState = CECW_ENDCOMP;
bool		g_bKoreanCharOn = false;

bool IsKoreanChar(HWND hWnd)
{
    HIMC    hImc;
    DWORD   dwConversion, dwSentence;
    hImc = ImmGetContext( hWnd );
    ImmGetConversionStatus( hImc, &dwConversion, &dwSentence );
    ImmReleaseContext( hWnd , hImc );
    return ((dwConversion & IME_CMODE_HANGEUL) == IME_CMODE_HANGEUL) ? true : false ;
}

// ------------------------------------- 
// 한글 On 상태 만들기 
// ------------------------------------- 
void ToggleKoreanChar(HWND hWnd)
{
	HIMC hImc;
	DWORD	dwConv, dwSent;
	hImc = ImmGetContext( hWnd );
	ImmGetConversionStatus( hImc, &dwConv, &dwSent );
	ImmSetConversionStatus( hImc, IME_CMODE_NATIVE, dwSent);
	ImmReleaseContext( hWnd, hImc);
	g_bKoreanCharOn = true;
}

//-----------------------------
// 영어상태 만들기 
//-----------------------------
void ToggleEngChar(HWND hWnd )
{
	HIMC hImc;
	DWORD	dwConv, dwSent;
	hImc = ImmGetContext( hWnd );
	ImmGetConversionStatus( hImc, &dwConv, &dwSent );
	dwConv &= ~IME_CMODE_NATIVE;
	ImmSetConversionStatus( hImc, dwConv, dwSent);
	ImmReleaseContext( hWnd, hImc);
	g_bKoreanCharOn = false;
	g_CompState = CECW_ENDCOMP;		
}

SIZE GetStrPixel(HWND hWnd, HFONT hFont, const TCHAR* const Str ) // 스트링의 픽셀값을 반환해줍니다.
{
	//				한글 
	//	_MBCS		
	// _tcsclen 은 _MBCS 일때는 한글 한글자를 1바이트로 처리함 
	// _tcslen 은  _MBCS 일때   한글 한글자를 2바이트로 처리함 

	size_t len =_tcslen( Str );
	SIZE size={0,};
	HDC hdc = GetDC( hWnd );
	SelectObject( hdc, (HFONT)hFont );
	BOOL bl = GetTextExtentPoint32( hdc, Str, (int)len, &size );
	ReleaseDC( hWnd,hdc );
	return size;
}


LRESULT CALLBACK InputProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	
	if( CSCENE::GetCurrScene()->WinMSGProc( hWnd, Msg,wParam,lParam) == true )
	{
		// 특정 신에서 특정 메세지를 처리 했다면 기본 메세지 처리기로 넘어감.
		assert( IWnd_Proc!= NULL);
		return CallWindowProc( IWnd_Proc, hWnd, Msg, wParam, lParam);
	}
	
	switch( Msg )
	{
		/*
	case WM_IME_NOTIFY:
		{
		}
		return 1;*/

	case WM_KEYUP:
		{
			
		}return 1;
	
	case WM_CREATE:
		{
			SendMessage( hWnd, WM_SETFONT,(WPARAM)g_D3D.GetGDIFont(),0);
		}
		break;

	case WM_DESTROY:
		{
			
		}
		return 0;

	case WM_KEYDOWN:
		{
			
			switch((wParam))  //LOWORD
			{
				
			case VK_PROCESSKEY:
				{
					// 현재 포커스 가진 에디트 박스의 속성이 한글을 허용치 않으면 
					// 무조건 영어로 변환 시킨다. 
					IGUI_Object* pTmp = CGUI_MGR::GetCurrFocus();
					if( pTmp != NULL )
					{
						if( pTmp->GetClsID() != GUI_EDITBOX ) break;
					}
					CGUI_Edit* pEdit =(CGUI_Edit*) pTmp;
					if( pEdit != NULL)
					{
						if(  pEdit->GetAttrib() == CGUI_Edit::PASSWD ||
							pEdit->GetAttrib() == CGUI_Edit::ID )
						{
							ToggleEngChar( g_D3D.GethWnd() );
							g_bKoreanCharOn = false;
						}else
						{
							g_bKoreanCharOn  = IsKoreanChar( hWnd );
						}
					}
				}
				break;
				
			}
		}
		break;
		
	case WM_CHAR:
		{
			switch( wParam )
			{
			case VK_TAB:
				{CGUI_MGR::NextEditBox();}
			case VK_RETURN:
				return false;
			}
		}
		break;
		
	   case WM_IME_STARTCOMPOSITION: // start Hangul composition
		   g_CompState = CECW_STARTCOMP;
		   break;
		   
	   case WM_IME_COMPOSITION: // doing Hangul composition
		   if ( g_CompState== CECW_STARTCOMP )
			   g_CompState = CECW_DOCOMP;
		   break;
		   
	   case WM_IME_ENDCOMPOSITION: // just changed to English input mode
		   g_CompState = CECW_ENDCOMP;		
		   break;
	}
	assert( IWnd_Proc!= NULL);
	return CallWindowProc( IWnd_Proc, hWnd, Msg, wParam, lParam);
}

#pragma comment(lib, "Comdlg32.lib"   )
HFONT ChooseFONTSYLE(void)
{	// 폰트의 설정을 담당하는 폰트 츄즈 박스가 뜹니다. 
	CHOOSEFONT cf; 
	LOGFONT lf; 
	// Initialize members of the CHOOSEFONT structure. 
	HFONT hFont;	
	cf.lStructSize = sizeof(CHOOSEFONT); 
	cf.hwndOwner = (HWND)NULL; 
	cf.hDC = (HDC)NULL; 
	cf.lpLogFont = &lf; 
	cf.iPointSize = 0; 
	cf.Flags = CF_SCREENFONTS|CF_EFFECTS; 
	cf.rgbColors = RGB(0,0,0); 
	cf.lCustData = 0L; 
	cf.lpfnHook = (LPCFHOOKPROC)NULL; 
	cf.lpTemplateName = (LPSTR)"dotumche";//NULL; 
	cf.hInstance = (HINSTANCE) NULL; 
	cf.lpszStyle = (LPSTR)NULL; 
	cf.nFontType = REGULAR_FONTTYPE;//SCREEN_FONTTYPE; 
	cf.nSizeMin = 0; 
	cf.nSizeMax = 0; 
				
	// Display the CHOOSEFONT common-dialog box. 
	ChooseFont(&cf); 
				
	// Create a logical font based on the user's 
	// selection and return a handle identifying 
	// that font. 
	hFont = CreateFontIndirect(cf.lpLogFont); 
	return hFont;
				
}
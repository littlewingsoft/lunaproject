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
// �ѱ� On ���� ����� 
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
// ������� ����� 
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

SIZE GetStrPixel(HWND hWnd, HFONT hFont, const TCHAR* const Str ) // ��Ʈ���� �ȼ����� ��ȯ���ݴϴ�.
{
	//				�ѱ� 
	//	_MBCS		
	// _tcsclen �� _MBCS �϶��� �ѱ� �ѱ��ڸ� 1����Ʈ�� ó���� 
	// _tcslen ��  _MBCS �϶�   �ѱ� �ѱ��ڸ� 2����Ʈ�� ó���� 

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
		// Ư�� �ſ��� Ư�� �޼����� ó�� �ߴٸ� �⺻ �޼��� ó����� �Ѿ.
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
					// ���� ��Ŀ�� ���� ����Ʈ �ڽ��� �Ӽ��� �ѱ��� ���ġ ������ 
					// ������ ����� ��ȯ ��Ų��. 
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
{	// ��Ʈ�� ������ ����ϴ� ��Ʈ ���� �ڽ��� ��ϴ�. 
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

//#include "Gui_Text.h"
//#include "D3D.h"
#include <guistdafx.h>

////////////////////////////////////////////////
//
// 스트링라인 찍어주는 텍스트 관련 클래스
//
////////////////////////////////////////////////
//DWORD CGUI_Text::TEXTCNT = 0;
CGUI_Text::CGUI_Text()
{
	m_Gui_ClsID= GUI_TEXT; //
	m_FontClr = 0xffffffff;
	m_Str =NULL; 
	m_Strlen = 0;
}

CGUI_Text::CGUI_Text(int x,int y,DWORD OutClr, DWORD FontClr,const TCHAR* Str )
{ 
	m_Gui_ClsID=GUI_TEXT;

	m_FontClr = FontClr; m_OutLineClr= OutClr; 
	m_Str= NULL; m_Strlen= 0;
	MakeStr2Texture( x, y, Str );
}

CGUI_Text::~CGUI_Text()
{
//	TEXTCNT --;
	if( m_Img.TexID != -1 )
		g_D3D.ReleaseANI( m_Img.TexID );
	SAFE_FREE( m_Str );
}
void CGUI_Text::SetFontClr(DWORD Clr) { m_FontClr = Clr; }
void CGUI_Text::SetOutLineClr(DWORD Clr) { m_OutLineClr = Clr; }

void CGUI_Text::SetString(const TCHAR* Str)
{
	SAFE_FREE( m_Str ); 
	m_Strlen = _tcslen(Str) ;
	m_Str = (TCHAR*) malloc( sizeof(TCHAR)*(m_Strlen+1) );  
	_tcscpy(m_Str,Str);
} 
void CGUI_Text::MakeStr2Texture( int x, int y, const TCHAR* szStr )
{
	SetString( (TCHAR*)szStr );
	if( this->m_Strlen == 0 ) return;
	// 부모가 있다면
	if( x == -1 && y == -1 )
	{
		if( this->m_pParent )
		{
			x = (int)m_BoundRect.left;
			y = (int)m_BoundRect.top;
		}else
		{
			x = 0;
			y = 0;
		}
	}


	// 스트링을 텍스쳐로 만들어 줍니다.
	if( m_Img.TexID != -1 )
	g_D3D.ReleaseANI( m_Img.TexID );


	memset( &m_Img.sprInfo, 0, sizeof( m_Img.sprInfo ));
	m_Img.TexID = g_D3D.Str2Texture((TCHAR*)m_Str,m_OutLineClr,m_FontClr );

	SIZE sz = g_D3D.GetActualImgSize( m_Img.TexID );
	m_Img.sprInfo.nWidth  = sz.cx;
	m_Img.sprInfo.nHeight = sz.cy;
	SetSrcRect( 0,0, sz.cx,sz.cy);
	m_DestRect.left = (float)x;
	m_DestRect.top = (float)y;
	m_DestRect.right = (float)(x+sz.cx);
	m_DestRect.bottom = (float)(y+sz.cy);

	SetBoundRect( (float)x,(float)y,(float)(x+sz.cx),(float)(y+sz.cy) );
	
	assert( g_D3D.GetTexture( m_Img.TexID ) != NULL );
}

void CGUI_Text::Process(){}
void CGUI_Text::Render()
{
	// 텍스쳐에 올라간 스트링을 그립니다.
	
	if( (m_DestRect.right - m_DestRect.left) != 0 )
	{
		if( (m_DestRect.bottom - m_DestRect.top) != 0 )
		{
			RECT tmp={ (LONG)m_DestRect.left,(LONG)m_DestRect.top, (LONG)m_DestRect.right,(LONG)m_DestRect.bottom };
			g_D3D.BltSprite( m_Layer, m_Img.TexID,&m_SrcRect, tmp, m_Clr, D3D_ALPHASTATE );//D3D_NORMALSTATE 
		}
		
	}
}
bool CGUI_Text::ProcMessage(GUIMSG& Msg)
{
//	if( this->ProcMessage_Child( Msg ) == true )
//		return true;

	return false;
}


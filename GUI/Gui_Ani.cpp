
#include "guistdafx.h"
#include "gui_ani.h"

namespace gui
{

////////////////////////////////////////////////
//
// GUI_ANI 관련 클래스
//
////////////////////////////////////////////////
CGUI_Ani::CGUI_Ani()
{
	m_Gui_ClsID= GUI_ANI;
	m_LastTime = 0;
	m_CurrentFrame = 0;
	m_bAni = false;
}
CGUI_Ani::CGUI_Ani(int x, int y, TCHAR* fName ):CGUI_Ani::CGUI_Ani()
{
	//m_Gui_ClsID=GUI_ANI;
	//g_D3D.LoadAni( fName, m_ImgHandle );
	
	if( m_ImgHandle.TexID == -1 )
	{
#ifdef _HTMLLOG
		g_HtmlLog.LogError( _T("%s 를 찾지 못했습니다."), fName );
#endif 
		assert( m_ImgHandle.TexID != -1 );
	}

	SetBoundRect( x,y, x + m_ImgHandle.sprInfo.nWidth , y + m_ImgHandle.sprInfo.nHeight); //m_ImgHandle.sprInfo.nFrame * 
	SetDestRect( x,y,x + m_ImgHandle.sprInfo.nWidth , y + m_ImgHandle.sprInfo.nHeight); //m_ImgHandle.sprInfo.nFrame * 
	SetSrcRect(0,0,m_ImgHandle.sprInfo.nWidth, m_ImgHandle.sprInfo.nHeight);
	m_LastTime = 0;	
	m_bAni = false;
	m_CurrentFrame = 0;
	
	m_Clr = 0xffffffff;
}

void CGUI_Ani::SetAni(bool bAni)
{
	m_bAni = bAni; 
}

void CGUI_Ani::SetFrame( int Frame )
{
	m_CurrentFrame = Frame; 
	m_CurrentFrame %= m_ImgHandle.sprInfo.nFrame; 
}

void CGUI_Ani::Process()
{
	// 자식을 돌리자 
//	Process_Child();
}

void CGUI_Ani::Render()
{
	if( GETA(m_Clr) == 0x00 ) return;

	if( m_bAni )
	{
		if( timeGetTime() - m_LastTime > m_ImgHandle.sprInfo.mspf )
		{
			m_LastTime = timeGetTime();
			m_CurrentFrame++;
			if( m_CurrentFrame >= m_ImgHandle.sprInfo.nFrame )
			m_CurrentFrame = 0;
		}
	}

	RECT srcRect=	{
					(m_CurrentFrame * m_ImgHandle.sprInfo.nWidth)+m_SrcRect.left, 
					m_SrcRect.top, 
					(m_CurrentFrame * m_ImgHandle.sprInfo.nWidth)+m_SrcRect.right,//	(m_CurrentFrame * m_ImgHandle.sprInfo.nWidth)+m_ImgHandle.sprInfo.nWidth,
					m_SrcRect.bottom 
					};
	if( (srcRect.right - srcRect.left) != 0 )
	{
		if( (srcRect.bottom - srcRect.top) != 0 )
		{
			RECT tmp={m_DestRect.left,m_DestRect.top, m_DestRect.right,m_DestRect.bottom };
			g_D3D.BltSprite( m_Layer, m_ImgHandle.TexID, &srcRect, tmp,m_Clr,D3D_ALPHASTATE );//D3D_NORMALSTATE
		}
		
	}
	// 자식을 그리자
	//Render_Child();
}

bool CGUI_Ani::ProcMessage(GUIMSG& pMsg)
{
	//if( this->ProcMessage_Child( pMsg ) == true )
	//	return true;

	return false;
}


}
// GUI_FocusTEXT.cpp: implementation of the CGUI_FocusText class.
//
//////////////////////////////////////////////////////////////////////

#include <guistdafx.h>

//	CGUI_Text(int x,int y,DWORD		FontClr,DWORD		OutClr,const TCHAR* Str );
CGUI_FocusText::CGUI_FocusText(int x,int y,DWORD FontClr,DWORD 	OutClr,const TCHAR* Str ):
	CGUI_Text(x,y,FontClr,OutClr,Str)
{

	m_State = Off;
	g_D3D.LoadAni( _T(".\\IMG\\etc\\WhiteFade.ani"), m_Back );
	
}

CGUI_FocusText::~CGUI_FocusText()
{
	
}


void	CGUI_FocusText::Render()
{
	if( m_State == On )
	{
		RECT rt={ m_DestRect.left, m_DestRect.top+1, m_DestRect.right, m_DestRect.bottom+1 };
		g_D3D.BltSprite( this->m_Layer, m_Back.TexID, &m_SrcRect, rt, 0xff000000, D3D_NORMALSTATE);
	}

	CGUI_Text::Render();
}

bool	CGUI_FocusText::ProcMessage(GUIMSG& pMsg)
{
	switch( pMsg.dwMsg )
	{
	
	case GUIMSG_MLDOWN:
		{ // 나의 영역위에 마우스가 올라왔으면 On 으로 처리함 
			int x =int( pMsg.dwParam1 );
			int y =int( pMsg.dwParam2 );

			if( m_BoundRect.left <= x  &&
				x <= m_BoundRect.right &&
				m_BoundRect.top  < y  &&
				y <= m_BoundRect.bottom )
			{
				m_State = On;
			}else
				m_State = Off;
		}
		return false;// 부모가 또 처리해야 한다 그러므로 나에서 메세지 처리가 끝나지 않아야함 
	}

	return false;
}

bool CGUI_FocusText::IsFocusOn()
{
	return (m_State==On);
}
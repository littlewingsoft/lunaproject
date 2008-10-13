
//#define DRGBA(r,g,b,a)      ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
//#define GETA(rgb) ((BYTE)((rgb)>>24))
//#define GETR(rgb) ((BYTE)((rgb)>>16))
//#define GETG(rgb) ((BYTE)(((WORD)(rgb)) >> 8))
//#define GETB(rgb) ((BYTE)(rgb))

//#include ".\gui_subclass_fadeinout.h"
#include "stdafx.h"

DWORD  GUI_Subclass_FadeInOut::FUNCTEMPLATE::_BeginTime=0;

GUI_Subclass_FadeInOut::GUI_Subclass_FadeInOut(void) //나타났다가 사라지기
{}

GUI_Subclass_FadeInOut::~GUI_Subclass_FadeInOut(void)
{}

void	GUI_Subclass_FadeInOut::Process()
{
	if( _StateList.empty() ) return;
	_pFunc = _StateList[0]._pFunc;
	(this->*_pFunc)();
}



void	GUI_Subclass_FadeInOut::Fade( int dt )
{
	BYTE A = GETA(m_Clr);
	if( 255 <= (A+dt) ) 
	{
		A=255;	dt=0;
	}
	if( A+dt < 0 )		{ A=0;		dt=0;}
	
	A += dt;
	m_Clr = DRGBA( 0xff,0xff,0xff, A );
	m_pParent->SetClr( m_Clr );
}

void	GUI_Subclass_FadeInOut::Render()
{

}

void	GUI_Subclass_FadeInOut::UDie()
{
//	_StateList.clear();
//	m_Clr = 0xffffffff ;
//	m_pParent->SetClr(m_Clr);
//	if( !_StateList.empty() )
//	_StateList.erase(_StateList.begin());

	FUNCTEMPLATE tt;
	tt.dt = -1;
	tt.DelayTime = 1000;
	tt._pFunc= FadeOutProcess;
	_StateList.push_back( tt );	
	_FadeState = fOUT;
}

void GUI_Subclass_FadeInOut::FadeInProcess(void)
{
	Fade( 15 );
	if( GETA(m_Clr) >= 0xff )
	{
		m_Clr= 0xffffffff;
		_StateList.erase( _StateList.begin() );
		FUNCTEMPLATE::_BeginTime = timeGetTime();
	}
}
void GUI_Subclass_FadeInOut::FadeWaitProcess(void)
{
	if( timeGetTime() - _StateList[0]._BeginTime >= _StateList[0].DelayTime )
	{
		_StateList.erase( _StateList.begin() );
	}
}

void GUI_Subclass_FadeInOut::FadeOutProcess(void)
{
	Fade( -15 );
	m_pParent->MoveGUI(0,-1.0f); //timeGetTime()-_BeginTime >  ||
	if( GETA(m_Clr) <= 10 ||  m_pParent->GetY() <= 0 ) // 시간, 그리고 투명도가 0,그리고 상단 얼마정도 이상 올라가면 죽으라
	{
		CGUI_MGR::DestroyObjQueue( this->GetParent()->GetName() );
	}
}

bool	GUI_Subclass_FadeInOut::ProcMessage(GUIMSG& pMsg)
{
	switch( pMsg.dwMsg )
	{
		case GUIMSG_CREATED: //나타났다가 사라지는걸 모두 여기서 정의해주자 일일이 코딩을...
			{
				m_Clr = 0x00ffffff;
				m_pParent->SetClr(m_Clr);
				FUNCTEMPLATE tt;
				tt.dt = 10;
				tt.DelayTime = 3000;
				tt._pFunc = GUI_Subclass_FadeInOut::FadeInProcess;
				_StateList.push_back( tt );

				FUNCTEMPLATE att;
				att.DelayTime = 500;
				att._pFunc = GUI_Subclass_FadeInOut::FadeWaitProcess;
				_StateList.push_back( att );

				FUNCTEMPLATE::_BeginTime = timeGetTime();
			}
		return true;

		case GUIMSG_DESTROY:
			_StateList.clear();
			//시작 화면에 쓰이는 함수를 여기서 호출..?
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////

void	GUI_Subclass_ZOOM_AlphaBlend_Score::SetNumber(int Score)
{
//	itoa( Score,_szBuff,10);
	wsprintf( _szBuff,"%03d",Score);
	_Len = strlen( _szBuff );
	for( int i=0; i < _Len; i++ )
	{
		_szBuff[i] -= 48;
	}
}
GUI_Subclass_ZOOM_AlphaBlend_Score::~GUI_Subclass_ZOOM_AlphaBlend_Score(void){}

bool	GUI_Subclass_ZOOM_AlphaBlend_Score::ProcMessage(GUIMSG& pMsg)
{
	switch( pMsg.dwMsg )
	{
		case GUIMSG_CREATED: //나타났다가 사라지는걸 모두 여기서 정의해주자 일일이 코딩을...
			{
				Util_Timer ( TIMER_GETELAPSEDTIME );
				_Zoom = 1.0f;
				_A = 255;
			}
		return true;

		case GUIMSG_DESTROY:
			
		return true;
	}
	return false;
}
void	GUI_Subclass_ZOOM_AlphaBlend_Score::Process()
{
	_A = (2.0f-_Zoom) * 128;
	_Zoom += 3.0f * Util_Timer ( TIMER_GETELAPSEDTIME );
	if( _Zoom > 2.0f )
	{
		CGUI_MGR::DestroyObjQueue( this->GetName() );
	}
}
void	GUI_Subclass_ZOOM_AlphaBlend_Score::Render()
{
	if( _Len <= 0 ) return;
	RECT Src={0,};

	for( int i=_Len-1;i>=0;i--)
	{
			RECT Dest={0,};
		SetRect(&Src,_szBuff[i]*22,0,_szBuff[i]*22+22,22);
		int sz = 22*_Zoom;
		SetRect(&Dest,0,0,sz,sz);
		sz >>= 1;
		OffsetRect(&Dest, 583+(i*24)+11-sz, 67+11-sz);

		g_D3D.BltSprite( D3D_MAX_FAR_LAYER0, m_Img.TexID, &Src,Dest,D3DCOLOR_ARGB((BYTE)127+_A,255,255,255),D3D_ALPHASTATE);//D3D_NORMALSTATE 
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//GUI_Subclass_MoveBounce::~GUI_Subclass_MoveBounce(void){}
//
//bool	GUI_Subclass_MoveBounce::ProcMessage(GUIMSG& pMsg)
//{
//	switch( pMsg.dwMsg )
//	{
//		case GUIMSG_CREATED: //나타났다가 사라지는걸 모두 여기서 정의해주자 일일이 코딩을...
//			{
//				Util_Timer ( TIMER_GETELAPSEDTIME );
//				_Vel = 9.8f;
//			}
//		return true;
//
//		case GUIMSG_DESTROY:
//			
//		return true;
//	}
//	return false;
//}
//void	GUI_Subclass_MoveBounce::Process()
//{
//	//_Zoom += 3.0f * Util_Timer ( TIMER_GETELAPSEDTIME );
//	//if( _Zoom > 2.0f )
//	//{
//	//	CGUI_MGR::DestroyObjQueue( this->GetName() );
//	//}
//}
//
//void	GUI_Subclass_MoveBounce::Render()
//{
//	//RECT Src={0,};
//	//for( int i=_Len-1;i>=0;i--)
//	//{
//	//		RECT Dest={0,};
//	//	SetRect(&Src,_szBuff[i]*22,0,_szBuff[i]*22+22,22);
//	//	int sz = 22*_Zoom;
//	//	SetRect(&Dest,0,0,sz,sz);
//	//	sz >>= 1;
//	//	OffsetRect(&Dest, 583+(i*24)+11-sz, 67+11-sz);
//
//	//	g_D3D.BltSprite( D3D_MAX_FAR_LAYER0, m_Img.TexID, &Src,Dest,D3DCOLOR_ARGB((BYTE)127+_A,255,255,255),D3D_ALPHASTATE);//D3D_NORMALSTATE 
//	//}
//}
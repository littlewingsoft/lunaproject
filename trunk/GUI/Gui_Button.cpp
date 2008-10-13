
#include "stdafx.h"
#include "gui_mgr.h"
#include "gui_button.h"

////////////////////////////////////////////////
//
// Name: CGUI_Button
//
// Desc: Default ��ư class
//
////////////////////////////////////////////////

CGUI_Button::CGUI_Button()
{ 
	m_Clr= 0xffffffff;  m_LastState = m_State = NORMAL; 
//	m_pCommand = NULL; 
	m_Gui_ClsID = GUI_BUTTON; 
	m_FirstClick.x =0;	m_FirstClick.y =0;
}
CGUI_Button::~CGUI_Button()
{
	DestroyAllChild();
//	SAFE_DELETE( m_pCommand );
};
/*
void CGUI_Button::SetCommand(ICommand* pCommand )
{
	SAFE_DELETE( m_pCommand );
	assert(pCommand!= NULL);
	m_pCommand = pCommand; 
	m_pCommand->SetParent( this ); 
}
*/
bool CGUI_Button::IsEvent()
{
	if(m_LastState == DOWN && m_State == FOCUSON )
	{
		return true;
	}
	return false; 
}

void	CGUI_Button::DoEvent() // ���õǾ��ִ� �̺�Ʈ�� �ߵ��غ귯 
{
	//if( m_pCommand != NULL )
	//{
	//	m_pCommand->excute(); // �ͽ�Ʈ�� Ŀ��� ������ ����, �ʴ��ϱ���� �ͽ�Ʈ�� �̺�Ʈ Ŀ������ 
	//}

	// �ְ� �θ� ã�� �׿��� �޼��� �̺�Ʈ ���� 
	
	IGUI_Object* pParent = GetSuperParent();
	if( pParent != NULL && pParent != this ) // ���� �ְ�θ�� ���ڽ��� �ƴϾ���� 
		CGUI_MGR::_SendMessage( pParent->GetName(), GUIMSG_BUTTON_EVENT,(DWORD)this,0 );//
	//else // �θ� ������ MGR �⺻ ���� ������ ����
		CGUI_MGR::_SendMessage( GUIMSG_BUTTON_EVENT,(DWORD)this,0 );

}

bool CGUI_Button::ProcMessage(GUIMSG& Msg)
{
//	if( this->ProcMessage_Child( pMsg ) == true )
//		return true;
	// ��Ȱ��ȭ�� ���¶�� GUIMSG_ENABLE�� �����ϰ� ��� �ݻ�
	if( m_State == INACTIVE && Msg.dwMsg != GUIMSG_ENABLE ) 
		return false;

	switch ( Msg.dwMsg )
	{
	case GUIMSG_ENABLE:
		m_State = NORMAL;
		return true;
	case GUIMSG_DISABLE:
		m_State = INACTIVE;
		return true;

	case GUIMSG_CLOSE:
		{
			CGUI_MGR::NoneFocusSetting();
			DestroyAllChild();
		}
		return false;

	//case GUIMSG_DRAG:
	//	{
	//		if( CGUI_MGR::GetCurrFocus() != this ) 
	//			return false; // ��ư�� �巹�� �ȵ� @,.@;
	//	}
	//		return true;

	case GUIMSG_MLDOWN:
		{
			int x = Msg.dwParam1;//g_MouseMGR.GetX();//(int);
			int y = Msg.dwParam2;//g_MouseMGR.GetY();//(int)Msg.dwParam2;
			if( m_DestRect.left < x  &&
				m_DestRect.top  < y  &&
				x < m_DestRect.right &&
				y < m_DestRect.bottom)
			{
				if( m_Img.TexID !=-1 )
				{// ���� ����Ű�� �˻����.
				if( g_D3D.IsTransparentColor( m_Img.TexID,x-m_DestRect.left,y-m_DestRect.top  ) == true ) 
					return false;
				}

				CGUI_MGR::SetGuiFocus( m_Name );	// �θ𿡰� ��Ŀ���� �ش�.
												// �ǳڵ��� �ݱ� ������ �������� �ֻ���...
												// �ǳڵ鳢���� ��Ŀ�� ���� ����.
												// ������ �ǳ��̸� �� �ڷ� ������
												// �ƴϸ� ���ڸ� �״��~
				m_FirstClick.x = x - m_BoundRect.left;
				m_FirstClick.y = y - m_BoundRect.top ;
				m_State = DOWN;
				g_sndmgr.Play("CLICK");

				// ����θ𿡰� ����
				// ���ٸ� �⺻ ������ ���� 
					IGUI_Object* pParent = GetParent();
				if( pParent != NULL && pParent != this ) // ���� �ְ�θ�� ���ڽ��� �ƴϾ���� 
					CGUI_MGR::_SendMessage( pParent->GetName(), GUIMSG_BUTTON_EVENT,(DWORD)this,0 );//
				return true;
			}
			return false;
		}
		break;

	case GUIMSG_MMOVE:
		{
			int x = g_MouseMGR.GetX();//(int)pMsg->dwParam1;
			int y = g_MouseMGR.GetY();//(int)pMsg->dwParam2;
			if( m_DestRect.left < x  &&
				m_DestRect.top  < y  &&
				x < m_DestRect.right &&
				y < m_DestRect.bottom)
			{

				if( m_Img.TexID != -1 )
				{
					if( g_D3D.IsTransparentColor( m_Img.TexID,x-m_DestRect.left,
						y-m_DestRect.top  ) == true ) // ���� ����Ű�� �˻����.
					{
						m_State = NORMAL; 
						return false;
					}
				}
					

				if( g_MouseMGR.IsLDowning() || g_MouseMGR.IsLDown() )//
					m_State = DOWN;
					else
					m_State = FOCUSON;
			}else
			{
				m_State = NORMAL;
			}
			return false;
		}
		break;

	case GUIMSG_MLUP:
		{
			int x = Msg.dwParam1; //g_MouseMGR.GetX();
			int y = Msg.dwParam2; //g_MouseMGR.GetY();
			if( m_DestRect.left < x  &&
				m_DestRect.top  < y  &&
				x < m_DestRect.right &&
				y < m_DestRect.bottom)
			{
				if( m_Img.TexID != -1 )
				{
					if( g_D3D.IsTransparentColor( m_Img.TexID,x-m_DestRect.left,
						y-m_DestRect.top  ) == true ) // ���� ����Ű�� �˻����.
					{
						m_State = NORMAL;
						break;
					}else
						m_State = FOCUSON;
				}else
				{
						m_State = FOCUSON;
				}

				if( IsEvent() == true )
				{
					DoEvent();
				}
				return true;
			}
		}break;

	}
	return false;
}

void CGUI_Button::Process(void)
{
	m_LastState = m_State;
}

void CGUI_Button::Render(void)
{
	if( m_Img.TexID != -1 )
	{
		RECT srcRt={  ((int)m_State*m_Img.sprInfo.nWidth),0,
			((int)m_State*m_Img.sprInfo.nWidth)+m_Img.sprInfo.nWidth, m_Img.sprInfo.nHeight };

		RECT tmp={m_DestRect.left,m_DestRect.top, m_DestRect.right,m_DestRect.bottom };
		g_D3D.BltSprite( m_Layer ,m_Img.TexID, &srcRt, tmp, m_Clr, D3D_NORMALSTATE); //D3D_ALPHASTATE //D3D_INTERFACE_LAYER0

	}
}

void CGUI_Button::Active()
{
	CGUI_MGR::_SendMessage(this->GetName(),GUIMSG_ENABLE,0,0);
}

void CGUI_Button::InActive()
{
	CGUI_MGR::_SendMessage(this->GetName(),GUIMSG_DISABLE,0,0);
}


////////////////////////////////////////////////
//
// Name: CGUI_CheckButton
//
// Desc: Default ��ư class
//
////////////////////////////////////////////////
CGUI_CheckButton::CGUI_CheckButton():CGUI_Button()
{
	this->m_Gui_ClsID = GUI_CHECKBUTTON;
	_bChecked = false;
	_Alpha = 255;
	_AlphaAdd = 0;	
}
CGUI_CheckButton::~CGUI_CheckButton()
{

}

void CGUI_CheckButton::SetCheck(bool bCheck)
{
	_bChecked = bCheck;
}


#include "..\etc\Util.h"
#include "..\..\scene.h"
void CGUI_CheckButton::Process(void)
{
	m_LastState = m_State;
//	_Alpha += _AlphaAdd * g_ElapsedTime; 
//	//if( _AlphaAdd != 0 ) 
//	if( _Alpha > 255 ){ _Alpha=255; _AlphaAdd = 0; }
//	if( _Alpha < 0 ){ _Alpha=0; _AlphaAdd = 0; }
}

void CGUI_CheckButton::Render(void)
{
	if( m_Img.TexID != -1 ) // üũ �̹����� �ɰ��� 
	{
		if( _bChecked )
		{
			RECT tmp={m_DestRect.left,m_DestRect.top, m_DestRect.right,m_DestRect.bottom };
			g_D3D.BltSprite( m_Layer ,m_Img.TexID, NULL, tmp, m_Clr, D3D_NORMALSTATE ); //D3D_NORMALSTATE D3D_ALPHASTATE//D3D_INTERFACE_LAYER0
		}
	}
}


bool CGUI_CheckButton::ProcMessage(GUIMSG& Msg)
{
	// ��Ȱ��ȭ�� ���¶�� GUIMSG_ENABLE�� �����ϰ� ��� �ݻ�
	if( m_State == INACTIVE && Msg.dwMsg != GUIMSG_ENABLE ) 
		return false;

	switch ( Msg.dwMsg )
	{
	case GUIMSG_DISABLE:
		{
			m_State = INACTIVE;
		}
		return true;

	case GUIMSG_CLOSE:
		{
			CGUI_MGR::NoneFocusSetting();
			DestroyAllChild();
		}
		return false;

	case GUIMSG_MLDOWN:
		{
			POINT pt={g_MouseMGR.GetX(),g_MouseMGR.GetY() };
			RECT rt = { m_DestRect.left,m_DestRect.top,m_DestRect.right,m_DestRect.bottom };
			if( PtInRect( &rt, pt ) )
			{
				CGUI_MGR::SetGuiFocus( m_Name );	// �θ𿡰� ��Ŀ���� �ش�.
				m_FirstClick.x = pt.x - m_BoundRect.left;
				m_FirstClick.y = pt.y - m_BoundRect.top ;
				m_State = DOWN;
				//g_SndMGR.Play( SND_CLICK );
				g_sndmgr.Play( "SND_CLICK" );
				return true;
			}
		}
		break;

	case GUIMSG_MLUP:
		{
			POINT pt={g_MouseMGR.GetX(),g_MouseMGR.GetY() };
			RECT rt = { m_DestRect.left,m_DestRect.top,m_DestRect.right,m_DestRect.bottom };
			if( PtInRect( &rt, pt ) )
			{
				m_State = FOCUSON;
				_bChecked = !_bChecked;
				if(_bChecked)
				{
					_Alpha = 0;
					_AlphaAdd = 1;
				}else
				{
					_Alpha = 255;
					_AlphaAdd = -1;				
				}

				if( IsEvent() == true )
				{
					CGUI_MGR::_SendMessage( GUIMSG_BUTTON_EVENT,(DWORD)this,0 );//m_Event
					// �ڱ� �ְ� �θ� �ִٸ� ã�Ƽ� �̺�Ʈ ����
					IGUI_Object* pParent = GetSuperParent();
					if( pParent != NULL )
					CGUI_MGR::_SendMessage( pParent->GetName(), GUIMSG_BUTTON_EVENT,(DWORD)this,0 );//m_Event
				}
				return true;
			}
		}break;

	}
	return false;
}

////////////////////////////////////////////////
//
// Name: CGUI_RadioButton
//
// Desc: Default ��ư class
//
////////////////////////////////////////////////
vector<string> CGUI_RadioButton::_RadioNameList;
CGUI_RadioButton::CGUI_RadioButton():CGUI_CheckButton()
{
	this->m_Gui_ClsID = GUI_RADIOBUTTON; 
	_bChecked = false;
	_Alpha = 255;
	_AlphaAdd = 0;	
}
CGUI_RadioButton::~CGUI_RadioButton()
{

}
//#include "..\etc\Util.h"
//#include "..\..\scene.h"

//void CGUI_RadioButton::Render(void)
//{
//	if( m_Img.TexID != -1 ) // üũ �̹����� �ɰ��� 
//	{
//		if( _bChecked )
//		{
//			RECT tmp={m_DestRect.left,m_DestRect.top, m_DestRect.right,m_DestRect.bottom };
//			g_D3D.BltSprite( m_Layer ,m_Img.TexID, NULL, tmp, m_Clr, D3D_NORMALSTATE ); //D3D_NORMALSTATE D3D_ALPHASTATE//D3D_INTERFACE_LAYER0
//		}
//
//	}
//}

void CGUI_RadioButton::SetCheck(bool bCheck)
{
	CGUI_CheckButton::SetCheck(bCheck);
	if( bCheck )
	{
		vector<string>::iterator it = _RadioNameList.begin();
		// ���� ��ư����Ʈ ��� �˻�
		while( it!=_RadioNameList.end())
		{
			// �׸��� �ڽŰ� ���� �׷������ ���� �ֶ�� ��üŷ
			const char* psz = (*it).c_str();
			if( strcmp( m_Name, psz ) != 0 )
			{
				CGUI_RadioButton* pButt =(CGUI_RadioButton*) CGUI_MGR::FindGui( (*it).c_str() );
				if( pButt ) //
				{
					if( pButt->GetGroupName() == _GroupName )
					{
						pButt->SetCheck( false );
					}
				}
			}
			it++;
		}
	}
}

bool CGUI_RadioButton::ProcMessage(GUIMSG& Msg)
{
	// ��Ȱ��ȭ�� ���¶�� GUIMSG_ENABLE�� �����ϰ� ��� �ݻ�
	if( m_State == INACTIVE && Msg.dwMsg != GUIMSG_ENABLE ) 
		return false;

	switch ( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			_RadioNameList.push_back( m_Name );
		}
		return true;

	case GUIMSG_DISABLE:
		{
			m_State = INACTIVE;
		}
		return true;

	case GUIMSG_DESTROY:
		{
			_RadioNameList.erase( find( _RadioNameList.begin(), _RadioNameList.end(), m_Name) );
		}
		return true;

	case GUIMSG_CLOSE:
		{
			//CGUI_MGR::NoneFocusSetting();
			DestroyAllChild();
		}
		return false;

	case GUIMSG_MLDOWN:
		{
			POINT pt={g_MouseMGR.GetX(),g_MouseMGR.GetY() };
			RECT rt = { m_DestRect.left,m_DestRect.top,m_DestRect.right,m_DestRect.bottom };
			if( PtInRect( &rt, pt ) )
			{
				CGUI_MGR::SetGuiFocus( m_Name );	// �θ𿡰� ��Ŀ���� �ش�.
				m_FirstClick.x = pt.x - m_BoundRect.left;
				m_FirstClick.y = pt.y - m_BoundRect.top ;
				m_State = DOWN;
				//g_SndMGR.Play( SND_CLICK );
				g_sndmgr.Play( "SND_CLICK" );
				return true;
			}
		}
		break;

	case GUIMSG_MLUP:
		{
			POINT pt={g_MouseMGR.GetX(),g_MouseMGR.GetY() };
			RECT rt = { m_DestRect.left,m_DestRect.top,m_DestRect.right,m_DestRect.bottom };
			if( PtInRect( &rt, pt ) )
			{
				m_State = FOCUSON;
				SetCheck(true);
				if( IsEvent() == true )
				{
					CGUI_MGR::_SendMessage( GUIMSG_BUTTON_EVENT,(DWORD)this,0 );//m_Event
					// �ڱ� �ְ� �θ� �ִٸ� ã�Ƽ� �̺�Ʈ ����
					IGUI_Object* pParent = GetSuperParent();
					if( pParent != NULL )
					CGUI_MGR::_SendMessage( pParent->GetName(), GUIMSG_BUTTON_EVENT,(DWORD)this,0 );//m_Event
				}
				return true;
			}
			
		}break;

	}
	return false;
}
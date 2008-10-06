
#include <guistdafx.h>

////////////////////////////////////////////////
//
// �ǳ� ���� Ŭ���� 
// ��� ��ư���� �ϰ͵� �𸣰�� 
// �ǳڿ����� ����Ʈ ��ư�� �����͸� �����Ҽ��ִ�.
// ���Ͱ� ������ ����Ʈ ��ư�� �޼����� ���� ����
////////////////////////////////////////////////
CGUI_Pannel::CGUI_Pannel()
{
	m_Gui_ClsID=GUI_PANNEL;
	m_ClickPos.x = 0;	m_ClickPos.y = 0;
}

CGUI_Pannel::~CGUI_Pannel(){}

//void CGUI_Pannel::SetLastModal( IGUI_Object* pModal )
//{
//	assert( pModal != NULL );
////	m_pLastModal = pModal; 
//}

bool CGUI_Pannel::ProcMessage(GUIMSG& Msg)
{
//	if( this->ProcMessage_Child( pMsg ) == true )
//		return true;

	switch( Msg.dwMsg )
	{
	case GUIMSG_VK:
		{
			// ���� ����Ű��� 
			if( Msg.dwParam1 != VK_RETURN ) return true;

			CGUI_Button* pButt =(CGUI_Button*) CGUI_MGR::FindGui( _DefaultButton.c_str() );
			if( pButt )
			{
				pButt->DoEvent();
				//fRECT rt={0,};
				//pButt->GetBoundRect( rt );
				//GUIMSG msg;
				//msg.dwMsg = GUIMSG_MLDOWN;
				//msg.dwParam1 = rt.left+ ((rt.right  - rt.left)/2);
				//msg.dwParam2 = rt.top+  ((rt.bottom - rt.top) /2);// MAKELONG(  ,  );
				//pButt->ProcMessage( msg );// �޼��� ť�� �־���� 

				//msg.dwMsg = GUIMSG_MLUP;
				//pButt->ProcMessage( msg );// �޼��� ť�� �־���� 

			}
		}
		return true;

	case GUIMSG_CLOSE:
		{
			CGUI_MGR::NoneFocusSetting();
			DestroyAllChild();
			CGUI_MGR::DestroyObj( m_Name );
		}
		return true;


	case GUIMSG_DRAG:
		{
			if( CGUI_MGR::GetCurrFocus() != (IGUI_Object*)this )break;
			// ���� �������̸� ���� �մϴ�.
			int x =(int)( Msg.dwParam1 );
			int y =(int)( Msg.dwParam2 );

			if( (m_BoundRect.left+m_ClickPos.x)   < m_BoundRect.left  ||
				(m_BoundRect.left + m_ClickPos.x) > m_BoundRect.right  ||
				(m_BoundRect.top  + m_ClickPos.y) < m_BoundRect.top    ||
				(m_BoundRect.top  + m_ClickPos.y) > m_BoundRect.bottom  )
			{
				m_LastBoundRect = m_BoundRect;
				return true;
			}

			GUIMSG msg;
			msg.dwMsg = GUIMSG_MOVEWND;
			msg.dwParam1 = (x - m_LastBoundRect.left - m_ClickPos.x) ;
			msg.dwParam2 = ( y - m_LastBoundRect.top - m_ClickPos.y) ;
			this->ProcMessage( msg );			
			// �ڽ��� ���ϵ� �鿡�� ������ �޼����� �����ش�.
			/*
			for( int i=0; i< m_ChildList.size(); i++)
				{
					m_ChildList[i]->MoveGUI(	m_BoundRect.left - m_LastBoundRect.left,
												m_BoundRect.top - m_LastBoundRect.top );
				}
*/
		}
		return false;

	case GUIMSG_MOVEWND:
		{
			MoveGUI( Msg.dwParam1,  Msg.dwParam2 );
			m_LastBoundRect = m_BoundRect;
		}
		return true;

	case GUIMSG_MLUP:
		{
			int x =(int)( Msg.dwParam1 );
			int y =(int)( Msg.dwParam2 );

			if( m_BoundRect.left <= x  &&
				x <= m_BoundRect.right &&
				m_BoundRect.top  <= y  &&
				y <= m_BoundRect.bottom )
			{
				return true;
			}
		}
		return false;
	case GUIMSG_MMOVE:
		{
			int x =(int)( Msg.dwParam1 );
			int y =(int)( Msg.dwParam2 );

			if( m_BoundRect.left <= x  &&
				x <= m_BoundRect.right &&
				m_BoundRect.top  <= y  &&
				y <= m_BoundRect.bottom )
			{
				return true;
			}
		}return false;

	case GUIMSG_MLDOWN:
		{
			// ���� �������� ��Ŀ�� �� 
			// �� ������ ������ ��Ŀ�� 
			int x =(int)( Msg.dwParam1 );
			int y =(int)( Msg.dwParam2 );

				m_ClickPos.x = LONG(x - m_BoundRect.left);
				m_ClickPos.y = LONG(y - m_BoundRect.top); 

			if( m_BoundRect.left <= x  &&
				x <= m_BoundRect.right &&
				m_BoundRect.top  <= y  &&
				y <= m_BoundRect.bottom )
			{
				CGUI_MGR::SetGuiFocus( m_Name );
				m_LastBoundRect = m_BoundRect;
				return true;
			}

//			CGUI_MGR::NoneFocusSetting();
		}
		return false;
	}

	return false;
}

void CGUI_Pannel::Process(void)
{
	// �޼����� ������°� ������ �ؿ� �͵� ��� ���ְ� 
	// ���Ŀ� �ڽ��� �޴������� �������.
//	Process_Child();
}

void CGUI_Pannel::Render(void)
{
	if( m_Img.TexID != -1 )
	{
	RECT tmp={ (LONG)m_DestRect.left,(LONG)m_DestRect.top, (LONG)m_DestRect.right,(LONG)m_DestRect.bottom };
	g_D3D.BltSprite( m_Layer, m_Img.TexID, &m_SrcRect, tmp, m_Clr, D3D_ALPHASTATE );
	}
//	Render_Child();
}

///////////////////////////////////////////////////////////////////////////
CGUI_ContextMenu::CGUI_ContextMenu()
{
m_Gui_ClsID=GUI_PANNEL_CONTEXTMENU;
}
CGUI_ContextMenu::~CGUI_ContextMenu()
{}

void CGUI_ContextMenu::Process(void)
{
	// ���콺�� Ŭ���Ǿ����� �ڽ��� ������ �ƴ϶��
	// �Ǵ� ��Ŀ���� �ڽ�,�ڽĵ� �����ϳ��� ���� �ʾҴٸ� �ڽ��� �������.
	// �ڽ��� ��Ŀ���� �޾Ƶ� �����. 
}

void CGUI_ContextMenu::Render(void)
{
	if( m_Img.TexID != -1 )
	{
	RECT tmp={ (LONG)m_DestRect.left,(LONG)m_DestRect.top, (LONG)m_DestRect.right,(LONG)m_DestRect.bottom };
	g_D3D.BltSprite( m_Layer, m_Img.TexID, &m_SrcRect, tmp, m_Clr, D3D_ALPHASTATE );
	}
}


bool CGUI_ContextMenu::ProcMessage(GUIMSG& Msg)
{
	switch( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			_pViewPersonInfo =  GUI_CreateObj<CGUI_Button> ( _T("ContextMenu_ViewPerson"),
				_T("Data\\IMG\\GUI\\ContextMenu_ViewPersonInfo.Ani"),3,3,-1,-1 );
			this->AddChild(_pViewPersonInfo);

			_pWhisper = GUI_CreateObj<CGUI_Button>( _T("ContextMenu_Whisper"),
				_T("Data\\IMG\\GUI\\ContextMenu_Whisper.Ani"),3,21,-1,-1 );
			this->AddChild( _pWhisper ); 

			_pKick = GUI_CreateObj<CGUI_Button>( _T("ContextMenu_Kick"),
				_T("Data\\IMG\\GUI\\ContextMenu_Kick.Ani"),3,40,-1,-1 );
			this->AddChild( _pKick ); 

			_pBlock = GUI_CreateObj<CGUI_Button>( _T("ContextMenu_Block"),
				_T("Data\\IMG\\GUI\\ContextMenu_Block.Ani"),3,60,-1,-1 );
			this->AddChild( _pBlock ); 
		}
		return true;
	case GUIMSG_DESTROY:
		return true;
	case GUIMSG_CLOSE:
		{
			CGUI_MGR::NoneFocusSetting();
			DestroyAllChild();
			CGUI_MGR::DestroyObjQueue( m_Name );
		}
		return true;

	case GUIMSG_FOCUSLEAVE:
		{
			//�� ��� �ڽ��� �˻��� ���� �����ϳ��� ��Ŀ�� �����༮�� �ִٸ� �Ҹ���� 
		CGUI_MGR::DestroyObjQueue( m_Name );
		}
		return true;

	case GUIMSG_BUTTON_EVENT:
		{
			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( Msg.dwParam1);
			// �ڽ��� ������� �⺻ ���������� ���� �޼��� ����
			CGUI_MGR::_SendMessage( GUIMSG_BUTTON_EVENT,(DWORD)pButton,0 );
			CGUI_MGR::DestroyObjQueue( GetName() );
			//if( strcmp(pButton->GetName(),"ContextMenu_ViewPerson" ) ==0 )
			//{
			//	int a=0;
			//}
			//else if( strcmp(pButton->GetName(),"ContextMenu_Whisper" ) ==0 )
			//{

			//}
			
		}return true;

	case GUIMSG_MRDOWN:
	case GUIMSG_MLDOWN:
		{
			// ���� �������� ��Ŀ�� �� 
			// �� ������ ������ ��Ŀ�� 
			int x =(int)( Msg.dwParam1 );
			int y =(int)( Msg.dwParam2 );

				m_ClickPos.x = LONG(x - m_BoundRect.left);
				m_ClickPos.y = LONG(y - m_BoundRect.top); 

			if( (x< m_BoundRect.left    || m_BoundRect.right < x ) ||
				 y< m_BoundRect.top     || m_BoundRect.bottom < y )
			{
				CGUI_MGR::DestroyObj( m_Name );
				return true;
			}

		}
		return false;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////
//
// Name: CGUI_ShowInfo
//
// Desc: �⺻���� ������ �����ֻ�
//
///////////////////////////////////////////////////////////////////////
CGUI_ShowInfo::CGUI_ShowInfo()
{
m_Gui_ClsID=GUI_PANNEL_SHOWINFO;
}
CGUI_ShowInfo::~CGUI_ShowInfo()
{}

void CGUI_ShowInfo::Process(void)
{
	// ���콺�� Ŭ���Ǿ����� �ڽ��� ������ �ƴ϶��
	// �Ǵ� ��Ŀ���� �ڽ�,�ڽĵ� �����ϳ��� ���� �ʾҴٸ� �ڽ��� �������.
	// �ڽ��� ��Ŀ���� �޾Ƶ� �����. 
}

void CGUI_ShowInfo::Render(void)
{
	if( m_Img.TexID != -1 )
	{
	RECT tmp={ (LONG)m_DestRect.left,(LONG)m_DestRect.top, (LONG)m_DestRect.right,(LONG)m_DestRect.bottom };
	g_D3D.BltSprite( m_Layer, m_Img.TexID, &m_SrcRect, tmp, m_Clr, D3D_ALPHASTATE );
	}
}

void CGUI_ShowInfo::SetInfo(PLAYERINFO* Info)
{
	if( Info == NULL ) return;
	_pAvatar->OpenUrl( Info->PlayerID, Info->Sex );
	TCHAR buff[64]={0,};
	CGUI_Text* pID = CGUI_MGR::CreateText( _T("����â"), _T("ID"),  180, 92, 0x5f000000,0xffffffff, Info->PlayerID );
	CGUI_Text* pNick = CGUI_MGR::CreateText( _T("����â"), _T("NICK"),  180, 128, 0x5f000000,0xffffffff, Info->NickName );
	wsprintf( buff, "Lv. %d", Info->level );
	CGUI_Text* pLevel = CGUI_MGR::CreateText( _T("����â"), _T("LV"),  180, 166, 0x5f000000,0xffffffff, buff );
	wsprintf( buff, "Exp %ld", Info->exp );
	CGUI_Text* pExp = CGUI_MGR::CreateText( _T("����â"), _T("Money"),  180, 204, 0x5f000000,0xffffffff, buff );
	wsprintf( buff,"%d�� %d�� %d�� ",Info->win+Info->lose, Info->win,Info->lose);//+Info.draw
	CGUI_Text* pWin = CGUI_MGR::CreateText( _T("����â"), _T("WPER"),  180, 242, 0x5f000000,0xffffffff, buff );


}

bool CGUI_ShowInfo::ProcMessage(GUIMSG& Msg)
{
	switch( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			_pAvatar = GUI_CreateObj< CGUI_Avatar>( "����â_�ƹ�Ÿ",NULL,22,92,130,160);
			this->AddChild( _pAvatar );
			_pClose = GUI_CreateObj<CGUI_Button >( _T("��������_Ȯ��"),_T("Data\\IMG\\gui\\Button_BlueOk.ani") ,134,275,-1,-1 );
			this->AddChild( _pClose );  

			CGUI_MGR::SetGuiFocus(_T("��������_Ȯ��") );
			CGUI_MGR::MoveCenter( _T("����â") );

			_DefaultButton = "��������_Ȯ��";
		}
		return true;
	case GUIMSG_DESTROY:
		return true;
	case GUIMSG_CLOSE:
		{
			CGUI_MGR::NoneFocusSetting();
			DestroyAllChild();
			CGUI_MGR::DestroyObjQueue( m_Name );
		}
		return true;

	case GUIMSG_FOCUSLEAVE:
		{
			//�� ��� �ڽ��� �˻��� ���� �����ϳ��� ��Ŀ�� �����༮�� �ִٸ� �Ҹ���� 
//		CGUI_MGR::DestroyObjQueue( m_Name );
		}
		return true;

	case GUIMSG_BUTTON_EVENT:
		{

			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( Msg.dwParam1);
			if( strcmp(pButton->GetName(),"��������_Ȯ��" ) ==0 )
			{

				// ���⼭ �ٸ� ������Ʈ�� �����غ귯 ��� 
					CGUI_MGR::NoneFocusSetting();
					CGUI_MGR::DestroyObjQueue( m_Name );
			}
				return true;
		}

	}
	return CGUI_Pannel::ProcMessage(Msg);
}
/////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//
// Name: CGUI_Console
//
// Desc: �⺻���� ��Ʈ���� ����ִ� ����â�̸� ��ư�� �ΰ��� ����� 
//       ���� �Ҽ� �ִ� 
///////////////////////////////////////////////////////////////////////
CGUI_Console::CGUI_Console()
{
	m_Gui_ClsID=GUI_PANNEL_CONSOLE;
}
CGUI_Console::~CGUI_Console()
{}
/*
void CGUI_Console::Process(void)
{
	// ���콺�� Ŭ���Ǿ����� �ڽ��� ������ �ƴ϶��
	// �Ǵ� ��Ŀ���� �ڽ�,�ڽĵ� �����ϳ��� ���� �ʾҴٸ� �ڽ��� �������.
	// �ڽ��� ��Ŀ���� �޾Ƶ� �����. 
}
*/
/*
void CGUI_Console::Render(void)
{
	if( m_Img.TexID != -1 )
	{
	RECT tmp={ (LONG)m_DestRect.left,(LONG)m_DestRect.top, (LONG)m_DestRect.right,(LONG)m_DestRect.bottom };
	g_D3D.BltSprite( m_Layer, m_Img.TexID, &m_SrcRect, tmp, m_Clr, D3D_ALPHASTATE );
	}
}*/

void CGUI_Console::SetCommand(ICommand* pCommand)
{
	if( pCommand == NULL ) return ;
	CGUI_Button* pBuff =(CGUI_Button*) CGUI_MGR::FindGui("�ܼ�â_�ݱ�");
	if( pBuff )
	{
		pBuff->SetCommand( pCommand );
	}
}

void CGUI_Console::AddString(const TCHAR* szStr)
{
	_pList->AddString( (TCHAR*)szStr, 0x9f0080ff, 0xffffffff  );
}

bool CGUI_Console::ProcMessage(GUIMSG& Msg)
{
	switch( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			SetModal(true);
			SetLayer( D3D_INTERFACE_LAYER1 );
			CGUI_MGR::SetGuiFocus(_T("��������_Ȯ��") );

			_pList = GUI_CreateObj<CGUI_ListBox>(  "�ܼ�â_�۸���Ʈ", NULL ,67,38, 230,124 ); 
			AddChild( _pList );

			_pClose = GUI_CreateObj<CGUI_Button>( _T("�ܼ�â_�ݱ�"),	_T("data\\IMG\\gui\\button_BlueOk.ani") ,134,182,-1,-1 );
			AddChild( _pClose );
			CGUI_MGR::MoveCenter( _T("�ܼ�â") );
		}
		return true;
	//case GUIMSG_DESTROY:
	//return true;
	//case GUIMSG_CLOSE:
	//	{
	//		CGUI_MGR::NoneFocusSetting();
	//		DestroyAllChild();
	//		CGUI_MGR::DestroyObjQueue( m_Name );
	//	}
	//	return true;

	case GUIMSG_BUTTON_EVENT:
		{

			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( Msg.dwParam1);
			if( strcmp(pButton->GetName(),"�ܼ�â_�ݱ�" ) ==0 )
			{
				// ���⼭ �ٸ� ������Ʈ�� �����غ귯 ��� 
					CGUI_MGR::NoneFocusSetting();
				CGUI_MGR::DestroyObj( m_Name );
			}
				return true;
		}

	}
	return CGUI_Pannel::ProcMessage(Msg);
}
/////////////////////////////////////////////////////////////////////////////////////////


//bool CGUI_Slide::ProcMessage(GUIMSG& pMsg)
//{
//	// ���콺 Ŭ���� �������� 
//	// ���콺 ���갡 ��������
//	// Ű���� �� �Ʒ��� �������� 
//	// ������ ���ٿ ���� 
//	// Ȩ, ���嵵 ���� 
//
//	if( ProcMessage_Child( pMsg ) == true ) 
//		return true;
//
//	static POINT ScrollDown={0,};
//	switch( pMsg.dwMsg )
//	{
//
//	case GUIMSG_MOVEWND:
//		{
//			MoveGUI( (float)pMsg.dwParam1, (float)pMsg.dwParam2 );
//		}
//		return false;
//
//	case GUIMSG_SCROLL_PREV: // �̰ɷ� �޼����� ������..
//		{
//			IGUI_Object* pSuperParent = m_pParent->GetParent();
//			if( pSuperParent == NULL ) pSuperParent=m_pParent;
//
//			while( pSuperParent->GetParent() != NULL )
//				pSuperParent = pSuperParent ->GetParent();
//
//			if( CGUI_MGR::GetCurrFocus() == this || CGUI_MGR::GetCurrFocus() == pSuperParent )
//			OnUP();
//			else 
//			return false;
//		}
//		return true;
//
//	case GUIMSG_SCROLL_NEXT:
//		{
//			IGUI_Object* pSuperParent = m_pParent->GetParent();
//			if( pSuperParent == NULL ) pSuperParent=m_pParent;
//
//			while( pSuperParent->GetParent() != NULL )
//				pSuperParent = pSuperParent ->GetParent();
//
//			if( CGUI_MGR::GetCurrFocus() == this || CGUI_MGR::GetCurrFocus() == pSuperParent )
//			OnDown();
//			else 
//			return false;
//		}
//		return true;
//
//	case GUIMSG_MLDOWN:
//		{
//			int x =int( pMsg.dwParam1 );
//			int y =int( pMsg.dwParam2 );
//			
//			if( m_BoundRect.left <= x  &&
//				x <= m_BoundRect.right &&
//				m_BoundRect.top  <= y  &&
//				y <= m_BoundRect.bottom )
//			{
//				// ���� 1���� �θ�� ����Ʈ �ڽ��̳� ���� �θ� ���� �ƴϸ� 
//				// �׿��� ��Ŀ���� ������ �ؾ� �Ѵ�.
//			IGUI_Object* pSuperParent = m_pParent->GetParent();
//			if( pSuperParent == NULL ) pSuperParent=m_pParent;
//
//			while( pSuperParent->GetParent() != NULL )
//				pSuperParent = pSuperParent ->GetParent();
//			
//				CGUI_MGR::SetGuiFocus( pSuperParent->GetName() ); // ���� �ְ� �θ� ��Ŀ���� ������ �ؾ���
//					
//			}
//		}
//	return false;
//	case  GUIMSG_DRAG:
//		{
//			if( m_pBar == NULL  ) return false;
//			if( pMsg.dwParam1 < m_BoundRect.left  ||
//				pMsg.dwParam1 > m_BoundRect.right ||
//				pMsg.dwParam2 < m_BoundRect.top   ||
//				pMsg.dwParam2 > m_BoundRect.bottom )
//			{
//				return false; // ������ �ش��ϴ� �޼����� �ƴϴ� 
//			}
//			// ���� ���콺 ��ġ�� ��ũ�� �� ������ 100 �������� ��ġ�� ���ϰ�
//			// �� 100���� ������ ���� ��ũ�� �ٷ� ȯ�� �����Ѵ�. �̺κ��� �Ǿ� �����Ƿ� 
//			// ��ũ�ѹ� ���������� 100���� ��ġ�� ������.
//			int mousex = pMsg.dwParam1;
//			int mousey = pMsg.dwParam2;
//			//				���콺��ġ - �ɼ���ġ / �ٿ�巺Ʈ ���� - 
//			int intervaly = m_pBar->GetFirstClick().y;
//			float Bar_y = m_pBar->GetY() - m_BoundRect.top;
//			float Bar_Height = m_pBar->GetHeight();
//			float newy = 0.0f;
//			float Enable_Height = GetHeight()-Bar_Height ;
//			m_PointPos = Bar_y/Enable_Height*100.0f ;
//
//			int y =int( pMsg.dwParam2 - intervaly ); // ������ ��������� �մϴ�.
//
//			if( y < m_BoundRect.top )// �� ���ų� Ŀ�� �ϰ� 
//			{
//				y = int(m_BoundRect.top); 
//			}
//			else if( y+Bar_Height>m_BoundRect.bottom )
//			{
//				y = m_BoundRect.bottom - Bar_Height;
//			}
//			
//			m_pBar->SetY( float(y) ); //m_BoundRect.top+
//
//			// ����Ʈ�� ���������Ƿ� 
//			// ��ũ���� ������ ��ŭ ������ �ش�. 
//			// ����Ʈ�� �����ϼ� �ִ� ������ ��ü DESTRECT ���� ���Ʒ� ���� point_Height /2 �� ��ŭ �̴�.
//			// �׸�ŭ�� �̵������� ������ ��ü�� ���� ���� ��ġ�� ��ü������ ������� ��ġ�� 2048 ������ �Ѵ�.
//			// 2�� �¼� ���̹Ƿ� / ����� �Ҽ��� 5�ڸ� ������ ������ ������ ���� 
//			// ���� ���콺 ��ġ???? �巹�� �ÿ��� 100������ ���� �̵���... 
//			//RECT PointRt={0, 0, (long)m_pBar->GetWidth(),  long(GetHeight() - (point_Height)) };
//			//float	tmpY = (m_pBar->GetY() +(m_pBar->GetHeight()/2) )- (m_BoundRect.top + (m_pBar->GetHeight()/2) );
//			//m_PointPos = (float)tmpY / PointRt.bottom*2048;
//
//			GUIMSG Msg;
//			Msg.dwMsg = GUIMSG_SCROLL_VERTICAL_DRAG;
//			Msg.dwParam1 = m_PointPos;
//			m_pParent->ProcMessage( Msg );
//			return true;
//		}
//	}
//
//	return false;
//}



///////////////////////////////////////////////////////////////////////
//
// Name: CGUI_Console
//
// Desc: �⺻���� ��Ʈ���� ����ִ� ����â�̸� ��ư�� �ΰ��� ����� 
//       ���� �Ҽ� �ִ� 
///////////////////////////////////////////////////////////////////////
CGUI_MakeRoom::CGUI_MakeRoom()
{
	m_Gui_ClsID=GUI_PANNEL_MAKEROOM;
}
CGUI_MakeRoom::~CGUI_MakeRoom()
{}

void CGUI_MakeRoom::Process(void)
{
}


void CGUI_MakeRoom::Render(void)
{
	if( m_Img.TexID != -1 )
	{
	RECT tmp={ (LONG)m_DestRect.left,(LONG)m_DestRect.top, (LONG)m_DestRect.right,(LONG)m_DestRect.bottom };
	g_D3D.BltSprite( m_Layer, m_Img.TexID, &m_SrcRect, tmp, m_Clr, D3D_ALPHASTATE );

	char buff[32]={0,};
	itoa(_PlayerCnt, buff, 10);
	g_D3D.BltText( m_Layer, m_DestRect.left+209,m_DestRect.top+182, 0xff000000, buff );
	}
}
static std::string haha[]={"�츮�Բ��ؿ�","ĳġ��¯¯¯","���ѱ��θ�����"};
bool CGUI_MakeRoom::ProcMessage(GUIMSG& Msg)
{
	switch( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			_PlayerCnt = 2;
			SetModal(true);
			SetLayer( D3D_INTERFACE_LAYER1 );
			CGUI_Edit* pEditRoomPass = GUI_CreateObj<CGUI_Edit>(_T("���ȣ�Է�â"), _T("Data\\IMG\\etc\\WhiteFade.ani"),70,133,170,18 );
			pEditRoomPass->SetAttirb(CGUI_Edit::PASSWD);
			pEditRoomPass->ClearString();
			pEditRoomPass->SetLimitStr( MAX_ROOM_PASSWORD -1 );
			pEditRoomPass->SetBoundRect(91,141,91+149,141+18); 
			pEditRoomPass->SetBackClr( 0x00ff0000);
			this->AddChild( pEditRoomPass );

			CGUI_Edit* pEditRoomName =GUI_CreateObj<CGUI_Edit>( _T("���̸��Է�â"),_T("Data\\IMG\\etc\\WhiteFade.ani"),70,85,170,18 );
			pEditRoomName ->SetAttirb( CGUI_Edit::NORMAL );
			pEditRoomName->ClearString();
			pEditRoomName->SetLimitStr( MAX_ROOM_NAME -1 );
			pEditRoomName->SetString( haha[rand()%3].c_str());
			pEditRoomName->SetBoundRect(91,67,91+149,67+18); 
			pEditRoomName->SetBackClr( 0x00ff0000);
			this->AddChild( pEditRoomName );

			CGUI_RadioButton* pButt_Public = GUI_CreateObj<CGUI_RadioButton>(_T("�游���â_����"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 100,108,-1,-1 );
			pButt_Public->SetCheck(true);
			this->AddChild( pButt_Public );
			CGUI_RadioButton* pButt_Private = GUI_CreateObj<CGUI_RadioButton>(_T("�游���â_�����"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 166,108,-1,-1 );
			pButt_Private->SetCheck(false);
			this->AddChild( pButt_Private );

			CGUI_Slide* pSlide = GUI_CreateObj<CGUI_Slide>(_T("�游���â_�ο���"),_T("Data\\IMG\\GUI\\Button_Red_Slide.ani"),106,180,-1,-1 );
			this->AddChild( pSlide );
			pSlide->SetAllowRegion(106,178);
			pSlide->SetRate(0);

			CGUI_Button* pButton_Slide_Min =  GUI_CreateObj<CGUI_Button> ( _T("�游���â_�ο���_�۰�"),
				_T("Data\\IMG\\GUI\\Button_Red_Slide_Left.Ani"),89,178,-1,-1 );
			this->AddChild( pButton_Slide_Min );

			CGUI_Button* pButton_Slide_Max=  GUI_CreateObj<CGUI_Button> ( _T("�游���â_�ο���_ũ��"),
				_T("Data\\IMG\\GUI\\Button_Red_Slide_Right.Ani"),178,178,-1,-1 );
			this->AddChild( pButton_Slide_Max );



			CGUI_Button* pButtonMakeRoomOk =  GUI_CreateObj<CGUI_Button> ( _T("�游���âȮ��"),
				_T("Data\\IMG\\GUI\\Button_RedOk.Ani"),62,215,-1,-1 );
			this->AddChild( pButtonMakeRoomOk  );

			CGUI_Button* pButtonMakeRoomCancel = GUI_CreateObj<CGUI_Button>( _T("�游���â���"),
				_T("Data\\IMG\\GUI\\Button_RedCancel.Ani"),136,215,-1,-1 );
			this->AddChild( pButtonMakeRoomCancel ); 

			CGUI_MGR::SetGuiFocus( _T("���̸��Է�â") );

			OBJMSG msg;
			msg.dwMsg = GUIMSG_FOCUSENTER;
			pEditRoomName->ProcMessage( msg );
			CGUI_MGR::MoveCenter( m_Name );
			CGUI_MGR::ReCalcEditList( this );
			_DefaultButton = "�游���âȮ��";
		}
		return true;
	case GUIMSG_DESTROY:
	{
			CGUI_MGR::NoneFocusSetting();
			DestroyAllChild();
			CGUI_MGR::DestroyObjQueue( m_Name );
		}
	return true;
	//case GUIMSG_CLOSE:
	//	{
	//		CGUI_MGR::NoneFocusSetting();
	//		DestroyAllChild();
	//		CGUI_MGR::DestroyObjQueue( m_Name );
	//	}
	//	return true;
	case GUIMSG_SCROLL_HORIZON_DRAG:
		{
			CGUI_Slide* pSlide = (CGUI_Slide*)Msg.dwParam1;
			int rate = Msg.dwParam2;
			// 100 �ۼ�Ʈ�� ������ 
			// 2,3,4,5,6 �̴ϱ��
			//20���θ� 2
			//40���θ� 3
			//60���θ� 4
			//80���θ� 5
			//100���θ� 6
			if( rate  <= 20 ) _PlayerCnt = 2;
			else
			if( rate >20 && rate <=40 ) _PlayerCnt = 3;
			else
			if( rate >40 && rate <=60 ) _PlayerCnt = 4;
			else
			if( rate >60 && rate <=80 ) _PlayerCnt = 5;
			else
			if( rate >80 && rate <=100 ) _PlayerCnt = 6;
			//_PlayerCnt  = (rate /20) +1;
		}
		return true;

	case GUIMSG_BUTTON_EVENT:
		{
			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( Msg.dwParam1);
			if( strcmp(pButton->GetName(),"�游���âȮ��" ) ==0 )
			{
				// ���⼭ �ٸ� ������Ʈ�� �����غ귯 ��� 
				CGUI_MGR::_SendMessage( GUIMSG_BUTTON_EVENT,(DWORD)pButton,0 );
				CGUI_MGR::DestroyObj( m_Name );
			}else if( strcmp(pButton->GetName(),"�游���â���" ) ==0 )
			{
				CGUI_MGR::_SendMessage( GUIMSG_BUTTON_EVENT,(DWORD)pButton,0 );
				CGUI_MGR::DestroyObjQueue( m_Name );
			}
			else if( strcmp(pButton->GetName(), _T("�游���â_�ο���_�۰�") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("�游���â_�ο���")	);
				if( pSlide)
				{
					int rr = pSlide->GetRate();
					pSlide->SetRate( rr-20 );
				}
			}
			else if( strcmp(pButton->GetName(),_T("�游���â_�ο���_ũ��") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("�游���â_�ο���") );
				if( pSlide)
				{
					int rr = pSlide->GetRate();
					pSlide->SetRate( rr+20 );
				}
			}

				return true;
		}

	}
	return CGUI_Pannel::ProcMessage(Msg);
}
/////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//
// Name: CGUI_Option
//
// Desc: ������ �ɼ��� �����Ҽ����� ���̺� ���½ÿ� �Ӽ��� �־��� 
//       ���� ���,��Ʈ�ѷ��� ���һ� 
///////////////////////////////////////////////////////////////////////
CGUI_Option::CGUI_Option()
{
	m_Gui_ClsID=GUI_PANNEL_OPTION;
}
CGUI_Option::~CGUI_Option()
{}

void CGUI_Option::Process(void)
{
}


void CGUI_Option::Render(void)
{
	if( m_Img.TexID != -1 )
	{
	RECT tmp={ (LONG)m_DestRect.left,(LONG)m_DestRect.top, (LONG)m_DestRect.right,(LONG)m_DestRect.bottom };
	g_D3D.BltSprite( m_Layer, m_Img.TexID, &m_SrcRect, tmp, m_Clr, D3D_ALPHASTATE );
	}
}

void CGUI_Option::RestoreValue(void)
{
	_RestoreValue = _CurrentValue;
}
void CGUI_Option::SetValue_BGMVolume(int vol) 
{
	if( vol <0 ) vol = 0;
	if( vol >100) vol = 100;
	_CurrentValue._BgmVolume = vol;  
	CGUI_Slide* pSlide =(CGUI_Slide*) CGUI_MGR::FindGui("�ɼ�â_���������");
	if( pSlide )
		pSlide ->SetRate( vol );
}
void CGUI_Option::SetValue_EFFVolume(int vol)
{ 
	if( vol <0 ) vol = 0;
	if( vol >100) vol = 100;
	_CurrentValue._EffVolume = vol; 
	CGUI_Slide* pSlide =(CGUI_Slide*) CGUI_MGR::FindGui("�ɼ�â_ȿ��������");
	if( pSlide )
		pSlide ->SetRate( vol );
}
void CGUI_Option::SetValue_FullMode(bool bFull)
{
	_CurrentValue._bFullMode= bFull;  
	CGUI_RadioButton* pRadio=NULL;
	if(bFull)
	pRadio = (CGUI_RadioButton*) CGUI_MGR::FindGui("�ɼ�â_Ǯ" );
	else
	pRadio = (CGUI_RadioButton*) CGUI_MGR::FindGui("�ɼ�â_â" );

	if( pRadio )pRadio ->SetCheck( true );
}
void CGUI_Option::SetValue_BlockWhisper(bool bWhisper)
{ 
	_CurrentValue._bWhisper= bWhisper; 
	CGUI_CheckButton* pWhisper =(CGUI_CheckButton*) CGUI_MGR::FindGui("�ɼ�â_�Ӹ��ź�" );
	if( pWhisper )pWhisper ->SetCheck( bWhisper );
}
void CGUI_Option::SetValue_BlockInvite(bool bInvite )
{
	_CurrentValue._bInvite= bInvite; 
	CGUI_CheckButton* pInvite =(CGUI_CheckButton*) CGUI_MGR::FindGui("�ɼ�â_�ʴ�ź�" );
	if( pInvite )pInvite->SetCheck( bInvite );
}
	
bool CGUI_Option::ProcMessage(GUIMSG& Msg)
{
	switch( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			SetModal(true);
			SetLayer( D3D_INTERFACE_LAYER1 );
	
			CGUI_Button* pButtonMakeRoomOk =  GUI_CreateObj<CGUI_Button> ( _T("�ɼ�â_Ȯ��"),	_T("Data\\IMG\\GUI\\Button_BlueOk.Ani"),97,247,-1,-1 );
			this->AddChild( pButtonMakeRoomOk  );

			CGUI_Button* pButtonMakeRoomCancel = GUI_CreateObj<CGUI_Button>( _T("�ɼ�â_���"),_T("Data\\IMG\\GUI\\Button_BlueCancel.Ani"),171,247,-1,-1 );
			this->AddChild( pButtonMakeRoomCancel ); 

			CGUI_RadioButton* pButt_full= GUI_CreateObj<CGUI_RadioButton>(_T("�ɼ�â_Ǯ"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 114,166,-1,-1 );
			pButt_full->SetGroupName( _T("�ɼ�â_ȭ����") );
			pButt_full->SetCheck( (g_D3D.IsFull()==true)?true:false );
			this->AddChild( pButt_full );

			CGUI_RadioButton* pButt_window = GUI_CreateObj<CGUI_RadioButton>(_T("�ɼ�â_â"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 217,166,-1,-1 );
			pButt_window->SetGroupName( _T("�ɼ�â_ȭ����") );
			pButt_window->SetCheck( (g_D3D.IsFull()==true)?false:true);
			this->AddChild( pButt_window );


			CGUI_CheckButton* pButt_blockinvite = GUI_CreateObj<CGUI_CheckButton>(_T("�ɼ�â_�ʴ�ź�"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 114,209,-1,-1 );
			pButt_blockinvite ->SetCheck(false );
			this->AddChild( pButt_blockinvite );
			CGUI_CheckButton* pButt_blockwhisper= GUI_CreateObj<CGUI_CheckButton>(_T("�ɼ�â_�Ӹ��ź�"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 217,209,-1,-1 );
			pButt_blockwhisper->SetCheck(false);
			this->AddChild( pButt_blockwhisper );


			CGUI_Slide* pSlide = GUI_CreateObj<CGUI_Slide>(_T("�ɼ�â_���������"),_T("Data\\IMG\\GUI\\Button_Blue_Slide.ani"),170,99,-1,-1 );
			pSlide->SetAllowRegion(170,242);
			this->AddChild( pSlide );
			pSlide->SetRate(0);
			CGUI_Button* pButton_Slide_BGMMin =  GUI_CreateObj<CGUI_Button> ( _T("�ɼ�â_�����_�۰�"),
				_T("Data\\IMG\\GUI\\Button_Blue_Slide_Left.Ani"),153,97,-1,-1 );
			this->AddChild( pButton_Slide_BGMMin  );

			CGUI_Button* pButton_Slide_BGMMax=  GUI_CreateObj<CGUI_Button> ( _T("�ɼ�â_�����_ũ��"),
				_T("Data\\IMG\\GUI\\Button_Blue_Slide_Right.Ani"),242,97,-1,-1 );
			this->AddChild( pButton_Slide_BGMMax );


			CGUI_Slide* pSlide1 = GUI_CreateObj<CGUI_Slide>(_T("�ɼ�â_ȿ��������"),_T("Data\\IMG\\GUI\\Button_Blue_Slide.ani"),170,123,-1,-1 );
			pSlide1->SetAllowRegion(170,242);
			this->AddChild( pSlide1 );
			pSlide1->SetRate(0);
			CGUI_Button* pButton_Slide_EffMin =  GUI_CreateObj<CGUI_Button> ( _T("�ɼ�â_ȿ����_�۰�"),
				_T("Data\\IMG\\GUI\\Button_Blue_Slide_Left.Ani"),153,121,-1,-1 );
			this->AddChild( pButton_Slide_EffMin );

			CGUI_Button* pButton_Slide_EffMax=  GUI_CreateObj<CGUI_Button> ( _T("�ɼ�â_ȿ����_ũ��"),
				_T("Data\\IMG\\GUI\\Button_Blue_Slide_Right.Ani"),242,121,-1,-1 );
			this->AddChild( pButton_Slide_EffMax );

			CGUI_MGR::MoveCenter( this->m_Name );
		}
		return true;
	//case GUIMSG_DESTROY:
	//return true;
	//case GUIMSG_CLOSE:
	//	{
	//		CGUI_MGR::NoneFocusSetting();
	//		DestroyAllChild();
	//		CGUI_MGR::DestroyObjQueue( m_Name );
	//	}
	//	return true;

	case GUIMSG_BUTTON_EVENT:
		{
			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( Msg.dwParam1);
			if( strcmp(pButton->GetName(),"�ɼ�â_Ȯ��" ) ==0 )
			{
				// ���⼭ �ٸ� ������Ʈ�� �����غ귯 ��� 
				
				RestoreValue();// ���簪 �����ϸ� 
				CGUI_MGR::DestroyObjQueue( m_Name );
			}else if( strcmp(pButton->GetName(),"�ɼ�â_���" ) ==0 )
			{
				_CurrentValue = _RestoreValue ;
				CGUI_MGR::DestroyObjQueue( m_Name );
			}
			else if( strcmp(pButton->GetName(), _T("�ɼ�â_�����_�۰�") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("�ɼ�â_���������")	);
				if( pSlide)
				{
					int rr = pSlide->GetRate();
				//	pSlide->SetRate( rr-10 );
					this->SetValue_BGMVolume( rr -10 );
				}
			}
			else if( strcmp(pButton->GetName(),_T("�ɼ�â_�����_ũ��") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("�ɼ�â_���������") );
				if( pSlide)
				{
					int rr = pSlide->GetRate();
					//pSlide->SetRate( rr+10 );
					this->SetValue_BGMVolume( rr +10 );
				}
			}
			else if( strcmp(pButton->GetName(), _T("�ɼ�â_ȿ����_�۰�") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("�ɼ�â_ȿ��������")	);
				if( pSlide)
				{
					int rr = pSlide->GetRate();
					//pSlide->SetRate( rr-10 );
					this->SetValue_EFFVolume( rr -10);
				}
			}
			else if( strcmp(pButton->GetName(),_T("�ɼ�â_ȿ����_ũ��") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("�ɼ�â_ȿ��������") );
				if( pSlide)
				{
					int rr = pSlide->GetRate();
					//pSlide->SetRate( rr+10 );
					this->SetValue_EFFVolume( rr + 10);
				}
			}

			return true;
		}

	}
	return CGUI_Pannel::ProcMessage(Msg);
}
/////////////////////////////////////////////////////////////////////////////////////////

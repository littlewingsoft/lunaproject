
#include <guistdafx.h>

////////////////////////////////////////////////
//
// 판넬 관련 클래스 
// 모든 버튼들은 암것도 모르고요 
// 판넬에서는 디폴트 버튼의 포인터를 설정할수있다.
// 엔터가 들어오면 디폴트 버튼이 메세지를 접수 ㅋㅋ
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
			// 만일 엔터키라면 
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
				//pButt->ProcMessage( msg );// 메세지 큐에 넣어놓음 

				//msg.dwMsg = GUIMSG_MLUP;
				//pButt->ProcMessage( msg );// 메세지 큐에 넣어놓음 

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
			// 나의 영역밖이면 무시 합니다.
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
			// 자신의 차일드 들에게 모조리 메세지를 날려준다.
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
			// 나를 눌렀으면 포커스 온 
			// 내 영역에 들어오면 포커스 
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
	// 메세지중 닫으라는게 들어오면 밑에 것들 모두 없애고 
	// 최후에 자신은 메니져에서 사라진다.
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
	// 마우스가 클릭되었을때 자신의 영역이 아니라면
	// 또는 포커스를 자신,자식들 둘중하나가 받지 않았다면 자신은 사라진다.
	// 자식이 포커스를 받아도 사라짐. 
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
			//내 모든 자식을 검사해 본뒤 그중하나가 포커스 가진녀석이 있다면 소멸안함 
		CGUI_MGR::DestroyObjQueue( m_Name );
		}
		return true;

	case GUIMSG_BUTTON_EVENT:
		{
			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( Msg.dwParam1);
			// 자신은 사라지고 기본 지정신으로 현재 메세지 전송
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
			// 나를 눌렀으면 포커스 온 
			// 내 영역에 들어오면 포커스 
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
// Desc: 기본적인 정보를 보여주삼
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
	// 마우스가 클릭되었을때 자신의 영역이 아니라면
	// 또는 포커스를 자신,자식들 둘중하나가 받지 않았다면 자신은 사라진다.
	// 자식이 포커스를 받아도 사라짐. 
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
	CGUI_Text* pID = CGUI_MGR::CreateText( _T("인포창"), _T("ID"),  180, 92, 0x5f000000,0xffffffff, Info->PlayerID );
	CGUI_Text* pNick = CGUI_MGR::CreateText( _T("인포창"), _T("NICK"),  180, 128, 0x5f000000,0xffffffff, Info->NickName );
	wsprintf( buff, "Lv. %d", Info->level );
	CGUI_Text* pLevel = CGUI_MGR::CreateText( _T("인포창"), _T("LV"),  180, 166, 0x5f000000,0xffffffff, buff );
	wsprintf( buff, "Exp %ld", Info->exp );
	CGUI_Text* pExp = CGUI_MGR::CreateText( _T("인포창"), _T("Money"),  180, 204, 0x5f000000,0xffffffff, buff );
	wsprintf( buff,"%d전 %d승 %d패 ",Info->win+Info->lose, Info->win,Info->lose);//+Info.draw
	CGUI_Text* pWin = CGUI_MGR::CreateText( _T("인포창"), _T("WPER"),  180, 242, 0x5f000000,0xffffffff, buff );


}

bool CGUI_ShowInfo::ProcMessage(GUIMSG& Msg)
{
	switch( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			_pAvatar = GUI_CreateObj< CGUI_Avatar>( "인포창_아바타",NULL,22,92,130,160);
			this->AddChild( _pAvatar );
			_pClose = GUI_CreateObj<CGUI_Button >( _T("정보보기_확인"),_T("Data\\IMG\\gui\\Button_BlueOk.ani") ,134,275,-1,-1 );
			this->AddChild( _pClose );  

			CGUI_MGR::SetGuiFocus(_T("정보보기_확인") );
			CGUI_MGR::MoveCenter( _T("인포창") );

			_DefaultButton = "정보보기_확인";
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
			//내 모든 자식을 검사해 본뒤 그중하나가 포커스 가진녀석이 있다면 소멸안함 
//		CGUI_MGR::DestroyObjQueue( m_Name );
		}
		return true;

	case GUIMSG_BUTTON_EVENT:
		{

			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( Msg.dwParam1);
			if( strcmp(pButton->GetName(),"정보보기_확인" ) ==0 )
			{

				// 여기서 다른 오브젝트를 생성해브러 써글 
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
// Desc: 기본적인 스트링을 띄워주는 공지창이며 버튼에 부가적 기능을 
//       셋팅 할수 있다 
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
	// 마우스가 클릭되었을때 자신의 영역이 아니라면
	// 또는 포커스를 자신,자식들 둘중하나가 받지 않았다면 자신은 사라진다.
	// 자식이 포커스를 받아도 사라짐. 
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
	CGUI_Button* pBuff =(CGUI_Button*) CGUI_MGR::FindGui("콘솔창_닫기");
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
			CGUI_MGR::SetGuiFocus(_T("정보보기_확인") );

			_pList = GUI_CreateObj<CGUI_ListBox>(  "콘솔창_글리스트", NULL ,67,38, 230,124 ); 
			AddChild( _pList );

			_pClose = GUI_CreateObj<CGUI_Button>( _T("콘솔창_닫기"),	_T("data\\IMG\\gui\\button_BlueOk.ani") ,134,182,-1,-1 );
			AddChild( _pClose );
			CGUI_MGR::MoveCenter( _T("콘솔창") );
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
			if( strcmp(pButton->GetName(),"콘솔창_닫기" ) ==0 )
			{
				// 여기서 다른 오브젝트를 생성해브러 써글 
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
//	// 마우스 클릭이 들어왔을때 
//	// 마우스 무브가 들어왔을때
//	// 키보드 위 아래가 들어왔을때 
//	// 페이지 업다운도 포함 
//	// 홈, 엔드도 포함 
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
//	case GUIMSG_SCROLL_PREV: // 이걸로 메세지가 들어오믄..
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
//				// 나의 1차적 부모는 리스트 박스이나 그의 부모가 널이 아니면 
//				// 그에게 포커스를 가지게 해야 한다.
//			IGUI_Object* pSuperParent = m_pParent->GetParent();
//			if( pSuperParent == NULL ) pSuperParent=m_pParent;
//
//			while( pSuperParent->GetParent() != NULL )
//				pSuperParent = pSuperParent ->GetParent();
//			
//				CGUI_MGR::SetGuiFocus( pSuperParent->GetName() ); // 나의 최고 부모가 포커스를 가지게 해야함
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
//				return false; // 나한테 해당하는 메세지가 아니다 
//			}
//			// 실제 마우스 위치로 스크롤 바 영역의 100 분율기준 위치를 구하고
//			// 그 100분율 기준을 실제 스크롤 바로 환산 적용한다. 이부분은 되어 있으므로 
//			// 스크롤바 영역에서의 100분율 위치만 구해줌.
//			int mousex = pMsg.dwParam1;
//			int mousey = pMsg.dwParam2;
//			//				마우스위치 - 옵셋위치 / 바운드렉트 보톰 - 
//			int intervaly = m_pBar->GetFirstClick().y;
//			float Bar_y = m_pBar->GetY() - m_BoundRect.top;
//			float Bar_Height = m_pBar->GetHeight();
//			float newy = 0.0f;
//			float Enable_Height = GetHeight()-Bar_Height ;
//			m_PointPos = Bar_y/Enable_Height*100.0f ;
//
//			int y =int( pMsg.dwParam2 - intervaly ); // 간격을 유지해줘야 합니다.
//
//			if( y < m_BoundRect.top )// 는 같거나 커야 하고 
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
//			// 포인트를 움직였으므로 
//			// 스크롤이 움직인 만큼 영향을 준다. 
//			// 포인트가 움직일수 있는 영역은 전체 DESTRECT 에서 위아래 각각 point_Height /2 한 만큼 이다.
//			// 그만큼을 이동가능한 영역의 전체로 보고 현재 위치는 전체에서의 상대적인 위치의 2048 분율로 한다.
//			// 2의 승수 승이므로 / 연산시 소숫점 5자리 까지는 오차가 생기지 않음 
//			// 현재 마우스 위치???? 드레그 시에는 100분율로 역시 이동함... 
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
// Desc: 기본적인 스트링을 띄워주는 공지창이며 버튼에 부가적 기능을 
//       셋팅 할수 있다 
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
static std::string haha[]={"우리함께해요","캐치미짱짱짱","대한국민만세다"};
bool CGUI_MakeRoom::ProcMessage(GUIMSG& Msg)
{
	switch( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			_PlayerCnt = 2;
			SetModal(true);
			SetLayer( D3D_INTERFACE_LAYER1 );
			CGUI_Edit* pEditRoomPass = GUI_CreateObj<CGUI_Edit>(_T("방암호입력창"), _T("Data\\IMG\\etc\\WhiteFade.ani"),70,133,170,18 );
			pEditRoomPass->SetAttirb(CGUI_Edit::PASSWD);
			pEditRoomPass->ClearString();
			pEditRoomPass->SetLimitStr( MAX_ROOM_PASSWORD -1 );
			pEditRoomPass->SetBoundRect(91,141,91+149,141+18); 
			pEditRoomPass->SetBackClr( 0x00ff0000);
			this->AddChild( pEditRoomPass );

			CGUI_Edit* pEditRoomName =GUI_CreateObj<CGUI_Edit>( _T("방이름입력창"),_T("Data\\IMG\\etc\\WhiteFade.ani"),70,85,170,18 );
			pEditRoomName ->SetAttirb( CGUI_Edit::NORMAL );
			pEditRoomName->ClearString();
			pEditRoomName->SetLimitStr( MAX_ROOM_NAME -1 );
			pEditRoomName->SetString( haha[rand()%3].c_str());
			pEditRoomName->SetBoundRect(91,67,91+149,67+18); 
			pEditRoomName->SetBackClr( 0x00ff0000);
			this->AddChild( pEditRoomName );

			CGUI_RadioButton* pButt_Public = GUI_CreateObj<CGUI_RadioButton>(_T("방만들기창_공개"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 100,108,-1,-1 );
			pButt_Public->SetCheck(true);
			this->AddChild( pButt_Public );
			CGUI_RadioButton* pButt_Private = GUI_CreateObj<CGUI_RadioButton>(_T("방만들기창_비공개"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 166,108,-1,-1 );
			pButt_Private->SetCheck(false);
			this->AddChild( pButt_Private );

			CGUI_Slide* pSlide = GUI_CreateObj<CGUI_Slide>(_T("방만들기창_인원수"),_T("Data\\IMG\\GUI\\Button_Red_Slide.ani"),106,180,-1,-1 );
			this->AddChild( pSlide );
			pSlide->SetAllowRegion(106,178);
			pSlide->SetRate(0);

			CGUI_Button* pButton_Slide_Min =  GUI_CreateObj<CGUI_Button> ( _T("방만들기창_인원수_작게"),
				_T("Data\\IMG\\GUI\\Button_Red_Slide_Left.Ani"),89,178,-1,-1 );
			this->AddChild( pButton_Slide_Min );

			CGUI_Button* pButton_Slide_Max=  GUI_CreateObj<CGUI_Button> ( _T("방만들기창_인원수_크게"),
				_T("Data\\IMG\\GUI\\Button_Red_Slide_Right.Ani"),178,178,-1,-1 );
			this->AddChild( pButton_Slide_Max );



			CGUI_Button* pButtonMakeRoomOk =  GUI_CreateObj<CGUI_Button> ( _T("방만들기창확인"),
				_T("Data\\IMG\\GUI\\Button_RedOk.Ani"),62,215,-1,-1 );
			this->AddChild( pButtonMakeRoomOk  );

			CGUI_Button* pButtonMakeRoomCancel = GUI_CreateObj<CGUI_Button>( _T("방만들기창취소"),
				_T("Data\\IMG\\GUI\\Button_RedCancel.Ani"),136,215,-1,-1 );
			this->AddChild( pButtonMakeRoomCancel ); 

			CGUI_MGR::SetGuiFocus( _T("방이름입력창") );

			OBJMSG msg;
			msg.dwMsg = GUIMSG_FOCUSENTER;
			pEditRoomName->ProcMessage( msg );
			CGUI_MGR::MoveCenter( m_Name );
			CGUI_MGR::ReCalcEditList( this );
			_DefaultButton = "방만들기창확인";
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
			// 100 퍼센트로 봤을때 
			// 2,3,4,5,6 이니까는
			//20프로면 2
			//40프로면 3
			//60프로면 4
			//80프로면 5
			//100프로면 6
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
			if( strcmp(pButton->GetName(),"방만들기창확인" ) ==0 )
			{
				// 여기서 다른 오브젝트를 생성해브러 써글 
				CGUI_MGR::_SendMessage( GUIMSG_BUTTON_EVENT,(DWORD)pButton,0 );
				CGUI_MGR::DestroyObj( m_Name );
			}else if( strcmp(pButton->GetName(),"방만들기창취소" ) ==0 )
			{
				CGUI_MGR::_SendMessage( GUIMSG_BUTTON_EVENT,(DWORD)pButton,0 );
				CGUI_MGR::DestroyObjQueue( m_Name );
			}
			else if( strcmp(pButton->GetName(), _T("방만들기창_인원수_작게") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("방만들기창_인원수")	);
				if( pSlide)
				{
					int rr = pSlide->GetRate();
					pSlide->SetRate( rr-20 );
				}
			}
			else if( strcmp(pButton->GetName(),_T("방만들기창_인원수_크게") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("방만들기창_인원수") );
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
// Desc: 갖가지 옵션을 조절할수있음 베이베 오픈시에 속성을 넣어줌 
//       단지 뷰어,콘트롤러의 역할뿐 
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
	CGUI_Slide* pSlide =(CGUI_Slide*) CGUI_MGR::FindGui("옵션창_배경음볼륨");
	if( pSlide )
		pSlide ->SetRate( vol );
}
void CGUI_Option::SetValue_EFFVolume(int vol)
{ 
	if( vol <0 ) vol = 0;
	if( vol >100) vol = 100;
	_CurrentValue._EffVolume = vol; 
	CGUI_Slide* pSlide =(CGUI_Slide*) CGUI_MGR::FindGui("옵션창_효과음볼륨");
	if( pSlide )
		pSlide ->SetRate( vol );
}
void CGUI_Option::SetValue_FullMode(bool bFull)
{
	_CurrentValue._bFullMode= bFull;  
	CGUI_RadioButton* pRadio=NULL;
	if(bFull)
	pRadio = (CGUI_RadioButton*) CGUI_MGR::FindGui("옵션창_풀" );
	else
	pRadio = (CGUI_RadioButton*) CGUI_MGR::FindGui("옵션창_창" );

	if( pRadio )pRadio ->SetCheck( true );
}
void CGUI_Option::SetValue_BlockWhisper(bool bWhisper)
{ 
	_CurrentValue._bWhisper= bWhisper; 
	CGUI_CheckButton* pWhisper =(CGUI_CheckButton*) CGUI_MGR::FindGui("옵션창_귓말거부" );
	if( pWhisper )pWhisper ->SetCheck( bWhisper );
}
void CGUI_Option::SetValue_BlockInvite(bool bInvite )
{
	_CurrentValue._bInvite= bInvite; 
	CGUI_CheckButton* pInvite =(CGUI_CheckButton*) CGUI_MGR::FindGui("옵션창_초대거부" );
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
	
			CGUI_Button* pButtonMakeRoomOk =  GUI_CreateObj<CGUI_Button> ( _T("옵션창_확인"),	_T("Data\\IMG\\GUI\\Button_BlueOk.Ani"),97,247,-1,-1 );
			this->AddChild( pButtonMakeRoomOk  );

			CGUI_Button* pButtonMakeRoomCancel = GUI_CreateObj<CGUI_Button>( _T("옵션창_취소"),_T("Data\\IMG\\GUI\\Button_BlueCancel.Ani"),171,247,-1,-1 );
			this->AddChild( pButtonMakeRoomCancel ); 

			CGUI_RadioButton* pButt_full= GUI_CreateObj<CGUI_RadioButton>(_T("옵션창_풀"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 114,166,-1,-1 );
			pButt_full->SetGroupName( _T("옵션창_화면모드") );
			pButt_full->SetCheck( (g_D3D.IsFull()==true)?true:false );
			this->AddChild( pButt_full );

			CGUI_RadioButton* pButt_window = GUI_CreateObj<CGUI_RadioButton>(_T("옵션창_창"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 217,166,-1,-1 );
			pButt_window->SetGroupName( _T("옵션창_화면모드") );
			pButt_window->SetCheck( (g_D3D.IsFull()==true)?false:true);
			this->AddChild( pButt_window );


			CGUI_CheckButton* pButt_blockinvite = GUI_CreateObj<CGUI_CheckButton>(_T("옵션창_초대거부"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 114,209,-1,-1 );
			pButt_blockinvite ->SetCheck(false );
			this->AddChild( pButt_blockinvite );
			CGUI_CheckButton* pButt_blockwhisper= GUI_CreateObj<CGUI_CheckButton>(_T("옵션창_귓말거부"),_T("Data\\IMG\\GUI\\Button_Check.ani"), 217,209,-1,-1 );
			pButt_blockwhisper->SetCheck(false);
			this->AddChild( pButt_blockwhisper );


			CGUI_Slide* pSlide = GUI_CreateObj<CGUI_Slide>(_T("옵션창_배경음볼륨"),_T("Data\\IMG\\GUI\\Button_Blue_Slide.ani"),170,99,-1,-1 );
			pSlide->SetAllowRegion(170,242);
			this->AddChild( pSlide );
			pSlide->SetRate(0);
			CGUI_Button* pButton_Slide_BGMMin =  GUI_CreateObj<CGUI_Button> ( _T("옵션창_배경음_작게"),
				_T("Data\\IMG\\GUI\\Button_Blue_Slide_Left.Ani"),153,97,-1,-1 );
			this->AddChild( pButton_Slide_BGMMin  );

			CGUI_Button* pButton_Slide_BGMMax=  GUI_CreateObj<CGUI_Button> ( _T("옵션창_배경음_크게"),
				_T("Data\\IMG\\GUI\\Button_Blue_Slide_Right.Ani"),242,97,-1,-1 );
			this->AddChild( pButton_Slide_BGMMax );


			CGUI_Slide* pSlide1 = GUI_CreateObj<CGUI_Slide>(_T("옵션창_효과음볼륨"),_T("Data\\IMG\\GUI\\Button_Blue_Slide.ani"),170,123,-1,-1 );
			pSlide1->SetAllowRegion(170,242);
			this->AddChild( pSlide1 );
			pSlide1->SetRate(0);
			CGUI_Button* pButton_Slide_EffMin =  GUI_CreateObj<CGUI_Button> ( _T("옵션창_효과음_작게"),
				_T("Data\\IMG\\GUI\\Button_Blue_Slide_Left.Ani"),153,121,-1,-1 );
			this->AddChild( pButton_Slide_EffMin );

			CGUI_Button* pButton_Slide_EffMax=  GUI_CreateObj<CGUI_Button> ( _T("옵션창_효과음_크게"),
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
			if( strcmp(pButton->GetName(),"옵션창_확인" ) ==0 )
			{
				// 여기서 다른 오브젝트를 생성해브러 써글 
				
				RestoreValue();// 현재값 저장하며 
				CGUI_MGR::DestroyObjQueue( m_Name );
			}else if( strcmp(pButton->GetName(),"옵션창_취소" ) ==0 )
			{
				_CurrentValue = _RestoreValue ;
				CGUI_MGR::DestroyObjQueue( m_Name );
			}
			else if( strcmp(pButton->GetName(), _T("옵션창_배경음_작게") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("옵션창_배경음볼륨")	);
				if( pSlide)
				{
					int rr = pSlide->GetRate();
				//	pSlide->SetRate( rr-10 );
					this->SetValue_BGMVolume( rr -10 );
				}
			}
			else if( strcmp(pButton->GetName(),_T("옵션창_배경음_크게") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("옵션창_배경음볼륨") );
				if( pSlide)
				{
					int rr = pSlide->GetRate();
					//pSlide->SetRate( rr+10 );
					this->SetValue_BGMVolume( rr +10 );
				}
			}
			else if( strcmp(pButton->GetName(), _T("옵션창_효과음_작게") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("옵션창_효과음볼륨")	);
				if( pSlide)
				{
					int rr = pSlide->GetRate();
					//pSlide->SetRate( rr-10 );
					this->SetValue_EFFVolume( rr -10);
				}
			}
			else if( strcmp(pButton->GetName(),_T("옵션창_효과음_크게") ) ==0 )
			{
				CGUI_Slide* pSlide = (CGUI_Slide*)CGUI_MGR::FindGui( _T("옵션창_효과음볼륨") );
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


#include <guistdafx.h>


IGUI_Object* CGUI_MGR::m_pLastFocus;
IGUI_Object* CGUI_MGR::m_pFocus;

IGUI_Object::GUI_PTRLIST CGUI_MGR::m_GuiList;
list<TCHAR*> CGUI_MGR::m_EditList;
std::queue<std::string> CGUI_MGR::_DestroyObjNameQ;
IGUIEvent* CGUI_MGR::	m_pEventHandler=NULL;
//--------------------------------------------------
// 위에 열거된 오브젝트를 일괄 처리해줄 메니져 
// 어떤 씬이든 Process,Render 만 포함 시켜주면 된다. 
//--------------------------------------------------
CGUI_MGR::CGUI_MGR()
{
	m_pFocus = NULL;
	m_pLastFocus = NULL;
}

CGUI_MGR::~CGUI_MGR()
{
 	DestroyAllObj();
}


void CGUI_MGR::AddGUI(IGUI_Object *pGui)
{
	assert( pGui != NULL );
	m_GuiList.push_back( pGui );
	GUIMSG msg;
	msg.dwMsg = GUIMSG_CREATED; // 생성됨을 통보한다. 
	msg.dwParam2 = msg.dwParam1 = 0;
	pGui->ProcMessage( msg );
}

CGUI_FocusText* CGUI_MGR::CreateFocusText(const TCHAR* szParentName, const TCHAR* MyName, int x,int y,DWORD OutClr,DWORD FontClr, const TCHAR* Str )
{
	if( _tcslen( Str ) == 0 )
		return NULL ;

	CGUI_FocusText* pTmp = new CGUI_FocusText( x, y, FontClr, OutClr, Str );

	pTmp->SetName( MyName );
	IGUI_Object* pParent = FindGui( szParentName );	

	if( pParent == NULL )
		AddGUI( pTmp );
	else
	pParent->AddChild( pTmp );
	
	return 	pTmp;
}

CGUI_Text* CGUI_MGR::CreateText(const TCHAR* szParentName, const TCHAR* MyName, int x,int y,D3DCOLOR OutClr,D3DCOLOR FontClr, const TCHAR* Str )
{
	if( _tcslen( Str ) == 0 )
		return NULL ;

	CGUI_Text* pTmp = new CGUI_Text( x, y, OutClr, FontClr,  Str );

	pTmp->SetName( MyName );
	IGUI_Object* pParent = FindGui( szParentName );	

	if( pParent == NULL )
		AddGUI( pTmp );
	else
	pParent->AddChild( pTmp );
	
	return 	pTmp;
}


//IGUI_Object* CGUI_MGR::CreateObject( GUI_CLSID  Type, const TCHAR* ClsName,const TCHAR* fName, int x,int y, int Width, int Height,void* extParam )
//{
//	IGUI_Object* pTmp= NULL;
//
//	switch( Type )
//	{
//
//	case GUI_PANNEL:
//		{
//			pTmp = new CGUI_Pannel; 
//			bool bl = false;
//			if( extParam != NULL)
//			bl = *((bool*)extParam);
//
//			if( bl == true )
//			{
///*				if( m_ModalFocus != NULL)
//				{
//					m_ModalFocus->SetModal( false ); // 이미 모달인게 있다면 모달리스로 만들고 
//				}*/
////				pTmp->SetLastModal( m_ModalFocus );
//				pTmp->SetModal( true ); 
////				m_ModalFocus  = ( IGUI_Object* ) pTmp;
//
//
//			}else
//			{
//				pTmp->SetModal( false );
//			}
//
//		}
//		break;
//
//	case GUI_ANI:
//		{
//			pTmp = new CGUI_Ani;
//		}
//		break;
//	case GUI_BUTTON:
//		{
//			// 인자로 필요한것: 버튼에 올라갈 스트링
//			pTmp = new CGUI_Button;				
//		}
//		break;
//
//	case GUI_EDITBOX:
//		{
//			CGUI_Edit::EDIT_ATTRIB attr =  *(CGUI_Edit::EDIT_ATTRIB*)extParam;
//			pTmp = new CGUI_Edit( attr );
//		}
//		break;
//
//	case GUI_LISTBOX:
//		{	
//			pTmp = new CGUI_ListBox;	
//		}
//		break;
//	case GUI_CLSID_AVATAR:
//		{
//			pTmp = new CGUI_Avatar;
//		}
//		break;
//	default:
//		{
//			assert( 0 && "Don't make GUI Obj" );
//		}
//		break;
//	}
//
//	GUIMSG msg;
//	msg.dwMsg = GUIMSG_CREATED; // 생성됨을 통보한다. 
//	msg.dwParam2 = msg.dwParam1 = 0;
//	pTmp->ProcMessage( msg );
//
//	pTmp->SetName( ClsName );
//	pTmp->m_Img.TexID =-1;
//
//	if( fName != NULL )
//	g_D3D.LoadAni( fName, pTmp->m_Img );
//
//	if( fName != NULL && pTmp->m_Img.TexID == -1 )
//	{
//		TCHAR buff[64]={0,};
//		_stprintf( buff, "don't find the  %s ", fName );
//		MessageBox( 0,buff,"err",MB_OK);
//		SAFE_DELETE( pTmp);
//		return NULL;
//	}
//
//if( fName != NULL )
//{
//	if( Width <= 0 ) 
//		Width = pTmp->m_Img.sprInfo.nWidth;
//	//else
//	//	pTmp->m_Img.sprInfo.nWidth = Width;
//	
//	if( Height <= 0 ) 
//		Height = pTmp->m_Img.sprInfo.nHeight;
//}
////else
////	pTmp->m_Img.sprInfo.nHeight = Height;
//
////  Add 촤일드 하면 부모의 좌표에 맞게 설정 
//	pTmp->SetSrcRect( 0,0, Width,Height);
//	pTmp->SetDestRect( (float)x, (float)y,
//						(float)(x+Width),
//						(float)(y+Height) );
//
//	pTmp->SetBoundRect( (float)x, (float)y,
//						(float)(x+Width),
//						(float)(y+Height) );
//	return pTmp;
//}


//IGUI_Object* CGUI_MGR::CreateObject( GUI_CLSID  Type, const TCHAR* ClsName,const TCHAR* fName, int x,int y, int Width, int Height,void* extParam )
//{
//	IGUI_Object* pTmp= NULL;
//
//	switch( Type )
//	{
//
//	case GUI_PANNEL:
//		{
//			pTmp = new CGUI_Pannel; 
//			bool bl = false;
//			if( extParam != NULL)
//			bl = *((bool*)extParam);
//
//			if( bl == true )
//			{
///*				if( m_ModalFocus != NULL)
//				{
//					m_ModalFocus->SetModal( false ); // 이미 모달인게 있다면 모달리스로 만들고 
//				}*/
////				pTmp->SetLastModal( m_ModalFocus );
//				pTmp->SetModal( true ); 
////				m_ModalFocus  = ( IGUI_Object* ) pTmp;
//
//
//			}else
//			{
//				pTmp->SetModal( false );
//			}
//
//		}
//		break;
//
//	case GUI_ANI:
//		{
//			pTmp = new CGUI_Ani;
//		}
//		break;
//	case GUI_BUTTON:
//		{
//			// 인자로 필요한것: 버튼에 올라갈 스트링
//			pTmp = new CGUI_Button;				
//		}
//		break;
//
//	case GUI_EDITBOX:
//		{
//			CGUI_Edit::EDIT_ATTRIB attr =  *(CGUI_Edit::EDIT_ATTRIB*)extParam;
//			pTmp = new CGUI_Edit( attr );
//		}
//		break;
//
//	case GUI_LISTBOX:
//		{	
//			pTmp = new CGUI_ListBox;	
//		}
//		break;
//	case GUI_CLSID_AVATAR:
//		{
//			pTmp = new CGUI_Avatar;
//		}
//		break;
//	default:
//		{
//			assert( 0 && "Don't make GUI Obj" );
//		}
//		break;
//	}
//
//	GUIMSG msg;
//	msg.dwMsg = GUIMSG_CREATED; // 생성됨을 통보한다. 
//	msg.dwParam2 = msg.dwParam1 = 0;
//	pTmp->ProcMessage( msg );
//
//	pTmp->SetName( ClsName );
//	pTmp->m_Img.TexID =-1;
//
//	if( fName != NULL )
//	g_D3D.LoadAni( fName, pTmp->m_Img );
//
//	if( fName != NULL && pTmp->m_Img.TexID == -1 )
//	{
//		TCHAR buff[64]={0,};
//		_stprintf( buff, "don't find the  %s ", fName );
//		MessageBox( 0,buff,"err",MB_OK);
//		SAFE_DELETE( pTmp);
//		return NULL;
//	}
//
//if( fName != NULL )
//{
//	if( Width <= 0 ) 
//		Width = pTmp->m_Img.sprInfo.nWidth;
//	//else
//	//	pTmp->m_Img.sprInfo.nWidth = Width;
//	
//	if( Height <= 0 ) 
//		Height = pTmp->m_Img.sprInfo.nHeight;
//}
////else
////	pTmp->m_Img.sprInfo.nHeight = Height;
//
////  Add 촤일드 하면 부모의 좌표에 맞게 설정 
//	pTmp->SetSrcRect( 0,0, Width,Height);
//	pTmp->SetDestRect( (float)x, (float)y,
//						(float)(x+Width),
//						(float)(y+Height) );
//
//	pTmp->SetBoundRect( (float)x, (float)y,
//						(float)(x+Width),
//						(float)(y+Height) );
//	return pTmp;
//}

void   CGUI_MGR::NoneFocusSetting(void)
{
	HWND hWnd = g_D3D.GethWnd();
	::SetFocus( hWnd ); 
	SetGuiFocus( NULL ); 
} 

IGUI_Object* CGUI_MGR::GetCurrFocus(void) 
{
	return m_pFocus; 
};
/*
IGUI_Object* CGUI_MGR::GetModalFocus(void) 
{ 
	return m_ModalFocus; 
}*/

// 모달 포커스 유 
// 포커스 유 
// 포커스 널

void CGUI_MGR::ReCalcEditList(IGUI_Object* pObj) // 현재 유용한 에디트 박스의 리스트를 재계산함 
{
// 인자로 넘어오는것에 대하여 검색해달라 하면 그것과 그것의 자식들만 에디트 박스인지 검색하여 추가 
// 인자로 넘어오는것이 없으면 현재 생성된 컨트롤은 모두 검색하고 
// 그것과 그것의 자식들이 에디트 박스인지 검색하여 추가 

	m_EditList.clear(); 
	if( pObj != NULL )
	{
		// 현재의 지정된 오브젝트와 그의 자식들만 들어감 
		if( pObj->GetClsID() == GUI_EDITBOX ) // 자기 자신부터 검사하고 
		{
			m_EditList.push_back( pObj->GetName() );
		}

		// 자식들을 검색해보고서 집어 넣는다 
		IGUI_Object::GUI_PTRLIST L = pObj->GetChildList();
		IGUI_Object::GUI_PTRLIST::iterator it = L.begin();
		while( it != L.end() )
		{
			IGUI_Object* pTmp = (*it);
			if( pTmp->GetClsID() == GUI_EDITBOX )
			{
				m_EditList.push_back( pTmp->GetName() );
			}
			
			it++;
		}
		
	}else
	{
		// 모든 부모와 그 의 자식 들을 검색함. 2차적 검색 까지 들어감
		IGUI_Object::GUI_PTRLIST::iterator it = m_GuiList.begin();
		while( it != m_GuiList.end() )
		{
			IGUI_Object* pTmp = (*it);
			if( pTmp->GetClsID() == GUI_EDITBOX )
			{
				m_EditList.push_back( pTmp->GetName() );
			}
			
			// 자식들을 검색해보고서 집어 넣는다 
			IGUI_Object::GUI_PTRLIST L = pTmp->GetChildList();
			IGUI_Object::GUI_PTRLIST::iterator cit = L.begin();
			while( cit != L.end() )
			{
				IGUI_Object* pTmpChild = (*cit);
				if( pTmpChild->GetClsID() == GUI_EDITBOX )
				{
					m_EditList.push_back( pTmpChild->GetName() );
				}
				
				cit ++;
			}
			
			
			it++;
		}
	}



/*	
	if( m_pFocus != NULL )
	{
		IGUI_Object* pFocus =  m_pFocus;

			IGUI_Object* pParent = pFocus->GetParent();
			if( pParent == NULL ) 
			{
				//pParent = pFocus ;
				m_EditList.push_back( pFocus->GetName() );
				return;
			}
			else
			{
				while( pParent != NULL ) 
				{
					if( pParent->GetParent() == NULL )
						break;
					pParent= pParent->GetParent();
				}
			}
			
			IGUI_Object::GUI_PTRLIST L = pParent->GetChildList();
			IGUI_Object::GUI_PTRLIST::iterator it = L.begin();
			while( it != L.end() )
			{
				IGUI_Object* pTmp = (*it);
				if( pTmp->GetClsID() == GUI_EDITBOX )
				{
					m_EditList.push_back( pTmp->GetName() );
				}
				
				it++;
			}

	}else 
	{
		IGUI_Object::GUI_PTRLIST::iterator it =	m_GuiList.begin();
		while( it != m_GuiList.end() )
		{
			IGUI_Object* pTmp = (*it);
			if( pTmp->GetClsID() == GUI_EDITBOX )
			{
				m_EditList.push_back( pTmp->GetName() );
				// 요기서 자식들도 에디트가 있으면 추가해버림 
			}

			IGUI_Object::GUI_PTRLIST L = pTmp->GetChildList();
			IGUI_Object::GUI_PTRLIST::iterator cit = L.begin();
			while( cit != L.end() )
			{
				IGUI_Object* pTmp = (*cit);
				if( pTmp->GetClsID() == GUI_EDITBOX )
				{
					m_EditList.push_back( pTmp->GetName() );
				}
				cit++;
			}

			it++;
		}	
	}
*/
}

void CGUI_MGR::NextEditBox(void)
{
	// 에디트 박스의 동적인 리스트를 구현하여 
	// 에디트 박스의 리스트를 순회하여 포커스를 줍니다.

	if( m_EditList.empty() == true ) return;

	static GUI_EDITLIST::iterator it = m_EditList.begin();
	// 선택된 것이 없다면 첨으로 되돌리기 
	// 현재 선택된 것을 찾고서 
	// 그것의 다음것으로 it 를 해준다 

	TCHAR* Name = (*it);
	
	if( m_pFocus !=NULL )
	{
		Name = m_pFocus->GetName();
		if( m_pFocus->GetClsID() == GUI_EDITBOX )
		{
			it = find( m_EditList.begin(), m_EditList.end(), Name );
		}
	}

	// 현재 논 포커스에 차일드 에디트 박스 두개  에디트 박스라면
	if( m_pFocus == NULL )
	{
		it = m_EditList.begin();
		Name = (*it );
	}
	else
	{
		it = find( it, m_EditList.end(), Name );
		it++;
		if( it == m_EditList.end() )
		{
			it = m_EditList.begin();
			Name = (*it);
		}else
		{
			Name = (*it);
		}
	}

	SetGuiFocus( Name );
	// 아니라면 
}


/*
1. 자신의 최고 부모를 찾아낸다. 만일 부모가 없다면 자신이 최고 부모임.
2. 부모 리스트 중에서 원래자리에선 빼고 맨뒤에 추가한다. 
*/
void CGUI_MGR::SetGuiFocus( const TCHAR* Name )
{ 
	// 현재 포커스 가진것이 모달일때
	// 모달이 아닐때 
	IGUI_Object* pGui = FindGui( Name );
	
	IGUI_Object* pLast = NULL;
	if( m_pFocus != NULL )
	{
		// 여기서 자신의 자식중하나가 포커스 가졌다면
		// 이 메세지는 보내지 않는다 
			IGUI_Object* pFocusParent = NULL;
			if( m_pFocus )
			pFocusParent = m_pFocus->GetSuperParent();
			//while( 1 )
			//{
			//	if( pFocusParent->GetParent() == NULL )	break;
			//	pFocusParent= pFocusParent->GetParent();
			//}

			IGUI_Object* pNewParent = NULL;
			if( pGui )
			pNewParent = pGui->GetSuperParent();
			//if( pGui != NULL )
			//while( 1 )
			//{
			//	if( pNewParent->GetParent() == NULL )	break;
			//	pNewParent= pNewParent->GetParent();
			//}
			
		// 최상위 부모를 찾았고 그 부모는 나이거나 또는 널일수 있다
		// 우리 부모가 아니고 널이 아니면 그 이 메세지를 전송해줌.

		if( ( pFocusParent!= NULL )&& pFocusParent != pNewParent )
		{
			pLast = m_pFocus;
			OBJMSG msg;
			msg.dwMsg = GUIMSG_FOCUSLEAVE;
			pLast->ProcMessage( msg );
			m_pLastFocus = pLast;
		}
	}

	m_pFocus = pGui;

	if( pGui == NULL )
	{
		if( m_pFocus != NULL ) // 이전에 선택된것이 있다면.. 그것의 부모를 찾는다.
		{
			IGUI_Object* pParent = m_pFocus;
			while( 1 )
			{
				if( pParent->GetParent() == NULL )	break;
				pParent = pParent->GetParent();
			}
			pParent ->SetLayer( D3D_INTERFACE_LAYER0 );
		}
	}
	else
	{
		OBJMSG msg;
		msg.dwMsg = GUIMSG_FOCUSENTER;
		pGui->ProcMessage( msg );
		IGUI_Object* pParent = pGui->GetSuperParent();
		//while( pParent->GetParent() != NULL )	{	pParent = pParent->GetParent();		}
		// 최상위 부모를 찾은셈 부모를 맨뒤( 화면에서 최상위 ) 로 보낸다.
		//if( pParent->GetClsID() == GUI_PANNEL ) 
		if( pParent != NULL ) 
		{
			IGUI_Object::GUI_LISTITOR it = find( m_GuiList.begin(), m_GuiList.end(), pParent );
			m_GuiList.erase( it );
			m_GuiList.push_back( pParent ); // 맨뒤로.. 
		}
	}

}

void CGUI_MGR::ProcessAll()
{
	std::vector<IGUI_Object*>::iterator it = m_GuiList.begin();

	while( it != m_GuiList.end() )
	{
		if( (*it)->IsShow() )
		{
			(*it)->Process();
			(*it)->Process_Child();
		}
		it++;
	}
	while( _DestroyObjNameQ.empty() == false )
	{
		std::string& name = _DestroyObjNameQ.front();
		DestroyObj( name.c_str() ); //(const TCHAR*)
		_DestroyObjNameQ.pop();
	}
}

void CGUI_MGR::RenderAll()
{
	size_t Cnt = m_GuiList.size();
	for( size_t i=0; i< Cnt; i++)
	{
		IGUI_Object* pTmp =(IGUI_Object*) m_GuiList[i];
		if( pTmp->IsShow() )
		{
			pTmp->Render();
			pTmp->Render_Child();
		}
	}

	//std::vector<IGUI_Object*>::iterator it = m_GuiList.begin();
	//while( it != m_GuiList.end() )
	//{
	//	if( (*it)->IsShow() )
	//	{
	//		(*it)->Render();
	//	}
	//	it++;
	//}
}

void CGUI_MGR::ProcMessage(GUIMSG& Msg)
{
	// 마우스가 움직일때 마다 포커스가 바뀌어야 한다.
	// 마우스가 현재 있는곳의 GUI 에게만 메세지가 가야한다.
	// 이것을 지키려면 현재의 마우스가 있는 위치의 포커스와
	// 마우스가 찍어논 GUI 의 포커스 두가지가 존재해야 한다 
	//m_pEventHandler->GUIEvent_Hander( *pMsg );


	IGUI_Object* pMouseFocus = NULL;
	if( Msg.dwMsg == GUIMSG_MMOVE )
	{
		if( (m_pFocus!=NULL) && m_pFocus->IsModal() == true )
		{
			pMouseFocus = m_pFocus;
		}
		else
		{
			IGUI_Object::GUI_PTRLIST childList;
			int temp = (int)m_GuiList.size() - 1;
			if( temp > 0 )
			{
				for( int guiIndex=temp; guiIndex >= 0  ; guiIndex-- )
				{
					IGUI_Object * pTmp = m_GuiList[guiIndex];
					if( pTmp ->ProcMessage( Msg )== true )
					{
						pMouseFocus = pTmp;
						break;
					}
				}
			}
		}		
	}
	// 현재 포커스가 있는것은 ...색깔을 뿔거게 해브러 ㅋㅋ 
	// 모달이 있다면 아무곳에도 메세지를 전송할 필요가 없습니다.
	// 모달을 처리 해야 하는데...
		if( (m_pFocus!=NULL) && m_pFocus->IsModal() == true )
		{
			pMouseFocus = m_pFocus;
		}


	// 현재 포커스가 있는곳에 메세지를 보낸다.
	// 포커스가 없다면 첨부터 모든 이들에게 메세지를 전송 
	// 포커스가 있는것이 항상 앞으로 정렬됨.
	// 지금은 창에만 적용되고 나중엔 버튼 텍스트 에니메 등 모두 적용될것임.
	
	if( pMouseFocus != NULL ) // 포커스가 있다면 그것만 적용됨 
	{
		IGUI_Object* pFocus = pMouseFocus  ;

			IGUI_Object* pParent = pFocus->GetParent();
			if( pParent == NULL ) 
				pParent = pFocus ;
			else
			{
				while( pParent != NULL ) 
				{
					if( pParent->GetParent() == NULL )
						break;
					pParent= pParent->GetParent();
				}
			}
		if( pParent != NULL )
		{
			if( pParent->ProcMessage_Child( Msg ) == true )
				return;
			if( pParent->ProcMessage( Msg ) == true ) 
				return;

			if( pParent->IsModal() == true ) return;	
		}
		
	}	
	else // 포커스 가진게 없으면 모두 돌아가며 메세지 전송 
	{
		IGUI_Object::GUI_PTRLIST childList;
		int tmp = (int)m_GuiList.size() - 1;
		if( tmp >0)
		{
			for( int guiIndex=tmp; guiIndex >= 0  ; guiIndex-- )
			{
				IGUI_Object * pTmp = m_GuiList[guiIndex];
				if( pTmp->ProcMessage_Child( Msg ) == true )
				return;//break;
				if( pTmp ->ProcMessage( Msg ) == true ) return;
				//			if( m_GuiList[guiIndex]->ProcMessage( pMsg ) == true ) return;
			}
		}
	}
	if( Msg.dwMsg == GUIMSG_MLDOWN )
	CGUI_MGR::NoneFocusSetting();	
	CGUI_MGR::_SendMessage(  Msg.dwMsg ,Msg.dwParam1,Msg.dwParam2);
}

IGUI_Object* CGUI_MGR::FindGui(const TCHAR* Name )
{ 
	if( Name==NULL ) return NULL;
	// 부모들을 모두 돌면서 그 부모의 자식또한 모두 돌아버린다.
	IGUI_Object::GUI_LISTITOR it = m_GuiList.begin();
	int size = (int) m_GuiList.size();
	IGUI_Object *pParent=NULL,*pChild = NULL ;

	IGUI_Object::GUI_LISTITOR endit = m_GuiList.end();
	while( it != endit )
	{
		pParent = (IGUI_Object*)*it;
		if( _tcscmp( Name, pParent->GetName() ) == 0 ) return pParent;

		pParent = (IGUI_Object*)*it;
		pChild = NULL;
		pChild = pParent->FindGui( Name );

		if( pChild != NULL ) 
			return pChild;

		it++;
	}
	
	return NULL;
}

void CGUI_MGR::DestroyObjQueue( const TCHAR* Name )  // 이름을 큐에 저장만 해놓는다.
{
	_DestroyObjNameQ.push( Name );
}

// 자기, 자기자신의 최고부모가 사라지면
// 널포커스로 바꿔줘야함 
void CGUI_MGR::DestroyObj( const TCHAR* Name )// 실제로 없애준다.
{
	// 자식을 먼저 검사한뒤 그 것을 없앤다. 
	// 없앴다면 리턴 해버리고 
	// 아니라면 그 부모를 없애는 것이므로 

	IGUI_Object* pDestGui = FindGui( Name );
	if( pDestGui == NULL ) return;

//	if( GetCurrFocus() == pDestGui )
	if( m_pFocus!=NULL && pDestGui != NULL )
	if( m_pFocus->GetSuperParent() == pDestGui->GetSuperParent() )
	NoneFocusSetting();
//	m_pFocus = m_pLastFocus;

	GUIMSG msg;
	msg.dwMsg = GUIMSG_DESTROY;
	msg.dwParam2 = msg.dwParam1;
	pDestGui->ProcMessage(msg);
	pDestGui->DestroyAllChild();
	// 자식들에게 먼저 보내준다.
	// 물론 자신의 자식들은 모두역시 재귀적으로 자신의 자식들에게 전송할것임
	// 결과적으로 최하위 자식들부터 차례대로 삭제된후
	// 최후에 최상위 부모가 삭제될것임 

	IGUI_Object::GUI_LISTITOR it = NULL;
	it = find( m_GuiList.begin(), m_GuiList.end(), pDestGui ); // 자식일때는 어찌해야 하지
	//IGUI_Object* pt =(IGUI_Object*) *it ;
	if( m_GuiList.end() != it || it != NULL )
	m_GuiList.erase( it );

	GUICLSID ClsID = pDestGui->GetClsID();
	SAFE_DELETE( pDestGui );
	if( ClsID == GUI_PANNEL )
	{
		ReCalcEditList();
	}
}

void CGUI_MGR::DestroyAllObj( DWORD mode)
{
	NoneFocusSetting();

	int size = (int) m_GuiList.size();
	IGUI_Object* pTmp = NULL;
	IGUI_Object::GUI_LISTITOR it = m_GuiList.begin();
	while( it != m_GuiList.end()  )
	{	
		pTmp = *it;
		if( (pTmp->GetLifeMode()&mode) == pTmp->GetLifeMode() ) 
		{
#ifdef _DEBUG
//			ConsoleOut( "%s Destryoed\n", pTmp->GetName() );
//		char ccc[128]={0,};
//		sprintf( ccc,"%s\n",pTmp->GetName() );
//		OutputDebugString(ccc);
#endif
			GUIMSG msg;
			msg.dwMsg = GUIMSG_DESTROY;
			msg.dwParam2 = msg.dwParam1;
			pTmp ->ProcMessage(msg);
			SAFE_DELETE( pTmp );
			m_GuiList.erase(it);
		}else
		it++;
	}
	

	while( _DestroyObjNameQ.empty() == false )
	{
		std::string& name = _DestroyObjNameQ.front();
		DestroyObj( name.c_str() ); //(const TCHAR*)
		_DestroyObjNameQ.pop();
	}

}


void	CGUI_MGR::MoveCenter(const TCHAR* Name)
{
	IGUI_Object* pObj =(IGUI_Object*) FindGui( Name );
	if( pObj == NULL ) return;
	// 화면 800, 600 의 중간에 위치를 셋팅해준다 
	int x = 400 - (pObj->m_Img.sprInfo.nWidth>>1);
	int y= 300 - (pObj->m_Img.sprInfo.nHeight>>1);

	pObj->MoveGUI( -pObj->GetX(), -pObj->GetY() );
	pObj->MoveGUI( (float)x, (float)y );
}

void CGUI_MGR::ModalBox(TCHAR* szStr, ICommand* pCommand )
{
	//CGUI_Pannel* pPannel =(CGUI_Pannel*) FindGui( _T("콘솔창") );

	DestroyObj( _T("콘솔창"));
	CGUI_Console* pConsole = GUI_CreateObj<CGUI_Console>( _T("콘솔창"),_T("data\\IMG\\Gui\\notice_back.ani"),0,0,-1,-1 );
	AddGUI( pConsole );
	pConsole->SetCommand( pCommand );
	pConsole->AddString( szStr );
//	if( pPannel == NULL )
//	{
//		pPannel = GUI_CreateObj<CGUI_Pannel>( _T("콘솔창"),_T("data\\IMG\\Gui\\notice_back.ani"),0,0,-1,-1 );
//		AddGUI( pPannel);
//
//		// 텍스트를 만들어서 올린다. 
//		CGUI_ListBox* pList = GUI_CreateObj<CGUI_ListBox>(  "콘솔창_글리스트", NULL ,67,38, 230,124 ); 
////		pList->SetLayer( D3D_INTERFACE_LAYER1 );
//		pPannel->AddChild( pList );
//		pList->AddString( szStr, 0x9f0080ff, 0xffffffff  );
//		CGUI_Button* pBut = GUI_CreateObj<CGUI_Button>( _T("콘솔창_닫기"),	_T("data\\IMG\\gui\\Button_BlueOk.ani") ,134,182,-1,-1 );
////		pBut->SetLayer( D3D_INTERFACE_LAYER1 );
////		pBut->SetCommand( new CCommand_CloseWindow( pCommand ) );
//		pPannel->AddChild( pBut );
//	}
//	else
//	{ 
//		CGUI_Button* pBut =(CGUI_Button*) FindGui( _T("콘솔창_닫기") );
////		if( pBut != NULL )
////		pBut ->SetCommand( new CCommand_CloseWindow( pCommand ) );
//
//		CGUI_ListBox* pList =(CGUI_ListBox*) FindGui( _T("콘솔창_글리스트") );
//		pList->RemoveAllContents();
//		if( pList  != NULL )
//		{
//			pList->AddString( szStr , 0x9f0080ff, 0xffffffff );
//		}
//	} 
//	SetGuiFocus(_T("콘솔창")); 
//	MoveCenter( _T("콘솔창") );
}



void CGUI_MGR::_SendMessage(eOM dwMsg, DWORD Param1, DWORD Param2 ) 
{
	GUIMSG msg;
	msg.dwMsg = dwMsg;
	msg.dwParam1 = Param1;
	msg.dwParam2 = Param2;
	m_pEventHandler->GUIEvent_Hander( msg );
}

void CGUI_MGR::_SendMessage(const TCHAR*szGUIName,eOM dwMsg, DWORD Param1, DWORD Param2 )
{
	GUIMSG msg;
	msg.dwMsg = dwMsg;
	msg.dwParam1 = Param1;
	msg.dwParam2 = Param2;
	IGUI_Object* pGui = CGUI_MGR::FindGui(szGUIName);
	if( pGui != NULL )
		pGui->ProcMessage( msg );
}


// 현재 INI 에 설정된것들을 추가,생성함.
// 임시적으로 버튼만 테스트해보고
// GUI_Ani,EditBox,ListBox, Pannel,GUI_TEXT 등도 추가예정
// CreateObject 를 포장함 
// 버튼 갯수는 무조건 256개로 설정.
/*
[BUTTON0]
이름 = "새로고침"
IMGPATH = "Data\\IMG\\Lobby\\Button_Refresh.ani"
pos_x = 11
pos_y = 30
부모 = 없음
발생이벤트 = GUIMSG_WINDOW_CLOSE
*/
bool CGUI_MGR::_CreateFromINI( const TCHAR* szName) 
{
	TCHAR Tmp[256]={0,},szGUIName[256]={0,}, szAniPath[256]={0,};
	TCHAR szParentName[128]={0,};
	int x=0,y=0,Width=-1,Height=-1;
	int bEnable = 1;
	int ButtonCnt = GetPrivateProfileInt( "GUI_COUNT","버튼갯수",0,szName);
	BYTE Layer = D3D_MAX_FAR_LAYER2;
	for( int i=0; i< ButtonCnt; i++)
	{
		_stprintf( Tmp, "버튼%d",i);
		GetPrivateProfileString( Tmp, "이름", "무명이", szGUIName, 256, szName);
		GetPrivateProfileString( Tmp, "IMGPATH", "Untitled.ani", szAniPath, 256, szName);
		GetPrivateProfileString( Tmp, "부모이름", "", szParentName, 128, szName);
		
		x = GetPrivateProfileInt( Tmp,"pos_x",0,szName);
		y = GetPrivateProfileInt( Tmp,"pos_y",0,szName);
		Width = GetPrivateProfileInt( Tmp,"Width",-1,szName);
		Height= GetPrivateProfileInt( Tmp,"Height",-1,szName);
		bEnable = GetPrivateProfileInt( Tmp,"ENABLE",1,szName);
		Layer = (BYTE)GetPrivateProfileInt( Tmp,"Layer",D3D_MAX_FAR_LAYER2,szName);
		if( x <= -1 || y<= -1 )
		{
			char str[256]={0,};
			wsprintf( str, "File::%s , [%s]",Tmp);
			MessageBox(0,str, "잘못된인자",MB_OK);
		}
		CGUI_Button* pObj = GUI_CreateObj<CGUI_Button> ( szGUIName,szAniPath,x,y,Width,Height );
		pObj->SetLayer( Layer );
		IGUI_Object* pParent = FindGui( szParentName );
		if( pParent == NULL )
		{
			AddGUI( pObj );
		}
		else
		{
			pParent->AddChild( pObj );
		}
		if( bEnable == 0 )
			CGUI_MGR::_SendMessage(szGUIName,GUIMSG_DISABLE,0,0);
	}
	return true;
}

// 현재 추가,생성되어 잇는것들중 INI 에 있는것들만 없애버림
void CGUI_MGR::_DestroyFromINI(const TCHAR* szName)
{
	int Cnt=0;
	TCHAR Tmp[256]={0,},szGUIName[256]={0,} ;
	int ButtonCnt = GetPrivateProfileInt( "GUI_COUNT","버튼갯수",0,szName);
	for( Cnt=0; Cnt<ButtonCnt ; Cnt++)
	{
		_stprintf( Tmp, "버튼%d",Cnt);
		GetPrivateProfileString( Tmp, "이름", "무명이", szGUIName, 256, szName);
		DestroyObj( szGUIName );
	}

}

bool CGUI_MGR::CreateFromINI( const TCHAR* szName) 
{
	CINI _Ini(szName);
	string szClass;
	string szAniPath;
	string szParentName;
	
	int x=0,y=0,Width=-1,Height=-1;
	int bEnable = 1;

	size_t ButtonCnt = _Ini.GetSectionCnt();
	int Layer = D3D_MAX_FAR_LAYER2;
	string section;
	for( size_t i=0; i< ButtonCnt; i++)
	{
		const char* Section_ButtonName = _Ini.GetSection(i);

		_Ini.GetValue_String( Section_ButtonName, "class", "Invalid", szClass );

		if( szClass == "Invalid" ) continue; // 잇힝

		_Ini.GetValue_String( Section_ButtonName, "IMGPATH", "Untitled.ani", szAniPath );
		_Ini.GetValue_String( Section_ButtonName, "부모이름", "", szParentName );
		_Ini.GetValue_Int( Section_ButtonName, "pos_x",0,x );
		_Ini.GetValue_Int( Section_ButtonName, "pos_y",0,y );
		_Ini.GetValue_Int( Section_ButtonName, "Width",-1,Width  );
		_Ini.GetValue_Int( Section_ButtonName, "Height",-1,Height );
		_Ini.GetValue_Int( Section_ButtonName, "ENABLE",1,bEnable );
		_Ini.GetValue_Int( Section_ButtonName, "Layer",D3D_MAX_FAR_LAYER2, Layer );

		if( x <= -1 || y<= -1 )
		{
			char str[256]={0,};
			wsprintf( str, "File::%s , [%s]",Section_ButtonName);
			MessageBox(0,str, "잘못된인자",MB_OK);
		}
		CGUI_Button* pObj = GUI_CreateObj<CGUI_Button> ( Section_ButtonName,szAniPath.c_str(),x,y,Width,Height );
		pObj->SetLayer( Layer );
		IGUI_Object* pParent = FindGui( szParentName.c_str() );
		if( pParent == NULL )
		{
			AddGUI( pObj );
		}
		else
		{
			pParent->AddChild( pObj );
		}
		if( bEnable == 0 )
			CGUI_MGR::_SendMessage( (char*)Section_ButtonName, GUIMSG_DISABLE,0,0);
	}
	return true;
}


void CGUI_MGR::DestroyFromINI(const TCHAR* szName)
{
	CINI _Ini(szName);

	size_t ButtonCnt = _Ini.GetSectionCnt();
	for( size_t Cnt=0; Cnt<ButtonCnt ; Cnt++)
	{
		DestroyObj( _Ini.GetSection(Cnt) );
	}

}


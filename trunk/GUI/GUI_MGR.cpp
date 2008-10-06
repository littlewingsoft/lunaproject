
#include <guistdafx.h>


IGUI_Object* CGUI_MGR::m_pLastFocus;
IGUI_Object* CGUI_MGR::m_pFocus;

IGUI_Object::GUI_PTRLIST CGUI_MGR::m_GuiList;
list<TCHAR*> CGUI_MGR::m_EditList;
std::queue<std::string> CGUI_MGR::_DestroyObjNameQ;
IGUIEvent* CGUI_MGR::	m_pEventHandler=NULL;
//--------------------------------------------------
// ���� ���ŵ� ������Ʈ�� �ϰ� ó������ �޴��� 
// � ���̵� Process,Render �� ���� �����ָ� �ȴ�. 
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
	msg.dwMsg = GUIMSG_CREATED; // �������� �뺸�Ѵ�. 
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
//					m_ModalFocus->SetModal( false ); // �̹� ����ΰ� �ִٸ� ��޸����� ����� 
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
//			// ���ڷ� �ʿ��Ѱ�: ��ư�� �ö� ��Ʈ��
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
//	msg.dwMsg = GUIMSG_CREATED; // �������� �뺸�Ѵ�. 
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
////  Add ���ϵ� �ϸ� �θ��� ��ǥ�� �°� ���� 
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
//					m_ModalFocus->SetModal( false ); // �̹� ����ΰ� �ִٸ� ��޸����� ����� 
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
//			// ���ڷ� �ʿ��Ѱ�: ��ư�� �ö� ��Ʈ��
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
//	msg.dwMsg = GUIMSG_CREATED; // �������� �뺸�Ѵ�. 
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
////  Add ���ϵ� �ϸ� �θ��� ��ǥ�� �°� ���� 
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

// ��� ��Ŀ�� �� 
// ��Ŀ�� �� 
// ��Ŀ�� ��

void CGUI_MGR::ReCalcEditList(IGUI_Object* pObj) // ���� ������ ����Ʈ �ڽ��� ����Ʈ�� ������ 
{
// ���ڷ� �Ѿ���°Ϳ� ���Ͽ� �˻��ش޶� �ϸ� �װͰ� �װ��� �ڽĵ鸸 ����Ʈ �ڽ����� �˻��Ͽ� �߰� 
// ���ڷ� �Ѿ���°��� ������ ���� ������ ��Ʈ���� ��� �˻��ϰ� 
// �װͰ� �װ��� �ڽĵ��� ����Ʈ �ڽ����� �˻��Ͽ� �߰� 

	m_EditList.clear(); 
	if( pObj != NULL )
	{
		// ������ ������ ������Ʈ�� ���� �ڽĵ鸸 �� 
		if( pObj->GetClsID() == GUI_EDITBOX ) // �ڱ� �ڽź��� �˻��ϰ� 
		{
			m_EditList.push_back( pObj->GetName() );
		}

		// �ڽĵ��� �˻��غ��� ���� �ִ´� 
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
		// ��� �θ�� �� �� �ڽ� ���� �˻���. 2���� �˻� ���� ��
		IGUI_Object::GUI_PTRLIST::iterator it = m_GuiList.begin();
		while( it != m_GuiList.end() )
		{
			IGUI_Object* pTmp = (*it);
			if( pTmp->GetClsID() == GUI_EDITBOX )
			{
				m_EditList.push_back( pTmp->GetName() );
			}
			
			// �ڽĵ��� �˻��غ��� ���� �ִ´� 
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
				// ��⼭ �ڽĵ鵵 ����Ʈ�� ������ �߰��ع��� 
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
	// ����Ʈ �ڽ��� ������ ����Ʈ�� �����Ͽ� 
	// ����Ʈ �ڽ��� ����Ʈ�� ��ȸ�Ͽ� ��Ŀ���� �ݴϴ�.

	if( m_EditList.empty() == true ) return;

	static GUI_EDITLIST::iterator it = m_EditList.begin();
	// ���õ� ���� ���ٸ� ÷���� �ǵ����� 
	// ���� ���õ� ���� ã�� 
	// �װ��� ���������� it �� ���ش� 

	TCHAR* Name = (*it);
	
	if( m_pFocus !=NULL )
	{
		Name = m_pFocus->GetName();
		if( m_pFocus->GetClsID() == GUI_EDITBOX )
		{
			it = find( m_EditList.begin(), m_EditList.end(), Name );
		}
	}

	// ���� �� ��Ŀ���� ���ϵ� ����Ʈ �ڽ� �ΰ�  ����Ʈ �ڽ����
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
	// �ƴ϶�� 
}


/*
1. �ڽ��� �ְ� �θ� ã�Ƴ���. ���� �θ� ���ٸ� �ڽ��� �ְ� �θ���.
2. �θ� ����Ʈ �߿��� �����ڸ����� ���� �ǵڿ� �߰��Ѵ�. 
*/
void CGUI_MGR::SetGuiFocus( const TCHAR* Name )
{ 
	// ���� ��Ŀ�� �������� ����϶�
	// ����� �ƴҶ� 
	IGUI_Object* pGui = FindGui( Name );
	
	IGUI_Object* pLast = NULL;
	if( m_pFocus != NULL )
	{
		// ���⼭ �ڽ��� �ڽ����ϳ��� ��Ŀ�� �����ٸ�
		// �� �޼����� ������ �ʴ´� 
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
			
		// �ֻ��� �θ� ã�Ұ� �� �θ�� ���̰ų� �Ǵ� ���ϼ� �ִ�
		// �츮 �θ� �ƴϰ� ���� �ƴϸ� �� �� �޼����� ��������.

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
		if( m_pFocus != NULL ) // ������ ���õȰ��� �ִٸ�.. �װ��� �θ� ã�´�.
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
		// �ֻ��� �θ� ã���� �θ� �ǵ�( ȭ�鿡�� �ֻ��� ) �� ������.
		//if( pParent->GetClsID() == GUI_PANNEL ) 
		if( pParent != NULL ) 
		{
			IGUI_Object::GUI_LISTITOR it = find( m_GuiList.begin(), m_GuiList.end(), pParent );
			m_GuiList.erase( it );
			m_GuiList.push_back( pParent ); // �ǵڷ�.. 
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
	// ���콺�� �����϶� ���� ��Ŀ���� �ٲ��� �Ѵ�.
	// ���콺�� ���� �ִ°��� GUI ���Ը� �޼����� �����Ѵ�.
	// �̰��� ��Ű���� ������ ���콺�� �ִ� ��ġ�� ��Ŀ����
	// ���콺�� ���� GUI �� ��Ŀ�� �ΰ����� �����ؾ� �Ѵ� 
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
	// ���� ��Ŀ���� �ִ°��� ...������ �԰Ű� �غ귯 ���� 
	// ����� �ִٸ� �ƹ������� �޼����� ������ �ʿ䰡 �����ϴ�.
	// ����� ó�� �ؾ� �ϴµ�...
		if( (m_pFocus!=NULL) && m_pFocus->IsModal() == true )
		{
			pMouseFocus = m_pFocus;
		}


	// ���� ��Ŀ���� �ִ°��� �޼����� ������.
	// ��Ŀ���� ���ٸ� ÷���� ��� �̵鿡�� �޼����� ���� 
	// ��Ŀ���� �ִ°��� �׻� ������ ���ĵ�.
	// ������ â���� ����ǰ� ���߿� ��ư �ؽ�Ʈ ���ϸ� �� ��� ����ɰ���.
	
	if( pMouseFocus != NULL ) // ��Ŀ���� �ִٸ� �װ͸� ����� 
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
	else // ��Ŀ�� ������ ������ ��� ���ư��� �޼��� ���� 
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
	// �θ���� ��� ���鼭 �� �θ��� �ڽĶ��� ��� ���ƹ�����.
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

void CGUI_MGR::DestroyObjQueue( const TCHAR* Name )  // �̸��� ť�� ���常 �س��´�.
{
	_DestroyObjNameQ.push( Name );
}

// �ڱ�, �ڱ��ڽ��� �ְ�θ� �������
// ����Ŀ���� �ٲ������ 
void CGUI_MGR::DestroyObj( const TCHAR* Name )// ������ �����ش�.
{
	// �ڽ��� ���� �˻��ѵ� �� ���� ���ش�. 
	// ���ݴٸ� ���� �ع����� 
	// �ƴ϶�� �� �θ� ���ִ� ���̹Ƿ� 

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
	// �ڽĵ鿡�� ���� �����ش�.
	// ���� �ڽ��� �ڽĵ��� ��ο��� ��������� �ڽ��� �ڽĵ鿡�� �����Ұ���
	// ��������� ������ �ڽĵ���� ���ʴ�� ��������
	// ���Ŀ� �ֻ��� �θ� �����ɰ��� 

	IGUI_Object::GUI_LISTITOR it = NULL;
	it = find( m_GuiList.begin(), m_GuiList.end(), pDestGui ); // �ڽ��϶��� �����ؾ� ����
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
	// ȭ�� 800, 600 �� �߰��� ��ġ�� �������ش� 
	int x = 400 - (pObj->m_Img.sprInfo.nWidth>>1);
	int y= 300 - (pObj->m_Img.sprInfo.nHeight>>1);

	pObj->MoveGUI( -pObj->GetX(), -pObj->GetY() );
	pObj->MoveGUI( (float)x, (float)y );
}

void CGUI_MGR::ModalBox(TCHAR* szStr, ICommand* pCommand )
{
	//CGUI_Pannel* pPannel =(CGUI_Pannel*) FindGui( _T("�ܼ�â") );

	DestroyObj( _T("�ܼ�â"));
	CGUI_Console* pConsole = GUI_CreateObj<CGUI_Console>( _T("�ܼ�â"),_T("data\\IMG\\Gui\\notice_back.ani"),0,0,-1,-1 );
	AddGUI( pConsole );
	pConsole->SetCommand( pCommand );
	pConsole->AddString( szStr );
//	if( pPannel == NULL )
//	{
//		pPannel = GUI_CreateObj<CGUI_Pannel>( _T("�ܼ�â"),_T("data\\IMG\\Gui\\notice_back.ani"),0,0,-1,-1 );
//		AddGUI( pPannel);
//
//		// �ؽ�Ʈ�� ���� �ø���. 
//		CGUI_ListBox* pList = GUI_CreateObj<CGUI_ListBox>(  "�ܼ�â_�۸���Ʈ", NULL ,67,38, 230,124 ); 
////		pList->SetLayer( D3D_INTERFACE_LAYER1 );
//		pPannel->AddChild( pList );
//		pList->AddString( szStr, 0x9f0080ff, 0xffffffff  );
//		CGUI_Button* pBut = GUI_CreateObj<CGUI_Button>( _T("�ܼ�â_�ݱ�"),	_T("data\\IMG\\gui\\Button_BlueOk.ani") ,134,182,-1,-1 );
////		pBut->SetLayer( D3D_INTERFACE_LAYER1 );
////		pBut->SetCommand( new CCommand_CloseWindow( pCommand ) );
//		pPannel->AddChild( pBut );
//	}
//	else
//	{ 
//		CGUI_Button* pBut =(CGUI_Button*) FindGui( _T("�ܼ�â_�ݱ�") );
////		if( pBut != NULL )
////		pBut ->SetCommand( new CCommand_CloseWindow( pCommand ) );
//
//		CGUI_ListBox* pList =(CGUI_ListBox*) FindGui( _T("�ܼ�â_�۸���Ʈ") );
//		pList->RemoveAllContents();
//		if( pList  != NULL )
//		{
//			pList->AddString( szStr , 0x9f0080ff, 0xffffffff );
//		}
//	} 
//	SetGuiFocus(_T("�ܼ�â")); 
//	MoveCenter( _T("�ܼ�â") );
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


// ���� INI �� �����Ȱ͵��� �߰�,������.
// �ӽ������� ��ư�� �׽�Ʈ�غ���
// GUI_Ani,EditBox,ListBox, Pannel,GUI_TEXT � �߰�����
// CreateObject �� ������ 
// ��ư ������ ������ 256���� ����.
/*
[BUTTON0]
�̸� = "���ΰ�ħ"
IMGPATH = "Data\\IMG\\Lobby\\Button_Refresh.ani"
pos_x = 11
pos_y = 30
�θ� = ����
�߻��̺�Ʈ = GUIMSG_WINDOW_CLOSE
*/
bool CGUI_MGR::_CreateFromINI( const TCHAR* szName) 
{
	TCHAR Tmp[256]={0,},szGUIName[256]={0,}, szAniPath[256]={0,};
	TCHAR szParentName[128]={0,};
	int x=0,y=0,Width=-1,Height=-1;
	int bEnable = 1;
	int ButtonCnt = GetPrivateProfileInt( "GUI_COUNT","��ư����",0,szName);
	BYTE Layer = D3D_MAX_FAR_LAYER2;
	for( int i=0; i< ButtonCnt; i++)
	{
		_stprintf( Tmp, "��ư%d",i);
		GetPrivateProfileString( Tmp, "�̸�", "������", szGUIName, 256, szName);
		GetPrivateProfileString( Tmp, "IMGPATH", "Untitled.ani", szAniPath, 256, szName);
		GetPrivateProfileString( Tmp, "�θ��̸�", "", szParentName, 128, szName);
		
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
			MessageBox(0,str, "�߸�������",MB_OK);
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

// ���� �߰�,�����Ǿ� �մ°͵��� INI �� �ִ°͵鸸 ���ֹ���
void CGUI_MGR::_DestroyFromINI(const TCHAR* szName)
{
	int Cnt=0;
	TCHAR Tmp[256]={0,},szGUIName[256]={0,} ;
	int ButtonCnt = GetPrivateProfileInt( "GUI_COUNT","��ư����",0,szName);
	for( Cnt=0; Cnt<ButtonCnt ; Cnt++)
	{
		_stprintf( Tmp, "��ư%d",Cnt);
		GetPrivateProfileString( Tmp, "�̸�", "������", szGUIName, 256, szName);
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

		if( szClass == "Invalid" ) continue; // ����

		_Ini.GetValue_String( Section_ButtonName, "IMGPATH", "Untitled.ani", szAniPath );
		_Ini.GetValue_String( Section_ButtonName, "�θ��̸�", "", szParentName );
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
			MessageBox(0,str, "�߸�������",MB_OK);
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


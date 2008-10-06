
#include <guistdafx.h>


/////////////////////////////////////////////////
//	About Scroll Button
//
//	�߰��� ����
//	2004.4.28 ��ũ�ѹ� ũ�⺸�� ��ũ�� â�� ������
//	          ��ũ�� �ٴ� ������Ű�� ����.
//
/////////////////////////////////////////////////
CGUI_ListBox_SCROLL::CGUI_ListBox_SCROLL():m_PointPos(0)
{
	m_pBack = NULL;
	m_pUp = NULL;	
	m_pBar= NULL;	
	m_pDn = NULL;	
	_tcscpy( m_Name , _T("SCROLLBAR") );
}
#define AVAILSIZE 1.5f //��ũ�ѹ� ũ��* AVAILSIZE �غ��� ����� �ȸ����� �����Ұ�
CGUI_ListBox_SCROLL::CGUI_ListBox_SCROLL(CGUI_ListBox* pParent ):m_PointPos(0)
{
	_IsDown= false;
	m_pBar= NULL;
	assert( pParent != NULL );
	if( pParent == NULL ) 
		return;
	_tcscpy( m_Name , pParent->GetName() );
	_tcscat( m_Name , _T("_SCROLLBAR") );
	pParent->SetScrollName( m_Name );
	SetParent( pParent );

	CGUI_MGR::AddGUI( this );

	m_pUp = GUI_CreateObj<CGUI_Button> ( _T("��ũ��_��"),pParent->GetUPButtonImgFileName() ,0,0,-1,-1 );
	m_pDn =GUI_CreateObj<CGUI_Button> (_T("��ũ��_�ٿ�"),pParent->GetDNButtonImgFileName(),0,0,-1,-1 ); 
	m_pBar =GUI_CreateObj<CGUI_Button> ( _T("��ũ��_����Ʈ"),_T("data\\IMG\\GUI\\Scroll_Bar.ani"), 0,0,-1,-1 );

	// ���� ������ ��ũ�� ���� ũ�� 2�� �Ѱ� ��ũ�ѿ������� �۴ٸ� 
	// �޸𸮿��� ���� ������ �߰� ���� �ʴ´�.
	//if( pParent->GetWidth() < (m_pBar ->GetWidth()* AVAILSIZE ) || 
	//	 pParent->GetHeight()<(m_pBar->GetHeight()* AVAILSIZE ) )

	float Interval_ToBody = 0.0f;
	pParent->GetInterval_ToBODY(Interval_ToBody );
	float Interval_ToHead = 0.0f;
	pParent->GetInterval_ToHEAD(Interval_ToHead );
	float Interval_ToTail = 0.0f;
	pParent->GetInterval_ToTAIL(Interval_ToTail );

	fRECT rt;
	pParent->GetBoundRect( rt ); // ���� ����Ʈ�� ������ �����սô�.
	RECT MyRect = { long(Interval_ToBody+ rt.right) ,  // ����Ʈ�� ������ �����Ƿ� �ٵ��� �� 
					long(Interval_ToHead+ rt.top+m_pUp->GetHeight()) ,  
					long(Interval_ToBody+ rt.right + m_pUp->GetWidth()), 
					long(Interval_ToTail+ rt.bottom - m_pDn->GetHeight()) }; 
	// ���߿� �Ӽ��� ���� ��� �������� �� DestRect �� �����ؾ� �ҵ�.
	
	SetDestRect( (float) MyRect.left, (float)MyRect.top ,  (float)MyRect.right, (float)MyRect.bottom );
	SetBoundRect( (float)MyRect.left, (float)MyRect.top ,  (float)MyRect.right, (float)MyRect.bottom );
	// �θ�â�� ������ ������ ���� �մϴ�. 


	CGUI_Ani* pBack = GUI_CreateObj<CGUI_Ani>( "��ũ��_��",_T("data\\img\\etc\\WhiteFade.ani"), 0,0,(int) GetWidth(), (int)GetHeight());
	pBack->SetClr( D3DCOLOR_ARGB(0xff, 112,155,214 ) );

	AddChild( pBack );
	AddChild( m_pUp );
	
	if( m_pBar ) // �����Ȱ��̶�� �߰���Ų��. 
	AddChild( m_pBar );
	AddChild( m_pDn );


	m_pUp   ->MoveGUI( 0,  - m_pUp->GetHeight()  );
	m_pDn   ->MoveGUI( 0, GetHeight() );

	if( (m_pBar->GetHeight()*AVAILSIZE )  >=  m_pDn->GetY() - (m_pUp->GetY()+m_pUp->GetHeight() ) )
	{
		SAFE_DELETE( m_pBar );
	}

	if( m_pBar )
	{
		float move = (GetWidth()/2) - (m_pBar->GetWidth() /2);
		m_pBar->MoveGUI( move, 0); //GetHeight() - //m_pBar->GetHeight() 
	}

	_localx = GetX() - pParent->GetX(); // ���� �����ġ = ���� ��ġ - �θ��� ������ġ
	_localy = GetY() - pParent->GetY(); // ���� �����ġ = ���� ��ġ - �θ��� ������ġ
}

CGUI_ListBox_SCROLL::~CGUI_ListBox_SCROLL()
{
	// ��ư �̹��� �Ҹ�?
}

void	CGUI_ListBox_SCROLL::HEAD()
{
	if( m_pBar==NULL) return;
	m_pBar->SetY( m_BoundRect.top  );
}


void	CGUI_ListBox_SCROLL::TAIL()
{
	m_PointPos= 100; // �� ������ �ű�.
	if( m_pBar==NULL) return;
	// ��ũ�ѹ� ���� �϶�� �޼��� ������ �ҵ�...

	float point_Height = 0.0f;
	point_Height = m_pBar->GetHeight();
	float y = (m_BoundRect.bottom - point_Height );
	m_pBar->SetY( y );
}

void	CGUI_ListBox_SCROLL::Process()
{
	// ��ư ������ �� ���ؼ� ��Ʈ���� ���⼭ �մϴ�.
	if( m_pUp )
	if( m_pUp->IsEvent() )
	{
		// �� �θ� ��Ŀ���� ������ �մϴ�.
		CGUI_MGR::SetGuiFocus( m_pParent->GetName() );
		OnUP();
	}
	if( m_pBar )
	if( m_pBar->IsEvent() )
		CGUI_MGR::SetGuiFocus( m_pParent->GetName() );

	if( m_pDn )
	if( m_pDn->IsEvent() )
	{
		CGUI_MGR::SetGuiFocus( m_pParent->GetName() );
		OnDown();
	}

	// ��ũ�� ����Ʈ�� �� �ι�ư�� ���Ͽ� ���� ����ŭ �������ų� �����ϴ�.
	if( g_MouseMGR.IsLUp() )
	{_IsDown  = false;}

	if( _IsDown )
	{
		GUIMSG msg;
		msg.dwMsg = GUIMSG_DRAG;
		msg.dwParam1= g_MouseMGR.GetX();
		msg.dwParam2= g_MouseMGR.GetY();
		this->ProcMessage( msg );
	}

	Process_Child();
}

void	CGUI_ListBox_SCROLL::Render()
{
	if( this->m_pParent )
	{
		SetX( m_pParent->GetX() + _localx );
		SetY( m_pParent->GetY() + _localy );
	}
	 Render_Child();
}
#define INTERVALPOINT 10 //100�������� �ѹ� ��ũ���Ҷ��� ��
// ������ذ��� ������ ��ũ�ѹ��� ��ġ�� �����ݴϴ�. 
#define BARREFRESH() {\
	if( m_pBar )\
	{\
		float _height = GetHeight()- m_pBar->GetHeight();\
		float y = (m_PointPos*_height) / 100.0f;\
		m_pBar->SetY( int(m_BoundRect.top + y) );\
	}\
}
void CGUI_ListBox_SCROLL::OnUP()
{
	/*
			���� ���ذ��� ���� m_PointPos �� 0~100 ���� ��´�.
			���� ���ذ����� ��ũ�� �ٿ� ��ũ�� �並 ������ŭ �̵���Ű�°��̴�.
			���� �並 �̵��ϰڴ�.
			1.m_PointPos-=10; ������ ������ŭ ���� ��Ŵ <0,100>
			if( m_PointPos <0) m_PointPos=0;

			��ũ�� �ٰ� �ִٸ� �ڽ��� ��ġ�� ȯ���Ŵ
			2. m_PointPos : 100 == x : bar_height -> x = (m_PointPos*bar_height) / 100;
			3. �θ𿡰� �޼��� ���� */

	m_PointPos-=INTERVALPOINT ;
	if( (int)m_PointPos <0) m_PointPos=0;
	BARREFRESH();
	OBJMSG Msg;
	Msg.dwMsg = GUIMSG_SCROLL_VERTICAL_DRAG;
	Msg.dwParam1 = m_PointPos; 
	m_pParent->ProcMessage( Msg );
}

void CGUI_ListBox_SCROLL::OnDown()
{
	m_PointPos+=INTERVALPOINT;
	if( (int)m_PointPos >100) m_PointPos=100;

	BARREFRESH();
	OBJMSG Msg;
	Msg.dwMsg = GUIMSG_SCROLL_VERTICAL_DRAG;
	Msg.dwParam1 = m_PointPos; 
	m_pParent->ProcMessage( Msg );	
}
			
bool	CGUI_ListBox_SCROLL::ProcMessage(GUIMSG& pMsg)
{
	// ���콺 Ŭ���� �������� 
	// ���콺 ���갡 ��������
	// Ű���� �� �Ʒ��� �������� 
	// ������ ���ٿ ���� 
	// Ȩ, ���嵵 ���� 

	if( ProcMessage_Child( pMsg ) == true ) 
		return true;

	static POINT ScrollDown={0,};
	switch( pMsg.dwMsg )
	{

	case GUIMSG_BUTTON_EVENT:
		{
			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( pMsg.dwParam1);
			if( strcmp(pButton->GetName(),"��ũ��_����Ʈ" ) ==0 )
			{
				// ���ȴ��� �������� üũ
				if( pButton->GetState() == CGUI_Button::FOCUSON )
					_IsDown = false;
				else if( pButton->GetState() == CGUI_Button::DOWN )
					_IsDown = true;
			}

		}return false;

	case GUIMSG_MOVEWND:
		{
			MoveGUI( (float)pMsg.dwParam1, (float)pMsg.dwParam2 );
		}
		return false;

	case GUIMSG_SCROLL_PREV: // �̰ɷ� �޼����� ������..
		{
			IGUI_Object* pSuperParent = m_pParent->GetParent();
			if( pSuperParent == NULL ) pSuperParent=m_pParent;

			while( pSuperParent->GetParent() != NULL )
				pSuperParent = pSuperParent ->GetParent();

			if( CGUI_MGR::GetCurrFocus() == this || CGUI_MGR::GetCurrFocus() == pSuperParent )
			OnUP();
			else 
			return false;
		}
		return true;

	case GUIMSG_SCROLL_NEXT:
		{
			IGUI_Object* pSuperParent = m_pParent->GetParent();
			if( pSuperParent == NULL ) pSuperParent=m_pParent;

			while( pSuperParent->GetParent() != NULL )
				pSuperParent = pSuperParent ->GetParent();

			if( CGUI_MGR::GetCurrFocus() == this || CGUI_MGR::GetCurrFocus() == pSuperParent )
			OnDown();
			else 
			return false;
		}
		return true;

	case GUIMSG_MLUP:
		_IsDown = false;
		return false;
	case GUIMSG_MLDOWN:
		{
			int x =int( pMsg.dwParam1 );
			int y =int( pMsg.dwParam2 );
			
			if( m_BoundRect.left <= x  &&
				x <= m_BoundRect.right &&
				m_BoundRect.top  <= y  &&
				y <= m_BoundRect.bottom )
			{
				// ���� 1���� �θ�� ����Ʈ �ڽ��̳� ���� �θ� ���� �ƴϸ� 
				// �׿��� ��Ŀ���� ������ �ؾ� �Ѵ�.
			IGUI_Object* pSuperParent = m_pParent->GetParent();
			if( pSuperParent == NULL ) pSuperParent=m_pParent;

			while( pSuperParent->GetParent() != NULL )
				pSuperParent = pSuperParent ->GetParent();
			
				CGUI_MGR::SetGuiFocus( pSuperParent->GetName() ); // ���� �ְ� �θ� ��Ŀ���� ������ �ؾ���
					
			}
		}
	return false;
	case  GUIMSG_DRAG:
		{
			if( m_pBar == NULL  ) return false;
			//if( pMsg.dwParam1 < m_BoundRect.left  &&
			//	pMsg.dwParam1 > m_BoundRect.right &&
			//	pMsg.dwParam2 < m_BoundRect.top   ||
			//	pMsg.dwParam2 > m_BoundRect.bottom )
			RECT rt = {m_BoundRect.left,m_BoundRect.top,m_BoundRect.right,m_BoundRect.bottom };
			if( IsPtInRect( rt,pMsg.dwParam1,pMsg.dwParam2) == false && _IsDown == false )
			{
				return false; // ������ �ش��ϴ� �޼����� �ƴϴ� 
			}
			// ���� ���콺 ��ġ�� ��ũ�� �� ������ 100 �������� ��ġ�� ���ϰ�
			// �� 100���� ������ ���� ��ũ�� �ٷ� ȯ�� �����Ѵ�. �̺κ��� �Ǿ� �����Ƿ� 
			// ��ũ�ѹ� ���������� 100���� ��ġ�� ������.
			int mousex = pMsg.dwParam1;
			int mousey = pMsg.dwParam2;
			//				���콺��ġ - �ɼ���ġ / �ٿ�巺Ʈ ���� - 
			int intervaly = m_pBar->GetFirstClick().y;
			float Bar_y = m_pBar->GetY() - m_BoundRect.top;
			float Bar_Height = m_pBar->GetHeight();
			float newy = 0.0f;
			float Enable_Height = GetHeight()-Bar_Height ;
			m_PointPos = Bar_y/Enable_Height*100.0f ;

			int y =int( pMsg.dwParam2 - intervaly ); // ������ ��������� �մϴ�.

			if( y < m_BoundRect.top )// �� ���ų� Ŀ�� �ϰ� 
			{
				y = int(m_BoundRect.top); 
			}
			else if( y+Bar_Height>m_BoundRect.bottom )
			{
				y = m_BoundRect.bottom - Bar_Height;
			}
			
			m_pBar->SetY( float(y) ); //m_BoundRect.top+

			// ����Ʈ�� ���������Ƿ� 
			// ��ũ���� ������ ��ŭ ������ �ش�. 
			// ����Ʈ�� �����ϼ� �ִ� ������ ��ü DESTRECT ���� ���Ʒ� ���� point_Height /2 �� ��ŭ �̴�.
			// �׸�ŭ�� �̵������� ������ ��ü�� ���� ���� ��ġ�� ��ü������ ������� ��ġ�� 2048 ������ �Ѵ�.
			// 2�� �¼� ���̹Ƿ� / ����� �Ҽ��� 5�ڸ� ������ ������ ������ ���� 
			// ���� ���콺 ��ġ???? �巹�� �ÿ��� 100������ ���� �̵���... 
			//RECT PointRt={0, 0, (long)m_pBar->GetWidth(),  long(GetHeight() - (point_Height)) };
			//float	tmpY = (m_pBar->GetY() +(m_pBar->GetHeight()/2) )- (m_BoundRect.top + (m_pBar->GetHeight()/2) );
			//m_PointPos = (float)tmpY / PointRt.bottom*2048;

			GUIMSG Msg;
			Msg.dwMsg = GUIMSG_SCROLL_VERTICAL_DRAG;
			Msg.dwParam1 = m_PointPos;
			m_pParent->ProcMessage( Msg );
			return true;
		}
	}



	return false;
}


////////////////////////////////////////////////
//
// List Box ���� Ŭ���� 
//
////////////////////////////////////////////////
CGUI_ListBox::CGUI_ListBox():m_UpButtonImgFileName("data\\IMG\\GUI\\Scroll_Up.ani"),m_DnButtonImgFileName("data\\IMG\\GUI\\Scroll_Dn.ani")
{ 
	m_Gui_ClsID= GUI_LISTBOX;
	SetLineCnt( 64 );
	
	m_ContentsRect.left=
	m_ContentsRect.right= 
	m_ContentsRect.top = 
	m_ContentsRect.bottom = 0;
	m_LastScrollPos = 0;
	m_pScroll = NULL;
	m_Interval_ToBODY =0.0f;
	m_Interval_ToHEAD =0.0f;
	m_Interval_ToTAIL =0.0f;
	_tcscpy( m_Name , _T("GUI_LISTBOX") );
	m_FocusIndex = 0;
}

CGUI_ListBox::~CGUI_ListBox()
{
	SAFE_DELETE( m_pScroll ); // ������ ��ũ���� ���.
}

void CGUI_ListBox::SetLineCnt( UINT MaxContentsCnt ) // ���� �����ش�� ���ι��۰� �����˴ϴ�.
{
	// ���� �ִ��͵��� �� ���󰡳�..?
	m_ChildList.clear();
	m_ChildList.reserve( MaxContentsCnt );
}

void CGUI_ListBox::AddContents( IGUI_Object* pObj )
{
	int  aa = int( m_ChildList.capacity()-1);
	int siz =(int) m_ChildList.size();
	while( siz  > aa )
	{

		IGUI_Object* pObj = m_ChildList[0];
		int Height = (int) pObj->GetHeight();
		m_ContentsRect.bottom -= Height;

		// delete �ϴ� ��ƾ�� ÷�� �Ǿ� �ҵ�.. 
		m_ChildList.erase( m_ChildList.begin()  );
		SAFE_DELETE( pObj );
		siz = (int)m_ChildList.size();
		// ù��° ���ĺ��� ������ ��� ���� ��ܿ;� �Ѵ�..
		int Size = (int)m_ChildList.size();
		for( int i=0; i<Size ; i++)
		{
			pObj = m_ChildList[i];
			pObj->MoveGUI( 0, (float)-Height );
			pObj->CalcViewRect( (fRECT*)&m_DestRect );
		}

	}
	int Size =(int) m_ChildList.size();
	
	AddChild( pObj );

	float Y =0.0f;
	if( Size != 0 )
	Y =  m_ChildList[Size-1]->GetY() +  m_ChildList[Size-1]->GetHeight() - m_DestRect.top ; //  m_ContentsRect.bottom;


	pObj->SetBoundRect( m_BoundRect.left, 
						m_BoundRect.top + Y, 
						m_BoundRect.left + pObj->GetWidth() ,
						m_BoundRect.top + Y + pObj->GetHeight() );

	pObj->SetDestRect( m_DestRect.left, 
						m_DestRect.top + Y, 
						m_DestRect.left + pObj->GetWidth() ,
						m_DestRect.top + Y + pObj->GetHeight() );

	this->m_ContentsRect.bottom += (long)pObj->GetHeight();

	if( m_DestRect.bottom - m_DestRect.top < m_ContentsRect.bottom)
	{
		// ���⼭ ��ũ�� �ٸ� �����մϴ�.
		if( m_pScroll == NULL )
		{
			m_pScroll = new CGUI_ListBox_SCROLL(this);
		}
		TAIL(); 
	}
}

void CGUI_ListBox::AddString( TCHAR *szStr, D3DCOLOR OutClr,D3DCOLOR FontClr,bool bFocusString )
{
	// �� �ɼ��� �ش��� ���̰� �ɵ�...
	// 1.�ȼ��˻�. ���ѿ����� ũ�⺸�� ���ų� ũ�� �װ��� ������Ʈ���� 1���ɼ� 
	// 2. �ѱ����� �˻��Ͽ� 1���ɼ��� ���ش�. �ѱ��� �ȳ��ö� ���� IsDBCSLeadByte �� �����Ͽ� FALSE �� 
	// ���� ������ ���� �ɼ��� ��� ���ش�. 
	// 2.5 �� �ɼ��� ���� ��Ʈ���� ������ġ�� ���ÿ� ���� ��Ʈ���� ���̴�.
	// ���� ��Ʈ���� �ڽ��� ���� +1 �Ͽ� �ι��ڸ� ���� �ִ´�.
	// 3. �� �˻縦 ��ģ �ɼ°� ���̰� ������ 
	// �� �ɼ°� ���̷ν� ��Ʈ���� �ø���. 
	int StrCnt = 0;
	int StringOffset[64]={0,};
	int Length[64]={0,};
	int szLen =(int) _tcslen( szStr );
	
	SIZE sz = GetStrPixel( CGUI_Edit::GetEDITHWND(), g_D3D.GetGDIFont(), szStr );
	float Width = GetWidth();
	if( sz.cx <= Width ) // ��Ʈ���� �ϳ���� 
	{
		StrCnt= 1;
		Length[0]=szLen;
	}
	else // ������ ��� 
	{
		int offset=0;
		int TmpLen = 0;//_tcslen( &szStr[offset] );			
		while( 1 ) // �� ���� ��Ƽ�� @,.@;
		{
			TCHAR TmpBuff[256]={0,};
			_tcsncpy( TmpBuff, &szStr[offset], TmpLen );

			sz = GetStrPixel( CGUI_Edit::GetEDITHWND(), g_D3D.GetGDIFont(), TmpBuff );
			if( sz.cx < Width )
			{
				//	���ڳ� �ѱ��̸� 2����Ʈ ����
				//	�����̳� ���ڸ� 
				if( IsDBCSLeadByte( TmpBuff[TmpLen] ) == TRUE ) //szStr
				TmpLen+=2;
				else
				TmpLen+=1;

				if( szLen < offset+TmpLen )
				{
					Length[ StrCnt ] = TmpLen;//szLen - offset-1;// �������� �ɼ��� ������ �װ�;
					StrCnt++;
					break;
				}
			}
			else // ������ �Ǿ� �߰� �ؾ� �մϴ�. 
			{
				if( IsDBCSLeadByte( TmpBuff[TmpLen-1 ] ) == TRUE )// �ѱ��̶��  //szStr
				{
					TmpLen-=2;
				}else
					TmpLen-=1;

				offset += TmpLen;
				Length[ StrCnt ] = TmpLen  ;//TmpLen;// �������� �ɼ��� ������ �װ�;
				StrCnt++;
				StringOffset[StrCnt] = offset ;//TmpLen-1;
				TmpLen = 0;
				TmpBuff[0]='\0';
			}
		}
	}

	for( int Index=0; Index < StrCnt; Index++)
	{
		TCHAR tmpBuff[256]={0,};
		_tcsncpy( tmpBuff, &szStr[ StringOffset[Index] ], Length[ Index ] );
		tmpBuff[ Length[ Index ] ] = '\0';
		int  aa =int( m_ChildList.capacity()-1);
		int siz =(int) m_ChildList.size();
		while( siz  > aa ) // ? ���� �͵��� ��� �Ʒ���..?
		{
			IGUI_Object* pObj = m_ChildList[0];
			int Height =(int) pObj->GetHeight();
			m_ContentsRect.bottom -= Height;
			m_ChildList.erase( m_ChildList.begin()  );
			SAFE_DELETE( pObj );
			siz =(int) m_ChildList.size();
			// ù��° ���ĺ��� ������ ��� ���� ��ܿ;� �Ѵ�..
			int Size =(int) m_ChildList.size();
			for( int i=0; i<Size ; i++)
			{
				pObj = m_ChildList[i];
				pObj->MoveGUI( 0, (float)-Height );
				pObj->CalcViewRect( (fRECT*)&m_DestRect );
			}
		}
		
		// ������ DestRect �� width ���� ��Ʈ���� ���� üũ�Ͽ� 
		// ��ٸ� �� �Ʒ� �������� ����.
		
		int Size =(int) m_ChildList.size();
		
		IGUI_Object* pTmp = m_ChildList[Size-1];
		float Y =0.0f;
		if( Size != 0 )
			Y = pTmp->GetY() +  pTmp->GetHeight() - m_DestRect.top ; //  m_ContentsRect.bottom;

		CGUI_Text* pText = NULL;
		if( bFocusString == true )
		{
			pText = CGUI_MGR::CreateFocusText( this->GetName(), _T("������"), 0, (int) Y , OutClr, FontClr, tmpBuff ); 
		}
		else
		{
			pText = CGUI_MGR::CreateText( this->GetName(), _T("������"), 0,(int) Y , OutClr, FontClr,  tmpBuff ); 
		}

		
		this->m_ContentsRect.bottom +=(long) pText->GetHeight();
		pText->CalcViewRect( (fRECT*)&m_DestRect );
		if( m_DestRect.bottom - m_DestRect.top < m_ContentsRect.bottom)
		{
			// ���⼭ ��ũ�� �ٸ� �����մϴ�.
			if( m_pScroll == NULL )
			{
				m_pScroll = new CGUI_ListBox_SCROLL(this);
			}
			// ���⼭ �� �Ʒ��� ������ �մϴ�.
			TAIL();
		}
	}	
}

// �� �Ʒ��� �����ݴϴ�.
// ��ũ�� ����Ʈ�� �ǾƷ��� ������ ��ũ�� �޼����� �θ𿡰�(����Ʈ�ڽ�) �����ϴ�. 
void CGUI_ListBox::HEAD()
{
	
	int ListSize =(int) m_ChildList.size();
	if( ListSize == 0 ) return;
	IGUI_Object* pObj = m_ChildList[ 0 ];
	float Mn = m_DestRect.top  - pObj->GetY() ; //- pObj->GetHeight()	

	
	// �� �������� �ִ� ���� ��ġ�� ���� ���ϼ� �ִ� �� �ٴ��� ��ġ���� ���� ���Ͽ� 
	// �� ��ŭ ��� ���� ���� ��� �÷����� 
	
	for( int i=0; i<ListSize ; i++)
	{
		pObj = m_ChildList[i];

		pObj->MoveGUI( 0, Mn );
		pObj->CalcViewRect( (fRECT*)&m_DestRect );
	}	

	if( m_pScroll )
	m_pScroll->HEAD();
	m_LastScrollPos= 0;
}

void CGUI_ListBox::TAIL()
{
	int ListSize =(int) m_ChildList.size();
	IGUI_Object* pObj = m_ChildList[ ListSize - 1 ];
	float Mn = pObj->GetY() - ( m_DestRect.bottom - pObj->GetHeight() );

	// �� �������� �ִ� ���� ��ġ�� ���� ���ϼ� �ִ� �� �ٴ��� ��ġ���� ���� ���Ͽ� 
	// �� ��ŭ ��� ���� ���� ��� �÷����� 

	for( int i=0; i<ListSize ; i++)
	{
		pObj = m_ChildList[i];
		pObj->MoveGUI( 0, -Mn );
		pObj->CalcViewRect( (fRECT*)&m_DestRect );
	}	

	if( m_pScroll )
	m_pScroll->TAIL();

	m_LastScrollPos= 100;
	
	// ��Ŀ������ ���������� ������ �������ݴϴ�.

}

bool CGUI_ListBox::ProcMessage(GUIMSG& pMsg)
{
	// ���� �����̿��� �޼������ ���ϵ忡�Դ� ������ �ʾƾ� �ϴµ�... 
	if( pMsg.dwMsg == GUIMSG_MLDOWN )
	{
		int x =int( pMsg.dwParam1 );
			int y =int( pMsg.dwParam2 );
			
			if( m_BoundRect.left <= x  &&
				x <= m_BoundRect.right &&
				m_BoundRect.top  <= y  &&
				y <= m_BoundRect.bottom )
			{
				if( this->ProcMessage_Child( pMsg ) == true )
				return true;			
			}
	}



	m_pScroll = (CGUI_ListBox_SCROLL*) CGUI_MGR::FindGui( _ScrollName.c_str() );
	if( m_pScroll != NULL )
	{
		if( m_pScroll->ProcMessage( pMsg ) == true )
			return true;
	}

	switch( pMsg.dwMsg )
	{

	case GUIMSG_MLDOWN:
		{
			// ���� �������� ��Ŀ�� �� 
			// �� ������ ������ ��Ŀ�� 
			int x =int( pMsg.dwParam1 );
			int y =int( pMsg.dwParam2 );
			
			if( m_BoundRect.left <= x  &&
				x <= m_BoundRect.right &&
				m_BoundRect.top  <= y  &&
				y <= m_BoundRect.bottom )
			{
				// �������߿� Ŭ���� ���� ������ �� 
				// �� �κи�ŭ�� ����~ 
	
				if(m_pParent!= NULL)
				{
					IGUI_Object* pSuperParent = m_pParent->GetParent();
					if( pSuperParent == NULL ) pSuperParent=m_pParent;
					
					while( pSuperParent->GetParent() != NULL )
						pSuperParent = pSuperParent ->GetParent();
					
					CGUI_MGR::SetGuiFocus( pSuperParent->GetName() ); // ���� �ְ� �θ� ��Ŀ���� ������ �ؾ���
				}else
				{
					CGUI_MGR::SetGuiFocus( GetName() ); // ���� �ְ� �θ� ��Ŀ���� ������ �ؾ���
					return true; // ���� �θ� �����Ƿ� �޼����� �ƹ��� ó���� �ʿ� ����
				}
				
				return false; // ������ �θ� ó���Ҽ��� ����
			}
		}

		return false;

		case GUIMSG_SCROLL_VERTICAL_UP:
			{}
		return true;

	case GUIMSG_SCROLL_VERTICAL_DRAG:
		{
			// ���ڴ� �ִ� 100 ������ �Ѿ�ɴϴ�. 
			// 10�̸� ���� ��Ʈ������ ���Ե� �簢���� 10���� �̹Ƿ� �� �簢���� 1000�̸� 100 �ȼ� �̴�.
			// 100 �ȼ��� ���� �� ����� �Ѵ�.
			//
			// ���� ��󿵿��� �������鿡�� ���� Ŭ�������� �ȴ�.
			// 1. �̵��� ��ŭ�� ���ѵ�;
			// 2. �����̵��ȼ� = (�Ⱥ��̴¿��� - ���̴¿��� )/100;  : (�Ⱥ��̴� ���� > ���̴� ���� ) 
			// 3. �����̵��ȼ��� �̵��� �ۼ���������ŭ ���ѵ� ���ų� ������.
			// ���� ��ġ�� - ���� ��ġ��
			// 99 -100 = -1 �̹Ƿ� -1 ��ŭ �̵���Ű�� �� ���� �ø��ٴ� ���..
			// �� -1 �� �ȼ������� ��ȯ ��Ű��?
			// -1 : 100 == x : 364 -> x = ( -1 * 364 / 100) �̹Ƿ�... 
			int dp = m_LastScrollPos-pMsg.dwParam1  ; // ���� ��ġ���� ���̸� ���սô�. 
			if( dp == 0 )
				return true;

			float ContentsHeight = m_ContentsRect.bottom - ( m_DestRect.bottom - m_DestRect.top ) ;
			if( ContentsHeight <= 0 ) // �Ⱥ��̴� ������ ũ�Ⱑ 0���� ������ ����
				return true; 

			float dPixel = (ContentsHeight/100.0f) * dp ; 

			int ListSize =(int) m_ChildList.size();
			// ������ �󸶿����� ������� ���̸� ���Ͽ�
			// �׸�ŭ�� �̵���Ų��. 
			IGUI_Object* pObj=NULL;
			for( int i=0; i<ListSize ; i++)
			{
				pObj = m_ChildList[i];
				pObj->MoveGUI( 0, dPixel );
				pObj->CalcViewRect( (fRECT*)&m_DestRect );
			}
			m_LastScrollPos =  pMsg.dwParam1 ;
		}
		return true;

	}

	return false;
}

void CGUI_ListBox::Process(void)
{
//	Process_Child();
	if( m_pScroll != NULL )
	{
		m_pScroll ->Process();
	}
}

void CGUI_ListBox::Render(void)
{
	if( m_Img.TexID != -1 )
	{
		RECT tmp={ (long)m_DestRect.left,(long)m_DestRect.top, (long)m_DestRect.right,(long)m_DestRect.bottom };
		g_D3D.BltSprite( m_Layer, m_Img.TexID, NULL, tmp, m_Clr, D3D_ALPHASTATE );
	}
	// �ؽ����� �Ϻθ� �����ִ� Ŭ������ �ؾ� ��.
	

//	Render_Child(); // �̰��� �Ƹ� ��ũ�� �ڽ� ���� �ɵ� 
	if( m_pScroll != NULL )
	{
		m_pScroll ->Render();
	}
}


void CGUI_ListBox::RemoveAllContents(void)
{
	//int  aa = (int)( m_ChildList.capacity()-1);
	//int ListSize = (int)m_ChildList.size();

	//IGUI_Object* pObj = NULL;

	//for( int i=0; i< ListSize; i++)
	//{
	//	pObj = m_ChildList[i];
	//	SAFE_DELETE( pObj );	
	//}
	DestroyAllChild();
	m_ContentsRect.bottom =0;
	m_ChildList.clear();// erase( m_ChildList.begin()  );	
	// ��ũ�� �ٵ� ������ �Ѵ�. 
	SAFE_DELETE( m_pScroll );
}


void  CGUI_ListBox::SetInterval_ToHEAD( float Interval )
{
	m_Interval_ToHEAD = Interval;
}

void  CGUI_ListBox::SetInterval_ToTAIL( float Interval )
{
	m_Interval_ToTAIL = Interval;
}

void  CGUI_ListBox::SetInterval_ToBODY( float Interval )
{
	m_Interval_ToBODY= Interval;
}

void CGUI_ListBox::GetInterval_ToHEAD( float& Interval )
{
	Interval = m_Interval_ToHEAD;
}

void CGUI_ListBox::GetInterval_ToTAIL( float& Interval )
{
	Interval = m_Interval_ToTAIL;
}

void CGUI_ListBox::GetInterval_ToBODY( float& Interval )
{
	Interval = m_Interval_ToBODY;
}



// ���� �����ϰ�������� �ε����� �־��ش�
void CGUI_ListBox::SetFocusContentsIndex(int Index ) 
{
	// ���� ���õ� ���� �������� 
	int LastFocus = GetFocusContentsIndex();
	GUIMSG msg;
	msg.dwMsg = GUIMSG_MLDOWN ;
	msg.dwParam1 = 0;
	msg.dwParam2 = 0;
	m_ChildList[ LastFocus ]->ProcMessage( msg );

	RECT rt;
	m_ChildList[ Index ]->GetDestRect( rt );
	// ���⼭ �����ѰͿ��� �־���� �Ѵ�.
	msg.dwParam1 = rt.left+ ((rt.right - rt.left) >>1);
	msg.dwParam2 = rt.top + ((rt.bottom - rt.top )>>1);
	m_ChildList[ Index ]->ProcMessage( msg );
	m_FocusIndex = Index;
}

// ���� ���õȰ��� �ε����� ��ȯ�Ѵ�.
int CGUI_ListBox::GetFocusContentsIndex( void ) 
{
	
	// CGUI_FocusText �϶��� ���� �ִٴ� ������ �ִµ� ���谡 �ϴ� �߸��ȵ�.. 
	// �̷��� �Ǹ� ��� IGUI_x ���� �ڽ��� ��Ŀ���� ������������ �˷��ټ� �־�� �Ѵ�.
	// ��� �͵��� �˷��ټ��� ������ ��ٷ� 
	// ���� ���� ��Ŀ���� ���������� �ǹ�...
	m_FocusIndex = 0;
	if( m_ChildList.empty() == false )
	{
		int ListSize =(int) m_ChildList.size();
		for( int i=0; i<ListSize ; i++)
		{
			CGUI_FocusText * pObj = (CGUI_FocusText *)m_ChildList[i];
			if( pObj->IsFocusOn() ) 
			{
				m_FocusIndex = i;
				break;
			}
		}
	}
	return m_FocusIndex;
}


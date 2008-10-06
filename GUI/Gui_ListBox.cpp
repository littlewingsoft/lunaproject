
#include <guistdafx.h>


/////////////////////////////////////////////////
//	About Scroll Button
//
//	추가된 사항
//	2004.4.28 스크롤바 크기보다 스크롤 창이 작으면
//	          스크롤 바는 생성시키지 않음.
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
#define AVAILSIZE 1.5f //스크롤바 크기* AVAILSIZE 해봐서 사이즈가 안맞으면 생성불가
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

	m_pUp = GUI_CreateObj<CGUI_Button> ( _T("스크롤_업"),pParent->GetUPButtonImgFileName() ,0,0,-1,-1 );
	m_pDn =GUI_CreateObj<CGUI_Button> (_T("스크롤_다운"),pParent->GetDNButtonImgFileName(),0,0,-1,-1 ); 
	m_pBar =GUI_CreateObj<CGUI_Button> ( _T("스크롤_포인트"),_T("data\\IMG\\GUI\\Scroll_Bar.ani"), 0,0,-1,-1 );

	// 만일 현재의 스크롤 바의 크기 2배 한게 스크롤영역보다 작다면 
	// 메모리에서 지워 버리고 추가 하지 않는다.
	//if( pParent->GetWidth() < (m_pBar ->GetWidth()* AVAILSIZE ) || 
	//	 pParent->GetHeight()<(m_pBar->GetHeight()* AVAILSIZE ) )

	float Interval_ToBody = 0.0f;
	pParent->GetInterval_ToBODY(Interval_ToBody );
	float Interval_ToHead = 0.0f;
	pParent->GetInterval_ToHEAD(Interval_ToHead );
	float Interval_ToTail = 0.0f;
	pParent->GetInterval_ToTAIL(Interval_ToTail );

	fRECT rt;
	pParent->GetBoundRect( rt ); // 메인 리스트의 우측에 생성합시다.
	RECT MyRect = { long(Interval_ToBody+ rt.right) ,  // 리스트의 우측에 붙으므로 바디값을 씀 
					long(Interval_ToHead+ rt.top+m_pUp->GetHeight()) ,  
					long(Interval_ToBody+ rt.right + m_pUp->GetWidth()), 
					long(Interval_ToTail+ rt.bottom - m_pDn->GetHeight()) }; 
	// 나중에 속성을 보고서 어디에 생성할지 그 DestRect 를 설정해야 할듯.
	
	SetDestRect( (float) MyRect.left, (float)MyRect.top ,  (float)MyRect.right, (float)MyRect.bottom );
	SetBoundRect( (float)MyRect.left, (float)MyRect.top ,  (float)MyRect.right, (float)MyRect.bottom );
	// 부모창에 오른쪽 좌측에 생성 합니다. 


	CGUI_Ani* pBack = GUI_CreateObj<CGUI_Ani>( "스크롤_백",_T("data\\img\\etc\\WhiteFade.ani"), 0,0,(int) GetWidth(), (int)GetHeight());
	pBack->SetClr( D3DCOLOR_ARGB(0xff, 112,155,214 ) );

	AddChild( pBack );
	AddChild( m_pUp );
	
	if( m_pBar ) // 생성된것이라면 추가시킨다. 
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

	_localx = GetX() - pParent->GetX(); // 나의 상대위치 = 나의 위치 - 부모의 절대위치
	_localy = GetY() - pParent->GetY(); // 나의 상대위치 = 나의 위치 - 부모의 절대위치
}

CGUI_ListBox_SCROLL::~CGUI_ListBox_SCROLL()
{
	// 버튼 이미지 소멸?
}

void	CGUI_ListBox_SCROLL::HEAD()
{
	if( m_pBar==NULL) return;
	m_pBar->SetY( m_BoundRect.top  );
}


void	CGUI_ListBox_SCROLL::TAIL()
{
	m_PointPos= 100; // 맨 끝으로 옮김.
	if( m_pBar==NULL) return;
	// 스크롤바 갱신 하라는 메세지 보내야 할듯...

	float point_Height = 0.0f;
	point_Height = m_pBar->GetHeight();
	float y = (m_BoundRect.bottom - point_Height );
	m_pBar->SetY( y );
}

void	CGUI_ListBox_SCROLL::Process()
{
	// 버튼 세가지 에 대해서 컨트롤을 여기서 합니다.
	if( m_pUp )
	if( m_pUp->IsEvent() )
	{
		// 내 부모가 포커스를 가지게 합니다.
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

	// 스크롤 포인트는 위 두버튼에 의하여 일정 값만큼 더해지거나 빼집니다.
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
#define INTERVALPOINT 10 //100분율에서 한번 스크롤할때의 값
// 가상기준값을 가지고 스크롤바의 위치를 정해줍니다. 
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
			가상 기준값을 가진 m_PointPos 는 0~100 까지 담는다.
			위의 기준값으로 스크롤 바와 스크롤 뷰를 비율만큼 이동시키는것이다.
			먼저 뷰를 이동하겠다.
			1.m_PointPos-=10; 임의의 단위만큼 감소 시킴 <0,100>
			if( m_PointPos <0) m_PointPos=0;

			스크롤 바가 있다면 자신의 위치로 환산시킴
			2. m_PointPos : 100 == x : bar_height -> x = (m_PointPos*bar_height) / 100;
			3. 부모에게 메세지 보냄 */

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
	// 마우스 클릭이 들어왔을때 
	// 마우스 무브가 들어왔을때
	// 키보드 위 아래가 들어왔을때 
	// 페이지 업다운도 포함 
	// 홈, 엔드도 포함 

	if( ProcMessage_Child( pMsg ) == true ) 
		return true;

	static POINT ScrollDown={0,};
	switch( pMsg.dwMsg )
	{

	case GUIMSG_BUTTON_EVENT:
		{
			CGUI_Button* pButton =reinterpret_cast<CGUI_Button*>( pMsg.dwParam1);
			if( strcmp(pButton->GetName(),"스크롤_포인트" ) ==0 )
			{
				// 눌렸는지 떼였는지 체크
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

	case GUIMSG_SCROLL_PREV: // 이걸로 메세지가 들어오믄..
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
				// 나의 1차적 부모는 리스트 박스이나 그의 부모가 널이 아니면 
				// 그에게 포커스를 가지게 해야 한다.
			IGUI_Object* pSuperParent = m_pParent->GetParent();
			if( pSuperParent == NULL ) pSuperParent=m_pParent;

			while( pSuperParent->GetParent() != NULL )
				pSuperParent = pSuperParent ->GetParent();
			
				CGUI_MGR::SetGuiFocus( pSuperParent->GetName() ); // 나의 최고 부모가 포커스를 가지게 해야함
					
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
				return false; // 나한테 해당하는 메세지가 아니다 
			}
			// 실제 마우스 위치로 스크롤 바 영역의 100 분율기준 위치를 구하고
			// 그 100분율 기준을 실제 스크롤 바로 환산 적용한다. 이부분은 되어 있으므로 
			// 스크롤바 영역에서의 100분율 위치만 구해줌.
			int mousex = pMsg.dwParam1;
			int mousey = pMsg.dwParam2;
			//				마우스위치 - 옵셋위치 / 바운드렉트 보톰 - 
			int intervaly = m_pBar->GetFirstClick().y;
			float Bar_y = m_pBar->GetY() - m_BoundRect.top;
			float Bar_Height = m_pBar->GetHeight();
			float newy = 0.0f;
			float Enable_Height = GetHeight()-Bar_Height ;
			m_PointPos = Bar_y/Enable_Height*100.0f ;

			int y =int( pMsg.dwParam2 - intervaly ); // 간격을 유지해줘야 합니다.

			if( y < m_BoundRect.top )// 는 같거나 커야 하고 
			{
				y = int(m_BoundRect.top); 
			}
			else if( y+Bar_Height>m_BoundRect.bottom )
			{
				y = m_BoundRect.bottom - Bar_Height;
			}
			
			m_pBar->SetY( float(y) ); //m_BoundRect.top+

			// 포인트를 움직였으므로 
			// 스크롤이 움직인 만큼 영향을 준다. 
			// 포인트가 움직일수 있는 영역은 전체 DESTRECT 에서 위아래 각각 point_Height /2 한 만큼 이다.
			// 그만큼을 이동가능한 영역의 전체로 보고 현재 위치는 전체에서의 상대적인 위치의 2048 분율로 한다.
			// 2의 승수 승이므로 / 연산시 소숫점 5자리 까지는 오차가 생기지 않음 
			// 현재 마우스 위치???? 드레그 시에는 100분율로 역시 이동함... 
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
// List Box 관련 클래스 
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
	SAFE_DELETE( m_pScroll ); // 우측의 스크롤을 담당.
}

void CGUI_ListBox::SetLineCnt( UINT MaxContentsCnt ) // 현재 정해준대로 라인버퍼가 생성됩니다.
{
	// 현재 있던것들은 다 날라가나..?
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

		// delete 하는 루틴이 첨가 되야 할듯.. 
		m_ChildList.erase( m_ChildList.begin()  );
		SAFE_DELETE( pObj );
		siz = (int)m_ChildList.size();
		// 첫번째 이후부터 끝까지 모두 위로 댕겨와야 한다..
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
		// 여기서 스크롤 바를 생성합니다.
		if( m_pScroll == NULL )
		{
			m_pScroll = new CGUI_ListBox_SCROLL(this);
		}
		TAIL(); 
	}
}

void CGUI_ListBox::AddString( TCHAR *szStr, D3DCOLOR OutClr,D3DCOLOR FontClr,bool bFocusString )
{
	// 그 옵셋이 해당의 길이가 될듯...
	// 1.픽셀검사. 제한영역의 크기보다 같거나 크면 그곳이 다음스트링의 1차옵셋 
	// 2. 한글인지 검사하여 1차옵셋을 빼준다. 한글이 안나올때 까정 IsDBCSLeadByte 로 검출하여 FALSE 가 
	// 나올 때까지 다음 옵셋을 계속 빼준다. 
	// 2.5 그 옵셋은 다음 스트링의 시작위치인 동시에 현재 스트링의 길이다.
	// 현재 스트링은 자신의 길이 +1 하여 널문자를 집어 넣는다.
	// 3. 두 검사를 거친 옵셋과 길이가 나오면 
	// 그 옵셋과 길이로써 스트링을 올린다. 
	int StrCnt = 0;
	int StringOffset[64]={0,};
	int Length[64]={0,};
	int szLen =(int) _tcslen( szStr );
	
	SIZE sz = GetStrPixel( CGUI_Edit::GetEDITHWND(), g_D3D.GetGDIFont(), szStr );
	float Width = GetWidth();
	if( sz.cx <= Width ) // 스트링이 하나라면 
	{
		StrCnt= 1;
		Length[0]=szLen;
	}
	else // 여러개 라면 
	{
		int offset=0;
		int TmpLen = 0;//_tcslen( &szStr[offset] );			
		while( 1 ) // 아 졸리 더티해 @,.@;
		{
			TCHAR TmpBuff[256]={0,};
			_tcsncpy( TmpBuff, &szStr[offset], TmpLen );

			sz = GetStrPixel( CGUI_Edit::GetEDITHWND(), g_D3D.GetGDIFont(), TmpBuff );
			if( sz.cx < Width )
			{
				//	한자나 한글이면 2바이트 증가
				//	영문이나 숫자면 
				if( IsDBCSLeadByte( TmpBuff[TmpLen] ) == TRUE ) //szStr
				TmpLen+=2;
				else
				TmpLen+=1;

				if( szLen < offset+TmpLen )
				{
					Length[ StrCnt ] = TmpLen;//szLen - offset-1;// 다음줄의 옵셋이 나오면 그것;
					StrCnt++;
					break;
				}
			}
			else // 한줄이 되어 추가 해야 합니다. 
			{
				if( IsDBCSLeadByte( TmpBuff[TmpLen-1 ] ) == TRUE )// 한글이라면  //szStr
				{
					TmpLen-=2;
				}else
					TmpLen-=1;

				offset += TmpLen;
				Length[ StrCnt ] = TmpLen  ;//TmpLen;// 다음줄의 옵셋이 나오면 그것;
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
		while( siz  > aa ) // ? 위의 것들을 모두 아래로..?
		{
			IGUI_Object* pObj = m_ChildList[0];
			int Height =(int) pObj->GetHeight();
			m_ContentsRect.bottom -= Height;
			m_ChildList.erase( m_ChildList.begin()  );
			SAFE_DELETE( pObj );
			siz =(int) m_ChildList.size();
			// 첫번째 이후부터 끝까지 모두 위로 댕겨와야 한다..
			int Size =(int) m_ChildList.size();
			for( int i=0; i<Size ; i++)
			{
				pObj = m_ChildList[i];
				pObj->MoveGUI( 0, (float)-Height );
				pObj->CalcViewRect( (fRECT*)&m_DestRect );
			}
		}
		
		// 현재의 DestRect 의 width 보다 스트링이 긴지 체크하여 
		// 길다면 그 아랫 라인으로 보냄.
		
		int Size =(int) m_ChildList.size();
		
		IGUI_Object* pTmp = m_ChildList[Size-1];
		float Y =0.0f;
		if( Size != 0 )
			Y = pTmp->GetY() +  pTmp->GetHeight() - m_DestRect.top ; //  m_ContentsRect.bottom;

		CGUI_Text* pText = NULL;
		if( bFocusString == true )
		{
			pText = CGUI_MGR::CreateFocusText( this->GetName(), _T("컨텐츠"), 0, (int) Y , OutClr, FontClr, tmpBuff ); 
		}
		else
		{
			pText = CGUI_MGR::CreateText( this->GetName(), _T("컨텐츠"), 0,(int) Y , OutClr, FontClr,  tmpBuff ); 
		}

		
		this->m_ContentsRect.bottom +=(long) pText->GetHeight();
		pText->CalcViewRect( (fRECT*)&m_DestRect );
		if( m_DestRect.bottom - m_DestRect.top < m_ContentsRect.bottom)
		{
			// 여기서 스크롤 바를 생성합니다.
			if( m_pScroll == NULL )
			{
				m_pScroll = new CGUI_ListBox_SCROLL(this);
			}
			// 여기서 맨 아래로 내려야 합니다.
			TAIL();
		}
	}	
}

// 맨 아래로 내려줍니다.
// 스크롤 포인트를 맨아래로 내린뒤 스크롤 메세지를 부모에게(리스트박스) 보냅니다. 
void CGUI_ListBox::HEAD()
{
	
	int ListSize =(int) m_ChildList.size();
	if( ListSize == 0 ) return;
	IGUI_Object* pObj = m_ChildList[ 0 ];
	float Mn = m_DestRect.top  - pObj->GetY() ; //- pObj->GetHeight()	

	
	// 맨 마지막에 있는 놈의 위치를 현재 보일수 있는 맨 바닥의 위치와의 차를 비교하여 
	// 그 만큼 모두 위로 끌어 당겨 올려버림 
	
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

	// 맨 마지막에 있는 놈의 위치를 현재 보일수 있는 맨 바닥의 위치와의 차를 비교하여 
	// 그 만큼 모두 위로 끌어 당겨 올려버림 

	for( int i=0; i<ListSize ; i++)
	{
		pObj = m_ChildList[i];
		pObj->MoveGUI( 0, -Mn );
		pObj->CalcViewRect( (fRECT*)&m_DestRect );
	}	

	if( m_pScroll )
	m_pScroll->TAIL();

	m_LastScrollPos= 100;
	
	// 포커스에는 마지막것의 영역을 셋팅해줍니다.

}

bool CGUI_ListBox::ProcMessage(GUIMSG& pMsg)
{
	// 나의 영역이외의 메세지라면 차일드에게는 보내지 않아야 하는데... 
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
			// 나를 눌렀으면 포커스 온 
			// 내 영역에 들어오면 포커스 
			int x =int( pMsg.dwParam1 );
			int y =int( pMsg.dwParam2 );
			
			if( m_BoundRect.left <= x  &&
				x <= m_BoundRect.right &&
				m_BoundRect.top  <= y  &&
				y <= m_BoundRect.bottom )
			{
				// 컨텐츠중에 클릭된 것의 영역을 얻어서 
				// 그 부분만큼을 셋팅~ 
	
				if(m_pParent!= NULL)
				{
					IGUI_Object* pSuperParent = m_pParent->GetParent();
					if( pSuperParent == NULL ) pSuperParent=m_pParent;
					
					while( pSuperParent->GetParent() != NULL )
						pSuperParent = pSuperParent ->GetParent();
					
					CGUI_MGR::SetGuiFocus( pSuperParent->GetName() ); // 나의 최고 부모가 포커스를 가지게 해야함
				}else
				{
					CGUI_MGR::SetGuiFocus( GetName() ); // 나의 최고 부모가 포커스를 가지게 해야함
					return true; // 나의 부모가 없으므로 메세지를 아무도 처리할 필요 없다
				}
				
				return false; // 나말고 부모가 처리할수도 있음
			}
		}

		return false;

		case GUIMSG_SCROLL_VERTICAL_UP:
			{}
		return true;

	case GUIMSG_SCROLL_VERTICAL_DRAG:
		{
			// 인자는 최대 100 분율로 넘어옵니다. 
			// 10이면 현재 컨트츠들이 포함된 사각형의 10프로 이므로 그 사각형이 1000이면 100 픽셀 이다.
			// 100 픽셀을 위로 업 해줘야 한다.
			//
			// 나의 대상영역이 컨텐츠들에겐 최종 클립영역이 된다.
			// 1. 이동한 만큼을 구한뒤;
			// 2. 단위이동픽셀 = (안보이는영역 - 보이는영역 )/100;  : (안보이는 영역 > 보이는 영역 ) 
			// 3. 단위이동픽셀을 이동한 퍼센테이지만큼 곱한뒤 빼거나 더해줌.
			// 현재 위치값 - 이전 위치값
			// 99 -100 = -1 이므로 -1 만큼 이동시키면 됨 위로 올린다는 얘기..
			// 그 -1 을 픽셀값으로 변환 시키면?
			// -1 : 100 == x : 364 -> x = ( -1 * 364 / 100) 이므로... 
			int dp = m_LastScrollPos-pMsg.dwParam1  ; // 이전 위치와의 차이를 구합시다. 
			if( dp == 0 )
				return true;

			float ContentsHeight = m_ContentsRect.bottom - ( m_DestRect.bottom - m_DestRect.top ) ;
			if( ContentsHeight <= 0 ) // 안보이는 영역의 크기가 0보다 작으면 낭패
				return true; 

			float dPixel = (ContentsHeight/100.0f) * dp ; 

			int ListSize =(int) m_ChildList.size();
			// 이전에 얼마였는지 현재와의 차이를 구하여
			// 그만큼을 이동시킨다. 
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
	// 텍스쳐의 일부를 보여주는 클리핑을 해야 함.
	

//	Render_Child(); // 이것은 아마 스크롤 박스 쯤이 될듯 
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
	// 스크롤 바도 없에야 한다. 
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



// 현재 선택하고싶은것의 인덱스를 넣어준다
void CGUI_ListBox::SetFocusContentsIndex(int Index ) 
{
	// 현재 선택된 것은 꺼버린다 
	int LastFocus = GetFocusContentsIndex();
	GUIMSG msg;
	msg.dwMsg = GUIMSG_MLDOWN ;
	msg.dwParam1 = 0;
	msg.dwParam2 = 0;
	m_ChildList[ LastFocus ]->ProcMessage( msg );

	RECT rt;
	m_ChildList[ Index ]->GetDestRect( rt );
	// 여기서 선택한것에게 넣어줘야 한다.
	msg.dwParam1 = rt.left+ ((rt.right - rt.left) >>1);
	msg.dwParam2 = rt.top + ((rt.bottom - rt.top )>>1);
	m_ChildList[ Index ]->ProcMessage( msg );
	m_FocusIndex = Index;
}

// 현재 선택된것의 인덱스를 반환한다.
int CGUI_ListBox::GetFocusContentsIndex( void ) 
{
	
	// CGUI_FocusText 일때만 쓸수 있다는 단점이 있는데 설계가 일단 잘못된듯.. 
	// 이렇게 되면 모든 IGUI_x 들은 자신이 포커스를 가졌는지등을 알려줄수 있어야 한다.
	// 모든 것들은 알려줄수는 있지만 곧바로 
	// 현재 내가 포커스를 가졌는지는 의문...
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


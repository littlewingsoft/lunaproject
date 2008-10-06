
#include <guistdafx.h>
#include "Share_Protocol\Network_Define.h"
////////////////////////////////////////////////
//
// 에디트 박스 관련 클래스 
//
////////////////////////////////////////////////
HWND CGUI_Edit::m_hEdit = NULL;
void    CGUI_Edit::SetAttirb( EDIT_ATTRIB attr )
{
	m_Attrib=attr;
	switch( attr )
	{
	case NORMAL:
	case READONLY:
		m_MaxLimit=MAX_LENGTH ;
		break;
	case ID:
		m_MaxLimit=MAX_PLAYER_ID -1; 
		break;
	case PASSWD:
		m_MaxLimit=MAX_PLAYER_PASSWORD - 1;
		break;
	}
}

CGUI_Edit::CGUI_Edit():	
	m_Attrib(NORMAL),
	m_Alpha(0xff),
	m_BlockStartIndex(0),
	m_BlockEndIndex(0)
{
	m_Gui_ClsID =GUI_EDITBOX;
	m_BackClr= D3DCOLOR_ARGB( 0xff, 0x7a, 0x96, 0xdf);
	
	m_BlockRect.left=
		m_BlockRect.bottom=
			m_BlockRect.right=
				m_BlockRect.top=0;

	memset( m_Str,0x00,32*sizeof(TCHAR));
	m_CaretPos.x = 0;
	m_CaretPos.y = 0;
	m_Strlength = 0;
	m_CaretSize.cy =12;
	m_CaretSize.cx = 2;

//	int len = 24;
	SendMessage( m_hEdit, EM_LIMITTEXT, MAX_LENGTH , 0 );
	m_MaxLimit = MAX_LENGTH ;
	SetFocus( CGUI_Edit::m_hEdit );
	m_FontClr = 0xff000000;
	// 원래의 설정을 저장하기 

}

CGUI_Edit::CGUI_Edit(EDIT_ATTRIB attr): // 더티하지만 ㅋㅋ~ 어쩔수 없다 나중에 고치자 

	m_Alpha(0xff),
	m_BlockStartIndex(0),
	m_BlockEndIndex(0)
{
	m_BackClr= D3DCOLOR_ARGB( 0xff, 0x7a, 0x96, 0xdf);
	m_Gui_ClsID = GUI_EDITBOX;
	memset( m_Str,0x00, 32*sizeof(TCHAR) );

	m_BlockRect.left= 
		m_BlockRect.bottom=
			m_BlockRect.right=
				m_BlockRect.top=0;

	m_CaretPos.x = 0; 
	m_CaretPos.y = 0; 
	m_Strlength = 0;
	m_CaretSize.cy =12;
	m_CaretSize.cx = 2;
	m_FontClr = 0xff000000;
	SetAttirb(attr);
	

}

CGUI_Edit::~CGUI_Edit()
{
 // 원래 되있던 설정으로 돌려준다.
}

#define Offsetx 0

// 현재 에디트 박스 내용의 시작 인덱스와 끝 인덱스 사이의 글자의 픽셀 크기를 돌려줌 
SIZE CGUI_Edit::GetStrPixel(int Start, int End)
{
	int len =End - Start;
	SIZE size={0,};
	HDC hdc = GetDC( m_hEdit );
	SelectObject( hdc, (HFONT)g_D3D.GetGDIFont());
	BOOL bl = GetTextExtentPoint32( hdc, &m_Str[Start], len, &size );
	ReleaseDC( CGUI_Edit::m_hEdit,hdc );
	return size;
}

void CGUI_Edit::SetBlock(int Start,int End ) // start =0 , end = -1 이면 스트링모두 블럭 
{
	if( Start == -1 && End == -1 )
	{
		SetRect( &m_BlockRect,0,0,0,0 );
	POINT pt={0,};
	GetCaretPos( &pt );
	SetCaretpos( pt.x, pt.y);
		return;
	}

	if( Start == 0 && End == -1 )
	{
		SendMessage( m_hEdit, EM_SETSEL,0, -1 );
		SendMessage( m_hEdit, EM_GETSEL, (WPARAM)&Start, (LPARAM)&End );
	}

	
	SIZE offset = GetStrPixel( 0, Start );
	SIZE size = GetStrPixel( Start, End );

	m_BlockStartIndex = Start;
	m_BlockEndIndex   = End;

	m_BlockRect.left	= m_BoundRect.left + offset.cx	+	Offsetx;
	m_BlockRect.right	= m_BlockRect.left + size.cx;//+1;

	m_BlockRect.top		= m_BoundRect.top +1;
	m_BlockRect.bottom	= m_BlockRect.top     + size.cy + 2;

	POINT pt={0,};
	GetCaretPos( &pt );
	SetCaretpos( pt.x, pt.y);
}

void CGUI_Edit::SetEndCaret()
{
	SetBlock( m_BlockStartIndex,m_BlockEndIndex);
	SendMessage( m_hEdit,WM_KEYDOWN,VK_END,0);
}

bool CGUI_Edit::ProcMessage(GUIMSG& pMsg)
{
///	if( this->ProcMessage_Child( pMsg ) == true )
//		return true;

	switch( pMsg.dwMsg )
	{
	case GUIMSG_CLOSE:
		{
			CGUI_MGR::NoneFocusSetting();
			DestroyAllChild(); // 자기 자식도 모두 죽이고 자기는 죽음.
			// 밖에서 delete this 안해도 됨 내부 루틴이 글케 되있음 
		}
		return false;

	case GUIMSG_FOCUSLEAVE: 
		// 나에게서 포커스가 해제됨 
		// 포커스를 잃었을땐 
		// 현재의 커서 위치 저장 해줘야 해...
		// 만일 내가 포커스를 잃는다면 원래 대로 해줘야 마땅해..
		if(  m_Attrib == READONLY )
		SendMessage( this->m_hEdit, EM_SETREADONLY,0,0); // it's not readonly No More 리드 온니가 아닙니다.
		return true;

	case GUIMSG_FOCUSENTER:	// 나에게 포커스가 옴 
		{
			SetWindowText(this->m_hEdit,m_Str);
			SetBlock(0,-1); // 첨부터 끝까지
			int len = 0; // 허용 길이 
			switch( m_Attrib )
			{
			case NORMAL:
				break;
			case READONLY:
				SendMessage( this->m_hEdit, EM_SETREADONLY,1,0); // set read only
				break;

			case PASSWD:
			case ID:
				ToggleEngChar( this->m_hEdit );
				break;

			}
			SendMessage( m_hEdit, EM_LIMITTEXT, m_MaxLimit, 0);
			::SetFocus( this->m_hEdit );
			return true;
		}
		return false;

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
				CGUI_MGR::SetGuiFocus( m_Name );
				return true;
			}
//			CGUI_MGR::NoneFocusSetting();
		}

		return false;
	}

	return false;
}

CGUI_Edit::EDIT_ATTRIB CGUI_Edit::GetAttrib(void){	return m_Attrib; }
BYTE CGUI_Edit::GetStrLen(void)
{
	int len = _tcslen( this->m_Str );
	if( len == 0 ){ m_Strlength = 0 ; return 0; }

	int charlen = 0;
	for( int i=0; i< len; i++)
	{
		if( m_Str[i] != ' ' )
			charlen++;
	}

	m_Strlength = charlen;
	return m_Strlength; 
}
TCHAR* const CGUI_Edit::GetString(void){ return m_Str; }
void	CGUI_Edit::SetCaretpos(int x, int y) { m_CaretPos.x = x; m_CaretPos.y = y; }
void	CGUI_Edit::SetCaretSize(SIZE& size){	m_CaretSize=size;}
void	CGUI_Edit::SetString(const TCHAR *  szStr){ _tcscpy(m_Str,szStr ); SetWindowText(m_hEdit,m_Str); }

// 최대 글 숫자.
void	CGUI_Edit::SetLimitStr( DWORD MaxLimit )
{
	MaxLimit %= 128; 
	m_MaxLimit = MaxLimit; 
	SendMessage( m_hEdit, EM_LIMITTEXT, MaxLimit, 0); 
	m_BoundRect.right = m_BoundRect.left +(6*MaxLimit) + Offsetx; 
}

void	CGUI_Edit::SetBackClr( D3DCOLOR Clr ) 
{
	m_BackClr = Clr ; 
}

void	CGUI_Edit::SetStrClr( D3DCOLOR Clr ) 
{
	m_FontClr = Clr ; 
}

void CGUI_Edit::Process(void)
{
	m_Alpha = 255;
}

void CGUI_Edit::RefreshStr(void)
{
	m_Strlength = GetWindowText( m_hEdit, m_Str, m_MaxLimit+1 );
	if( m_Strlength <= 0 ) 
		return;

	HDC hdc = GetDC( CGUI_Edit::m_hEdit );
	SIZE size={0,};
	SelectObject( hdc, (HFONT)g_D3D.GetGDIFont());
	
#ifdef UNICODE	
	GetTextExtentPoint32( hdc, &m_Str[m_Strlength-1], 1,&size);
#else
	if( g_CompState == CECW_DOCOMP || g_CompState == CECW_STARTCOMP )
	{
		GetTextExtentPoint32( hdc, &m_Str[m_Strlength-2], 2,&size);
	}
	else 
	{
		GetTextExtentPoint32( hdc, &m_Str[m_Strlength-1], 1,&size);
	}
#endif

	SetCaretSize( size );
	ReleaseDC( m_hEdit,hdc );
}

void CGUI_Edit::Render(void)
{
	bool	bInactive = false;

	if( CGUI_MGR::GetCurrFocus() != (IGUI_Object*)this ) //현재포커스가 내가 아니면 
	bInactive = false;
	else bInactive = true;

	D3DCOLOR BackClr = m_BackClr;
	// 글뒤의 배경을 보여주기   
	if( bInactive == true )
	BackClr = D3DCOLOR_ARGB( 0xff, 0xff, 0xef, 0xef );
	

	if( GETA( m_BackClr) != 0x00 )
	{
	//SetRect(&srcRt, 0,0,m_Img.sprInfo.nWidth,m_Img.sprInfo.nHeight);
	RECT rt ={ m_BoundRect.left, m_BoundRect.top, m_BoundRect.right, m_BoundRect.bottom };
	g_D3D.BltSprite( m_Layer, m_Img.TexID, NULL, rt,	BackClr,	D3D_ALPHASTATE );
	}


	if( bInactive == true )
	{
	DWORD s=0,e=0;
	SendMessage(m_hEdit,EM_GETSEL, (WPARAM)&s, (LPARAM)&e );
	SetBlock( s,e); // 블럭의 위치를 항상 계산
	}
	D3DCOLOR TmpClr = m_FontClr;
	// 글씨 찍기 ------------------- 

	//  블럭 찍고 커서 찍기 ---------------------------
	if( bInactive == true ) 
	{
		// 한글이고 조합중
		RECT destRt={0,}; //SetRect(&srcRt,0,0,m_Img.sprInfo.nWidth,m_Img.sprInfo.nHeight);
		if( m_BlockStartIndex != m_BlockEndIndex ) // 현재 블럭 그리기 
		{
			g_D3D.BltSprite( m_Layer,m_Img.TexID,NULL, m_BlockRect, D3DCOLOR_ARGB( (BYTE)m_Alpha,0xff,0xba,0xbd), D3D_NORMALSTATE );	//0x80ff0808
		}

		if( g_CompState == CECW_STARTCOMP || g_CompState == CECW_DOCOMP )	// 한글일때 
		{
			//Offsetx++ Offsetx
			SetRect(&destRt,m_BoundRect.left+ m_CaretPos.x+0,		m_BlockRect.top,//m_BoundRect.top+1, 
				            m_BoundRect.left+ m_CaretPos.x+0 + m_CaretSize.cx,	m_BlockRect.top+m_CaretSize.cy +2);
		}
		else if( g_CompState == CECW_ENDCOMP  )	// 영어
		{
			SetRect(&destRt,m_BoundRect.left+ m_CaretPos.x+Offsetx,		m_BlockRect.top, 
				m_BoundRect.left+ m_CaretPos.x+1+Offsetx,				m_BlockRect.top+m_CaretSize.cy+2 );
		}

		g_D3D.BltSprite( m_Layer,m_Img.TexID,NULL, destRt, D3DCOLOR_ARGB(0xff,122,150,223), D3D_ALPHASTATE );
	}

	
	int x=m_BoundRect.left+Offsetx ,y =m_BoundRect.top+ ( ((m_BoundRect.bottom - m_BoundRect.top) / 2) - 6); // 바운드의 세로 너비 -2
	if( m_Attrib == PASSWD ) // 암호 칠때 '*' 표시 나게 합니다. 
	{
		int len = _tcsclen( m_Str ) ;
		if( len != 0 )
		{
		TCHAR* str=(TCHAR*)malloc( (len +1)* sizeof(TCHAR) );
		for( int i=0; i< len; i++)
		{
			static TCHAR aa[] = { _T("P*A*S*S*W*O*R*D*") };
			str[i]= aa[i];
		}
		str[len]= '\0';
		g_D3D.BltText( m_Layer, x, y, TmpClr, str );
		free( str );
		}
	}
	else
	{
		g_D3D.BltText( m_Layer, x,y, TmpClr, m_Str );
	}

}

void CGUI_Edit::ClearString(void)// 글자를 모두 지우고 커서를 원위치한다.
{
	SetWindowText(this->m_hEdit,"");
	SetBlock(0,-1); // 첨부터 끝까지
	RefreshStr();
}

void CGUI_Edit::SetHEDIT( HWND hEdit )
{
	m_hEdit = hEdit;	
}

HWND CGUI_Edit::GetEDITHWND(void)
{
	return m_hEdit;
}


#include <guistdafx.h>
#include "Share_Protocol\Network_Define.h"
////////////////////////////////////////////////
//
// ����Ʈ �ڽ� ���� Ŭ���� 
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
	// ������ ������ �����ϱ� 

}

CGUI_Edit::CGUI_Edit(EDIT_ATTRIB attr): // ��Ƽ������ ����~ ��¿�� ���� ���߿� ��ġ�� 

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
 // ���� ���ִ� �������� �����ش�.
}

#define Offsetx 0

// ���� ����Ʈ �ڽ� ������ ���� �ε����� �� �ε��� ������ ������ �ȼ� ũ�⸦ ������ 
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

void CGUI_Edit::SetBlock(int Start,int End ) // start =0 , end = -1 �̸� ��Ʈ����� �� 
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
			DestroyAllChild(); // �ڱ� �ڽĵ� ��� ���̰� �ڱ�� ����.
			// �ۿ��� delete this ���ص� �� ���� ��ƾ�� ���� ������ 
		}
		return false;

	case GUIMSG_FOCUSLEAVE: 
		// �����Լ� ��Ŀ���� ������ 
		// ��Ŀ���� �Ҿ����� 
		// ������ Ŀ�� ��ġ ���� ����� ��...
		// ���� ���� ��Ŀ���� �Ҵ´ٸ� ���� ��� ����� ������..
		if(  m_Attrib == READONLY )
		SendMessage( this->m_hEdit, EM_SETREADONLY,0,0); // it's not readonly No More ���� �´ϰ� �ƴմϴ�.
		return true;

	case GUIMSG_FOCUSENTER:	// ������ ��Ŀ���� �� 
		{
			SetWindowText(this->m_hEdit,m_Str);
			SetBlock(0,-1); // ÷���� ������
			int len = 0; // ��� ���� 
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
			// ���� �������� ��Ŀ�� �� 
			// �� ������ ������ ��Ŀ�� 
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

// �ִ� �� ����.
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

	if( CGUI_MGR::GetCurrFocus() != (IGUI_Object*)this ) //������Ŀ���� ���� �ƴϸ� 
	bInactive = false;
	else bInactive = true;

	D3DCOLOR BackClr = m_BackClr;
	// �۵��� ����� �����ֱ�   
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
	SetBlock( s,e); // ���� ��ġ�� �׻� ���
	}
	D3DCOLOR TmpClr = m_FontClr;
	// �۾� ��� ------------------- 

	//  �� ��� Ŀ�� ��� ---------------------------
	if( bInactive == true ) 
	{
		// �ѱ��̰� ������
		RECT destRt={0,}; //SetRect(&srcRt,0,0,m_Img.sprInfo.nWidth,m_Img.sprInfo.nHeight);
		if( m_BlockStartIndex != m_BlockEndIndex ) // ���� �� �׸��� 
		{
			g_D3D.BltSprite( m_Layer,m_Img.TexID,NULL, m_BlockRect, D3DCOLOR_ARGB( (BYTE)m_Alpha,0xff,0xba,0xbd), D3D_NORMALSTATE );	//0x80ff0808
		}

		if( g_CompState == CECW_STARTCOMP || g_CompState == CECW_DOCOMP )	// �ѱ��϶� 
		{
			//Offsetx++ Offsetx
			SetRect(&destRt,m_BoundRect.left+ m_CaretPos.x+0,		m_BlockRect.top,//m_BoundRect.top+1, 
				            m_BoundRect.left+ m_CaretPos.x+0 + m_CaretSize.cx,	m_BlockRect.top+m_CaretSize.cy +2);
		}
		else if( g_CompState == CECW_ENDCOMP  )	// ����
		{
			SetRect(&destRt,m_BoundRect.left+ m_CaretPos.x+Offsetx,		m_BlockRect.top, 
				m_BoundRect.left+ m_CaretPos.x+1+Offsetx,				m_BlockRect.top+m_CaretSize.cy+2 );
		}

		g_D3D.BltSprite( m_Layer,m_Img.TexID,NULL, destRt, D3DCOLOR_ARGB(0xff,122,150,223), D3D_ALPHASTATE );
	}

	
	int x=m_BoundRect.left+Offsetx ,y =m_BoundRect.top+ ( ((m_BoundRect.bottom - m_BoundRect.top) / 2) - 6); // �ٿ���� ���� �ʺ� -2
	if( m_Attrib == PASSWD ) // ��ȣ ĥ�� '*' ǥ�� ���� �մϴ�. 
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

void CGUI_Edit::ClearString(void)// ���ڸ� ��� ����� Ŀ���� ����ġ�Ѵ�.
{
	SetWindowText(this->m_hEdit,"");
	SetBlock(0,-1); // ÷���� ������
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

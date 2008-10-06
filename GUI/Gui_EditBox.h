

#pragma once

#include "IGui_Object.h"

class CGUI_Edit : public IGUI_Object
{
public:
//										허용모드		글제한   수정 
	enum EDIT_ATTRIB{	NORMAL,     //    한/영			24		 가능 
						READONLY,   //    한/영			24		 불가능 
						ID,			//		영			10		 가능 
						PASSWD };	//		영			8        가능 

	enum {MAX_LENGTH = 64};

protected:
	EDIT_ATTRIB m_Attrib;	
	POINT		m_CaretPos;
	SIZE		m_CaretSize;
	int			m_BlockStartIndex;
	int			m_BlockEndIndex;
	RECT		m_BlockRect;
	BYTE		m_Alpha;
	DWORD		m_FontClr,m_BackClr;
	BYTE		m_Strlength;
	TCHAR		m_Str[MAX_LENGTH]; // 최대 64 글자입니다.
	DWORD		m_MaxLimit;
	static HWND	m_hEdit;
public:
	DECL_CREATE(CGUI_Edit);

	static HWND GetEDITHWND(void);
	static void SetHEDIT(HWND);
	operator HWND()const { return m_hEdit; }

	
	EDIT_ATTRIB GetAttrib(void);

	void	SetBlock(int Start,int End ); // start =0 , end = -1 이면 스트링모두 블럭 
	void	SetEndCaret();// 캐럿을 끝으로 보냄
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);

	void	RefreshStr(void);
	BYTE	GetStrLen(void);
	TCHAR* const GetString(void);
	SIZE	GetStrPixel(int Start, int End); // 스트링의 실제 픽셀 사이즈를 반환한다. 
	void	SetCaretpos(int x, int y);
	void	SetCaretSize(SIZE& size);
	void	SetString(const TCHAR *  szStr);
	void	SetLimitStr( DWORD MaxLimit );// 최대 글 숫자.
	void	SetBackClr( DWORD Clr );
	void	SetStrClr( DWORD Clr );
	void	ClearString(void);// 글자를 모두 지우고 커서를 원위치한다.
	void    SetAttirb(const EDIT_ATTRIB attr );
	CGUI_Edit();
	CGUI_Edit(EDIT_ATTRIB attr);
	virtual ~CGUI_Edit();
};


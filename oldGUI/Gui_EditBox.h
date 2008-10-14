

#pragma once

#include "IGui_Object.h"

class CGUI_Edit : public IGUI_Object
{
public:
//										�����		������   ���� 
	enum EDIT_ATTRIB{	NORMAL,     //    ��/��			24		 ���� 
						READONLY,   //    ��/��			24		 �Ұ��� 
						ID,			//		��			10		 ���� 
						PASSWD };	//		��			8        ���� 

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
	TCHAR		m_Str[MAX_LENGTH]; // �ִ� 64 �����Դϴ�.
	DWORD		m_MaxLimit;
	static HWND	m_hEdit;
public:
	DECL_CREATE(CGUI_Edit);

	static HWND GetEDITHWND(void);
	static void SetHEDIT(HWND);
	operator HWND()const { return m_hEdit; }

	
	EDIT_ATTRIB GetAttrib(void);

	void	SetBlock(int Start,int End ); // start =0 , end = -1 �̸� ��Ʈ����� �� 
	void	SetEndCaret();// ĳ���� ������ ����
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);

	void	RefreshStr(void);
	BYTE	GetStrLen(void);
	TCHAR* const GetString(void);
	SIZE	GetStrPixel(int Start, int End); // ��Ʈ���� ���� �ȼ� ����� ��ȯ�Ѵ�. 
	void	SetCaretpos(int x, int y);
	void	SetCaretSize(SIZE& size);
	void	SetString(const TCHAR *  szStr);
	void	SetLimitStr( DWORD MaxLimit );// �ִ� �� ����.
	void	SetBackClr( DWORD Clr );
	void	SetStrClr( DWORD Clr );
	void	ClearString(void);// ���ڸ� ��� ����� Ŀ���� ����ġ�Ѵ�.
	void    SetAttirb(const EDIT_ATTRIB attr );
	CGUI_Edit();
	CGUI_Edit(EDIT_ATTRIB attr);
	virtual ~CGUI_Edit();
};


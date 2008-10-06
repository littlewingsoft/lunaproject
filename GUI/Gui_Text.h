
#pragma once

#include "IGui_Object.h"

///////////////////////////////////////////////////////
class CGUI_Text : public IGUI_Object
{
protected:
	size_t		m_Strlen;
	TCHAR*		m_Str;
	DWORD		m_FontClr,m_OutLineClr;

public:
	DECL_CREATE(CGUI_Text)

//	static		DWORD TEXTCNT;
	CGUI_Text();
	CGUI_Text(int x,int y,DWORD		OutClr, DWORD		FontClr,const TCHAR* Str );

	virtual ~CGUI_Text();
	void	SetFontClr(DWORD		 Clr);
	void	SetOutLineClr(DWORD		 Clr);
	void	SetString(const TCHAR* Str);
	TCHAR*	GetString(void) const { return (TCHAR*)m_Str; } 
	void	MakeStr2Texture( int x, int y, const TCHAR* Str );
	
	virtual void	Process();
	virtual void	Render();
	virtual bool	ProcMessage(GUIMSG& pMsg);
};

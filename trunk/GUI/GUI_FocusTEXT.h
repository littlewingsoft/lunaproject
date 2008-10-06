

// GUI_FocusTEXT.h: interface for the CGUI_FocusTEXT class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Gui_Text.h"

class CGUI_FocusText : public CGUI_Text  
{
	RES_ANI	m_Back;
	enum eSTATE{Off,On};
	eSTATE m_State;
public:
	DECL_CREATE( CGUI_FocusText );

	bool IsFocusOn();

	CGUI_FocusText();
	CGUI_FocusText(int x,int y,DWORD		FontClr,DWORD		OutClr,const TCHAR* Str );

	virtual ~CGUI_FocusText();

	virtual void	Render();
	virtual bool	ProcMessage(GUIMSG& pMsg);
	
};


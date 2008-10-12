
#pragma once

#include "IGui_Object.h"

namespace gui
{
///////////////////////////////////////////////////////
class CGUI_Ani : public IGUI_Object
{
	DWORD	m_LastTime;
	int		m_CurrentFrame;
	bool	m_bAni;
public:

	CGUI_Ani();
	CGUI_Ani(int x, int y, TCHAR* fName );

	DECL_CREATE(CGUI_Ani);

	virtual ~CGUI_Ani(){}
	void	SetAni(bool bAni = false);
	void	SetFrame( int Frame =0 );
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);	

};

}
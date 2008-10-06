#include "guistdafx.h"
#include ".\gui_avatar.h"

CGUI_Avatar::CGUI_Avatar(void)
{
	m_Gui_ClsID = GUI_AVATAR;
}

CGUI_Avatar::~CGUI_Avatar(void)
{
}


void	CGUI_Avatar::Process()
{
	
}

void	CGUI_Avatar::Render()
{
	_AvatarCore.Draw( g_D3D.GetTexture(_WorkTextureID) );
	RECT dest={m_BoundRect.left,m_BoundRect.top,m_BoundRect.right,m_BoundRect.bottom};
	g_D3D.BltSprite( this->m_Layer,  _WorkTextureID, &m_SrcRect, dest);
}

bool	CGUI_Avatar::ProcMessage(GUIMSG& pMsg)
{
	switch( pMsg.dwMsg )
	{
		case GUIMSG_CREATED:
			g_D3D.CreateBlankTexture(_WorkTextureID,130,160);
		return true;

		case GUIMSG_DESTROY:
			g_D3D.ReleaseTexture( _WorkTextureID );
			_AvatarCore.Reset();
		return true;
	}

	return false;
}

void CGUI_Avatar::OpenUrl(const char* Url,BYTE Sex )
{
	_AvatarCore._Sex = Sex;
	_AvatarCore.OpenURL(Url);
}
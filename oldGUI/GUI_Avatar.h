#pragma once
#include "igui_object.h"

#include "AvatarCore.h"

class CGUI_Avatar :	public IGUI_Object
{

	int _WorkTextureID;
	AVATARCORE::AvatarCore _AvatarCore;
public:	

	DECL_CREATE(CGUI_Avatar);

	int GetWorkTextureID(){ return _WorkTextureID; }
	AVATARCORE::AvatarCore& GetAvatarCore(void){ return _AvatarCore; }
	CGUI_Avatar(void);
	virtual ~CGUI_Avatar(void);
	void OpenUrl(const char* Url,BYTE Sex );

	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);

};

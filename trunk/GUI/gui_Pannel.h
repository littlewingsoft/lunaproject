
#pragma once

#include "IGui_Object.h"
#include "GUI_AVATAR.h"
#include "GUI_EditBox.h"
#include "../etc/Util.h"
class CGUI_Pannel : public IGUI_Object
{
public:
	DECL_CREATE(CGUI_Pannel);

	CGUI_Pannel();
	virtual ~CGUI_Pannel();

	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
	void	SetLastModal( IGUI_Object* pModal );
protected:
	POINT m_ClickPos;
	string	_DefaultButton;
};

//////////////////////////////////////////////////
class CGUI_ContextMenu : public CGUI_Pannel
{
	CGUI_Button* _pViewPersonInfo;
	CGUI_Button* _pWhisper;
	CGUI_Button* _pKick;
	CGUI_Button* _pBlock;
public:
		DECL_CREATE( CGUI_ContextMenu );
	CGUI_ContextMenu();
	virtual ~CGUI_ContextMenu();
	void	Enable_ViewPersonInfo(){if(_pViewPersonInfo)_pViewPersonInfo->Active(); }
	void	Enable_Whisper(){if(_pWhisper)_pWhisper->Active(); }
	void	Enable_Kick(){if(_pKick)_pKick->Active(); }
	void	Enable_Block(){if(_pBlock)_pBlock->Active(); }

	void	Disable_ViewPersonInfo(){if(_pViewPersonInfo)_pViewPersonInfo->InActive(); }
	void	Disable_Whisper(){if(_pWhisper)_pWhisper->InActive(); }
	void	Disable_Kick(){if(_pKick)_pKick->InActive(); }
	void	Disable_Block(){if(_pBlock)_pBlock->InActive(); }

	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
	
};

//////////////////////////////////////////////////

struct PLAYERINFO;
class CGUI_ShowInfo: public CGUI_Pannel
{
	CGUI_Avatar*	_pAvatar ;
	CGUI_Text*		_pID ;
	CGUI_Text*		_pNick ;
	CGUI_Text*		_pLevel ;
	CGUI_Text*		_pMoney ;
	CGUI_Text*		_pWin ;
	CGUI_Button*	_pClose;
public:
		DECL_CREATE( CGUI_ShowInfo );
	CGUI_ShowInfo();
	virtual ~CGUI_ShowInfo();

	void	SetInfo(PLAYERINFO* Info);
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
	
};

//////////////////////////////////////////////////
// Name: CGUI_Console
// Desc: �⺻���� ������ �����ϴ� �ܼ�â
//////////////////////////////////////////////////
#include <string>
#include <GUI_ListBox.h>
class CGUI_Console : public CGUI_Pannel
{
	CGUI_ListBox*	_pList;
	CGUI_Button*	_pClose;

public:
		DECL_CREATE( CGUI_Console );
	CGUI_Console();
	virtual ~CGUI_Console();

	void	AddString(const TCHAR* szStr );
	void	SetCommand(ICommand* pCommand);
//	void	Process();
//	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
};
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Name: CGUI_RadioGroup
// Desc: ȭ��� ������������ üũ��ư���� ������ 
//       ���⿡ ����ϸ� ��� ��ư�� ��� üũ�Ǵ���
//       �� ��Ʈ���� ��� ������ 
//////////////////////////////////////////////////
class CGUI_RadioGroup:public IGUI_Object
{
public:
	DECL_CREATE( CGUI_RadioGroup );
	CGUI_RadioGroup();
	virtual ~CGUI_RadioGroup();
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
};

//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Name: CGUI_MakeRoom
// Desc: �游���â
//////////////////////////////////////////////////

class CGUI_MakeRoom: public CGUI_Pannel
{
	CGUI_Edit*		_Edit_RoomName;
	CGUI_Edit*		_Edit_Pass;
//	CGUI_RadioButton* _pPublic
	CGUI_Button*	_pOk;
	CGUI_Button*	_pCancel;
//	CGUI_Slide*		_pSlide;

	std::string _RoomName;
	std::string _Pass;
	int			_PlayerCnt;//��� �ο� �ּ� 2~6�����
	

public:
	DECL_CREATE( CGUI_MakeRoom);
	CGUI_MakeRoom();
	virtual ~CGUI_MakeRoom();

	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
};
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// Name: CGUI_Option
// Desc: �ɼ� â
//////////////////////////////////////////////////

class CGUI_Option: public CGUI_Pannel
{
	struct ovalue
	{
		ovalue(){}
		char _bFullMode;
		char _bWhisper; // 1 �Ӹ���� 0: �Ӹ� �Ұ�
		char _bInvite; // 1  �ʴ���� 0: �ʴ�Ұ�

		BYTE		_BgmVolume;// ����� ���� 0~100 
		BYTE		_EffVolume;// ȿ���� ���� 0~100
	};
	
	ovalue _RestoreValue; // �ٲ�� ������
	ovalue _CurrentValue; // ��� �ٲ�°�

public:
	DECL_CREATE( CGUI_Option);
	CGUI_Option();
	virtual ~CGUI_Option();

	void SetValue_BGMVolume(int vol);
	void SetValue_EFFVolume(int vol);
	void SetValue_FullMode(bool bFull);
	void SetValue_BlockWhisper(bool bWhisper);
	void SetValue_BlockInvite(bool bInvite );
	
	const ovalue& GetValue (void){return _CurrentValue; }
	void	RestoreValue(void);
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
};
//////////////////////////////////////////////////
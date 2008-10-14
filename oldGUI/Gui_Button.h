
#pragma once
#include "Igui_Object.h"

//class ICommand;
//기본적인 버튼 /////////////////////////////////////////////////////
class CGUI_Button :public IGUI_Object
{
public:
	enum STATE{ NORMAL=0,FOCUSON=1,DOWN=2,INACTIVE=3	};// 버튼의 상태는 모두 4가지 
protected:
//	ICommand*	m_pCommand;
	STATE		m_State,m_LastState; 
	POINT		m_FirstClick; // 버튼기준 좌표로 클릭된 위치를 기억함
//	eOM			m_Event;
public:
	DECL_CREATE(CGUI_Button);
	CGUI_Button();
	virtual ~CGUI_Button();
	
//	void	SetCommand(ICommand* pCommand );
//	void	_SetEvent(eOM Event ){ m_Event = Event;}
//	eOM		_GetEvent(void)const {return m_Event;}
	POINT   GetFirstClick()const {return m_FirstClick;}
	bool	IsEvent();
	void	DoEvent(); // 셋팅되어있는 이벤트를 발동해브러 
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
	STATE	GetState()const {return m_State; }
	void	InActive();// 비활성화
	void	Active();  // 활성화
};

//////////////////////////////////////////////////////////////////
// Name: 체크 버튼 
// Desc: 마우스 업 메세지가 오면 현재상태가 전이됨
//////////////////////////////////////////////////////////////////
class CGUI_CheckButton :public CGUI_Button
{
protected:
	bool _bChecked;
	int _Alpha;
	int _AlphaAdd;
#ifdef _DEBUG
	RES_ANI _BackImg;
#endif
public:
	virtual void SetCheck(bool bCheck);
	DECL_CREATE(CGUI_CheckButton);
	CGUI_CheckButton();
	virtual ~CGUI_CheckButton();
	const bool IsChecked(void)const { return _bChecked; }
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
};

//////////////////////////////////////////////////////////////////
// Name: 라디오 버튼 
// Desc: 그룹컨트롤에 등록해놓고선 자신이 클릭되었을때 자신의 그룹의
// 모든 멤버들을 언체킹 시킨다. 
//////////////////////////////////////////////////////////////////
class CGUI_RadioButton :public CGUI_CheckButton
{
	// 자신이 속한 라디오 그룹명
	string _GroupName;

	// 모든 라디오 버튼은 무조건 여기에 이름을 등록해브러 
	// Created 시 등록 Destroy 시 삭제 
	static vector<string> _RadioNameList; // 
public:
	void	SetGroupName(const char* GroupName){  _GroupName= GroupName; }
	const char* GetGroupName(){ return _GroupName.c_str(); }
	virtual void SetCheck(bool bCheck);
	DECL_CREATE(CGUI_RadioButton);
	CGUI_RadioButton();
	virtual ~CGUI_RadioButton();
	bool	ProcMessage(GUIMSG& pMsg);

};
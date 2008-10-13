
#pragma once
#include "Igui_Object.h"

//class ICommand;
//�⺻���� ��ư /////////////////////////////////////////////////////
class CGUI_Button :public IGUI_Object
{
public:
	enum STATE{ NORMAL=0,FOCUSON=1,DOWN=2,INACTIVE=3	};// ��ư�� ���´� ��� 4���� 
protected:
//	ICommand*	m_pCommand;
	STATE		m_State,m_LastState; 
	POINT		m_FirstClick; // ��ư���� ��ǥ�� Ŭ���� ��ġ�� �����
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
	void	DoEvent(); // ���õǾ��ִ� �̺�Ʈ�� �ߵ��غ귯 
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
	STATE	GetState()const {return m_State; }
	void	InActive();// ��Ȱ��ȭ
	void	Active();  // Ȱ��ȭ
};

//////////////////////////////////////////////////////////////////
// Name: üũ ��ư 
// Desc: ���콺 �� �޼����� ���� ������°� ���̵�
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
// Name: ���� ��ư 
// Desc: �׷���Ʈ�ѿ� ����س��� �ڽ��� Ŭ���Ǿ����� �ڽ��� �׷���
// ��� ������� ��üŷ ��Ų��. 
//////////////////////////////////////////////////////////////////
class CGUI_RadioButton :public CGUI_CheckButton
{
	// �ڽ��� ���� ���� �׷��
	string _GroupName;

	// ��� ���� ��ư�� ������ ���⿡ �̸��� ����غ귯 
	// Created �� ��� Destroy �� ���� 
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
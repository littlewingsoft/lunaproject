#pragma once
#include "IGUI_Object.h"

/*
	���ڷ��̼� ������ ���ҷμ�
	�ܵ����δ� ������ ������
	�⺻ GUI ��Ʈ�ѵ� ���� ���� ������ ������ �ϰ� ���ŵȴ�. 

	1. ���� ������ ��ġ�� ���� ��ġ �̹Ƿ�( �θ��� ����� ��ġ�� �ƴϴ�)
	2. ���� ��ġ�� �������� �θ� ��ġ ��Ų��, �ӵ���  pixel/msec  �̵����� �������� �Ѵ�.. �̵��� �ð���� �̵�
	3. �ʱ�, ��� ��ġ, ���� �ӵ��� �̵��Ұ��ΰ�.
	4. �����ġ�� �̵��Ŀ��� 



*/
/*
          (nSec)  (nSec)  (nSec)   
     ����   ->    ������    ->    ����
*/
#include <vector>
class GUI_Subclass_FadeInOut :	public IGUI_Object
{
	typedef void (GUI_Subclass_FadeInOut::*PFUNC)(); // ���������� �˻��ϸ� �մ��ϸ� �ش� �Լ��� 
	PFUNC	_pFunc; // 
	struct FUNCTEMPLATE
	{
		static DWORD _BeginTime;
		PFUNC	_pFunc; // 
		int dt;
		DWORD DelayTime; // �̽ð��� ������ ���� ������ ��ȯ���� �ּ���.
	};


	std::vector<FUNCTEMPLATE> _StateList;
	// ���鿡 ��Ÿ����(������ȭ), 
	enum FADE{fIN,	fNORMAL, fOUT};
	FADE  _FadeState;
public:
	DECL_CREATE( GUI_Subclass_FadeInOut );

	GUI_Subclass_FadeInOut(void);
	~GUI_Subclass_FadeInOut(void);

	void	FadeInProcess(void);
	void	FadeWaitProcess(void);
	void    FadeOutProcess(void);
	
	//������ �ð����� 255-0���� ���� �ܰ踦 ����ϴ°�
	void	Fade(int add); // add ����� ������ ������� ������ ���� ������� �ڱ� �θ� ��Ʈ��
	void	UDie();
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
	
};

/////////////////////////////////////////////////////////////////////

class GUI_Subclass_ZOOM_AlphaBlend_Score:	public IGUI_Object
{
	float	_Zoom;
	int		_A;
	char	_szBuff[32];
	int		_Len;
public:
	DECL_CREATE( GUI_Subclass_ZOOM_AlphaBlend_Score );

	GUI_Subclass_ZOOM_AlphaBlend_Score(){ _Zoom=1.0f; _A=255;_Len=0; }
	~GUI_Subclass_ZOOM_AlphaBlend_Score(void);

	//������ �ð����� 255-0���� ���� �ܰ踦 ����ϴ°�
	void	SetNumber(int Num);
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
};

/////////////////////////////////////////////////////////////////////
// �ܵ����� ������ ����.
// �θ� (ex.Ani ���� ��ġ�� �߷� ó���Ͽ� ��ó�� �ٿ ��Ŵ 
//class GUI_Subclass_MoveBounce:	public IGUI_Object
//{
//	float	_Vel; // ���ӵ� 
//	RECT	_Bound;// �� �ڽ��� �ε�ġ�� ��,��,�� 3���� �ϳ��� �ε�ġ�� ���ݾ� �ݹ� ����� �پ���. 
//public:
//	DECL_CREATE( GUI_Subclass_MoveBounce );
//
//	GUI_Subclass_MoveBounce(){ _Vel=0.0f; }
//	~GUI_Subclass_MoveBounce(void);
//
//	//������ �ð����� 255-0���� ���� �ܰ踦 ����ϴ°�
//	void	SetBound(RECT& bound){ _Bound=  bound; }
//	void	Process();
//	void	Render();
//	bool	ProcMessage(GUIMSG& pMsg);
//};
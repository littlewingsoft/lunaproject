#pragma once
#include "igui_object.h"
#include "GUI_Button.h"
class CGUI_Slide :	public CGUI_Button
{
public:
	enum SLIDESTYLE { HORIZON,  /* ����*/
					  VERTICAL, /* ����*/ };
private:
	WORD _AllowRegion_Start;
	WORD _AllowRegion_End;
	static	CGUI_Slide* _pCurrentDownSlide;
	SLIDESTYLE _Style;
	int	_Rate; // 0~100 ������ ������
public:
	
	CGUI_Slide(void);
	~CGUI_Slide(void);
	DECL_CREATE( CGUI_Slide );
	// �θ�Ŭ����Ÿ���� �⺻ ��ư�� 
	// ���� ������ �ּ�,�ִ��ư�� ��������������� �̴� �� �����̵��� �ڽ��� �ɼ��ִ�.
	// �׳� �����̵� ��ư �ϳ��� ������
	// �����̵� �̵����� ������ �����Ҽ�������
	// �ּ� ����
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
	void	SetRate(int rate); // 0~100 ������ ������ ������ ������ �´� ��ġ�� �̵��� 
	int		GetRate();
	void SetStyle(SLIDESTYLE style ){ _Style = style; } 
	void SetAllowRegion(WORD start, WORD end ){ _AllowRegion_Start = start; _AllowRegion_End = end;  }
};



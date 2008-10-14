#pragma once
#include "igui_object.h"
#include "GUI_Button.h"
class CGUI_Slide :	public CGUI_Button
{
public:
	enum SLIDESTYLE { HORIZON,  /* 수평*/
					  VERTICAL, /* 수직*/ };
private:
	WORD _AllowRegion_Start;
	WORD _AllowRegion_End;
	static	CGUI_Slide* _pCurrentDownSlide;
	SLIDESTYLE _Style;
	int	_Rate; // 0~100 까지의 비율값
public:
	
	CGUI_Slide(void);
	~CGUI_Slide(void);
	DECL_CREATE( CGUI_Slide );
	// 부모클래스타입은 기본 버튼이 
	// 보통 구성은 최소,최대버튼이 따라붙을수있으며 이는 곧 슬라이드의 자식이 될수있다.
	// 그냥 슬라이드 버튼 하나만 있으며
	// 슬라이드 이동가능 영역을 정의할수있으며
	// 최소 영역
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
	void	SetRate(int rate); // 0~100 까지의 비율을 넣으면 비율에 맞는 위치로 이동함 
	int		GetRate();
	void SetStyle(SLIDESTYLE style ){ _Style = style; } 
	void SetAllowRegion(WORD start, WORD end ){ _AllowRegion_Start = start; _AllowRegion_End = end;  }
};



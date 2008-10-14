#pragma once
#include "IGUI_Object.h"

/*
	데코레이션 페턴의 역할로서
	단독으로는 사용되지 않으며
	기본 GUI 콘트롤등 에서 투명도 제어의 역할을 하곤 제거된다. 

	1. 나의 지정된 위치는 절대 위치 이므로( 부모의 상대적 위치가 아니다)
	2. 나의 위치를 기준으로 부모를 위치 시킨뒤, 속도는  pixel/msec  이동값을 기준으로 한다.. 이동은 시간기반 이동
	3. 초기, 대상 위치, 얼마의 속도로 이동할것인가.
	4. 대상위치로 이동후에는 



*/
/*
          (nSec)  (nSec)  (nSec)   
     투명   ->    불투명    ->    투명
*/
#include <vector>
class GUI_Subclass_FadeInOut :	public IGUI_Object
{
	typedef void (GUI_Subclass_FadeInOut::*PFUNC)(); // 종료조건을 검사하며 합당하면 해당 함수를 
	PFUNC	_pFunc; // 
	struct FUNCTEMPLATE
	{
		static DWORD _BeginTime;
		PFUNC	_pFunc; // 
		int dt;
		DWORD DelayTime; // 이시간이 지나면 다음 것으로 변환시켜 주세요.
	};


	std::vector<FUNCTEMPLATE> _StateList;
	// 전면에 나타나고(불투명화), 
	enum FADE{fIN,	fNORMAL, fOUT};
	FADE  _FadeState;
public:
	DECL_CREATE( GUI_Subclass_FadeInOut );

	GUI_Subclass_FadeInOut(void);
	~GUI_Subclass_FadeInOut(void);

	void	FadeInProcess(void);
	void	FadeWaitProcess(void);
	void    FadeOutProcess(void);
	
	//관건은 시간동안 255-0으로 가는 단계를 계산하는것
	void	Fade(int add); // add 양수면 불투명에 가까워짐 음수면 투명에 가까워짐 자기 부모를 컨트롤
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

	//관건은 시간동안 255-0으로 가는 단계를 계산하는것
	void	SetNumber(int Num);
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
};

/////////////////////////////////////////////////////////////////////
// 단독으론 쓰이지 못함.
// 부모 (ex.Ani 등의 위치를 중력 처리하여 공처럼 바운스 시킴 
//class GUI_Subclass_MoveBounce:	public IGUI_Object
//{
//	float	_Vel; // 가속도 
//	RECT	_Bound;// 이 박스에 부딪치면 좌,우,하 3면중 하나에 부딪치면 조금씩 반발 계수가 줄어든다. 
//public:
//	DECL_CREATE( GUI_Subclass_MoveBounce );
//
//	GUI_Subclass_MoveBounce(){ _Vel=0.0f; }
//	~GUI_Subclass_MoveBounce(void);
//
//	//관건은 시간동안 255-0으로 가는 단계를 계산하는것
//	void	SetBound(RECT& bound){ _Bound=  bound; }
//	void	Process();
//	void	Render();
//	bool	ProcMessage(GUIMSG& pMsg);
//};
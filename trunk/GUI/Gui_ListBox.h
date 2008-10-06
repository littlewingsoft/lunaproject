
#pragma once
#include "IGui_Object.h"
#include "Gui_Ani.h"
#include "Gui_Button.h"
#include <string>
////////////////////////////////////////
// 스크롤을 담당해주는 기본적으로 우측에 달리는 상,바,하 의 버튼역활
class CGUI_ListBox;
class CGUI_ListBox_SCROLL : public IGUI_Object
{
	CGUI_Ani*		m_pBack;	// 한개를 로드해서 횟수만큼 포문돌아서 찍어요.
	CGUI_Button*	m_pUp;			// 클릭이벤트 발생시 가상 기준값을 제어함
	CGUI_Button*	m_pBar;		// 가상 기준값에 따라 현재 위치를 정하기도 하고 가상기준값을 변경하기도함. 
	CGUI_Button*	m_pDn;			// 클릭 이벤트 발생시 가상 기준값을 제어함
	DWORD			m_PointPos; // 0~100 까지의 가상기준값 스크롤 뷰와 바에 모두 적용될 기준입니다. 
	float _localx; // 직계 부모와 나의 상대적 위치
	float _localy; // 직계 부모와 나의 상대적 위치
	bool			_IsDown;
public:
	DECL_CREATE( CGUI_ListBox_SCROLL );
	CGUI_ListBox_SCROLL();
	CGUI_ListBox_SCROLL(CGUI_ListBox* pObj);
	~CGUI_ListBox_SCROLL();

	void	HEAD();
	void	OnUP();
	void	OnDown();
	void	TAIL();
	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);
};
////////////////////////////////////////
// 리스트에 Contents 추가 
// 실제 영역은 컨텐츠가 하나도 없을땐 0 
// 계속 추가될때마다 + 컨텐츠 크기 
// 추가된 컨텐츠 만큼 실제 사각형에 크기가 더 추가됨 

#pragma warning(disable:4786) 
#include <vector>
using namespace std;

//class CGUI_ListBox_SCROLL;

class CGUI_ListBox : public IGUI_Object
{
	CGUI_ListBox_SCROLL*  m_pScroll;
	std::string			_ScrollName;
	int			m_LastScrollPos;
	RECT		m_ContentsRect; // 리스트박스 내에서 콘테츠들이 화면에 보이는 영역, 클리핑시쓰임..*/
	int			m_FocusIndex;
	float		m_Interval_ToHEAD;
	float		m_Interval_ToTAIL;
	float		m_Interval_ToBODY;
	std::string		m_UpButtonImgFileName;
	std::string		m_DnButtonImgFileName;
	
public:
	DECL_CREATE(CGUI_ListBox )

		void SetScrollName(const char* szName){ _ScrollName= szName;}
	void SetFocusContentsIndex(int Index ); // 현재 선택된것의 인덱스를 반환한다.
	int	 GetFocusContentsIndex( void ); // 현재 선택된것의 인덱스를 반환한다.

	// 바운드박스를 셋팅해주면 한라인의 크기와..
	void	SetLineCnt( UINT LineCnt = 32 ); // 현재 정해준대로 컨텐츠 버퍼가 생성됩니다.
	void	AddString( TCHAR *szStr,DWORD OutClr,DWORD FontClr,bool bFocusString = false); // 스트링 잘라 붙이기 추가 
	void	AddContents( IGUI_Object* pObj ); //  외부에서 추가할때는 이것으로 추가함 
	void	RemoveAllContents(void);
	void	HEAD();
	void	TAIL();
	
	CGUI_ListBox();
	~CGUI_ListBox();

	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);

	// 리스트 박스와 스크롤바의 위치를 지정해줄 메소드가 필요함.
	// 아래는 스크롤 바가 상하좌우중 우에 붙었을때를 가정함 
	// 모든값은 + 일수록 리스트 박스와 멀어짐 
	// 수치는 상대적이다.현재 리스트 박스의 위치에서 상대적인 값임.
	// 수치의 종류는 세가지. 
	// 1. 리스트 박스 와 스크롤바의 몸통의 거리 
	// 2. 스크롤 바의 몸통은 기본적으로 리스트 박스의 가로(or 세로) 폭과 길이가 같으며
	//    헤드 테일 버튼을 각각 뺀만큼이다 .
	// 헤드와 테일을 

	// 스크롤 바가 생성될때만 아래 메소드를 쓴다.
	// 스크롤 바가 생성되고 나면 더이상 필요가 없음. 
	const char* GetUPButtonImgFileName(void){return m_UpButtonImgFileName.c_str(); }
	const char* GetDNButtonImgFileName(void){return m_DnButtonImgFileName.c_str(); }
	void SetUPButtonImgFileName(const char* fName=_T("data\\IMG\\GUI\\Scroll_Up.ani"))
	{ if(strlen(fName)<=0)return; m_UpButtonImgFileName = fName; } 

	void SetDNButtonImgFileName(const char* fName=_T("data\\IMG\\GUI\\Scroll_Dn.ani"))
	{ if(strlen(fName)<=0)return; m_DnButtonImgFileName = fName; } 

	void  SetInterval_ToHEAD( float Interval =0.0f );
	void  SetInterval_ToTAIL( float Interval =0.0f );
	void  SetInterval_ToBODY( float Interval =0.0f );

	void GetInterval_ToHEAD( float& Interval );
	void GetInterval_ToTAIL( float& Interval );
	void GetInterval_ToBODY( float& Interval );

};


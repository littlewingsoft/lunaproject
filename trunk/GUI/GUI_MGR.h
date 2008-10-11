

// GUI_MGR.h: interface for the CGUI_MGR class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "guistdafx.h"

struct GUIMSG;
class IGUI_Object;
// edit, pannel, button, list box( 판넬에 올라감)
//----------------------------------------------------------------------
// 위에 열거된 오브젝트들을 일괄 처리 하는 메니져 
//----------------------------------------------------------------------
// 모든 GUI들은 이것들 상속받아서 구현해놔야 할듯..
// 자식들은 부모에게 이것을 
class IGUIEvent 
{
public:
	virtual int GUIEvent_Hander( GUIMSG& Msg )=0;
};

#include <queue>
class CGUI_MGR  
{
private:
	static IGUI_Object* m_pLastFocus;
	static IGUI_Object* m_pFocus;

	static IGUI_Object::GUI_PTRLIST	 m_GuiList;
	typedef list<TCHAR*> 	GUI_EDITLIST; 
	static GUI_EDITLIST			m_EditList;

	static std::queue<std::string> _DestroyObjNameQ;
	
public:
	CGUI_MGR();
	~CGUI_MGR();
	
	static void	AddGUI( IGUI_Object* pGui );
	static IGUI_Object* FindGui(const TCHAR* Name);
	static void DestroyObj(const TCHAR* Name);
	static void DestroyObjQueue(const TCHAR* Name);	
	static void DestroyAllObj( DWORD mode= VOLATILE );
    
//	template<typename T> 적용가능하나 코딩의 압뷁~으로 인하여 다음에 쓰기로 함
//	void CreateObj(T*& pTmp, const TCHAR* ClsName,const TCHAR* fName, int x,int y, int Width, int Height );

	// 텍스트 만 생성합니다. 
	static CGUI_Text* CreateText( const TCHAR* szParentName, const TCHAR* MyName, int x,int y,DWORD OutClr,DWORD FontClr, const TCHAR* Str );
	// 아 졸라리 더티해 뭔가 해결방법을 찾아보세~ 
	// 현재로선 이렇게 생성하는것 외에는 방법을 모르겟음 템플릿을 가지고 어찌어찌 해보면 될듯 
	static CGUI_FocusText* CreateFocusText( const TCHAR* szParentName, const TCHAR* MyName, int x,int y,DWORD OutClr,DWORD FontClr, const TCHAR* Str );

	/*
			GUI_TEXT,
			GUI_ANI,
			GUI_PANNEL,		// 각종 버튼등이 올라감 그냥 드레그 기능만 있음 
			GUI_BUTTON, 
			GUI_EDITBOX, 
			GUI_LISTBOX,
	*/
	static void	ModalBox(TCHAR* szStr, ICommand* pCommand = NULL );
	static void	ProcessAll(void);
	static void	RenderAll(void);
	static void	ProcMessage(GUIMSG& pMsg);

	// 에디트 박스의 리스트를 정렬할때는 
	// 모달이 켜질때와 닫힐때
	// 씬이 시작될때 
	static void ReCalcEditList(IGUI_Object* pObj=NULL); // 현재 유용한 에디트 박스의 리스트를 재계산함 
	static void NextEditBox(void);

	static void SetGuiFocus( const TCHAR* Name );
	static void NoneFocusSetting(void);
	static void MoveCenter(const TCHAR* Name);
	static IGUI_Object* GetCurrFocus(void);

//	IGUI_Object* GetModalFocus(void);
	static IGUIEvent* m_pEventHandler;
	// 메세지 수신자를 셋팅함
	static void SetEventProc(IGUIEvent* pEvent ){ m_pEventHandler = pEvent; }
	// 자신에게 셋팅된 메세지 수신자에게 전송해주기만함
	static void _SendMessage(eOM dwMsg, DWORD Param1, DWORD Param2 );

	// 특정 GUI에게 메세지를 전송함 
	// FINDGUI 와 위의 _SendMessage를 합친것
	static void _SendMessage(const TCHAR*szGUIName,eOM dwMsg, DWORD Param1, DWORD Param2 );

	static bool _CreateFromINI( const TCHAR* szName); // 현재 INI 에 설정된것들을 추가,생성함.
	static void _DestroyFromINI(const TCHAR* szName);// 현재 추가,생성되어 잇는것들중 INI 에 있는것들만 없애버림

	static bool CreateFromINI( const TCHAR* szName); // 현재 INI 에 설정된것들을 추가,생성함.
	static void DestroyFromINI(const TCHAR* szName);// 현재 추가,생성되어 잇는것들중 INI 에 있는것들만 없애버림
};

template<typename T>
T* GUI_CreateObj( const TCHAR* ClsName,const TCHAR* fName, int x,int y, int Width, int Height )
{
	T* pTmp= T::Create();
	
	pTmp->SetName( ClsName );
	pTmp->m_Img.TexID =-1;

	if( fName != NULL )
	g_D3D.LoadAni( fName, pTmp->m_Img );

	if( fName != NULL && pTmp->m_Img.TexID == -1 )
	{
		TCHAR buff[64]={0,};
		_stprintf( buff, "don't find the  %s ", fName );
		MessageBox( 0,buff,"err",MB_OK);
		if( pTmp){ delete pTmp; }
		return NULL;
	}

if( fName != NULL )
{
	if( Width <= 0 ) 
		Width = pTmp->m_Img.sprInfo.nWidth;
	
	if( Height <= 0 ) 
		Height = pTmp->m_Img.sprInfo.nHeight;
}

//  Add 촤일드 하면 부모의 좌표에 맞게 설정 
	pTmp->SetSrcRect( 0,0, Width,Height);
	pTmp->SetDestRect( (float)x, (float)y,
						(float)(x+Width),
						(float)(y+Height) );

	pTmp->SetBoundRect( (float)x, (float)y,
						(float)(x+Width),
						(float)(y+Height) );
	return pTmp;
}
//extern CGUI_MGR g_GuiMGR;

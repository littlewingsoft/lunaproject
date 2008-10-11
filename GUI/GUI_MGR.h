

// GUI_MGR.h: interface for the CGUI_MGR class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "guistdafx.h"

struct GUIMSG;
class IGUI_Object;
// edit, pannel, button, list box( �ǳڿ� �ö�)
//----------------------------------------------------------------------
// ���� ���ŵ� ������Ʈ���� �ϰ� ó�� �ϴ� �޴��� 
//----------------------------------------------------------------------
// ��� GUI���� �̰͵� ��ӹ޾Ƽ� �����س��� �ҵ�..
// �ڽĵ��� �θ𿡰� �̰��� 
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
    
//	template<typename T> ���밡���ϳ� �ڵ��� �Д�~���� ���Ͽ� ������ ����� ��
//	void CreateObj(T*& pTmp, const TCHAR* ClsName,const TCHAR* fName, int x,int y, int Width, int Height );

	// �ؽ�Ʈ �� �����մϴ�. 
	static CGUI_Text* CreateText( const TCHAR* szParentName, const TCHAR* MyName, int x,int y,DWORD OutClr,DWORD FontClr, const TCHAR* Str );
	// �� ���� ��Ƽ�� ���� �ذ����� ã�ƺ���~ 
	// ����μ� �̷��� �����ϴ°� �ܿ��� ����� �𸣰��� ���ø��� ������ ������� �غ��� �ɵ� 
	static CGUI_FocusText* CreateFocusText( const TCHAR* szParentName, const TCHAR* MyName, int x,int y,DWORD OutClr,DWORD FontClr, const TCHAR* Str );

	/*
			GUI_TEXT,
			GUI_ANI,
			GUI_PANNEL,		// ���� ��ư���� �ö� �׳� �巹�� ��ɸ� ���� 
			GUI_BUTTON, 
			GUI_EDITBOX, 
			GUI_LISTBOX,
	*/
	static void	ModalBox(TCHAR* szStr, ICommand* pCommand = NULL );
	static void	ProcessAll(void);
	static void	RenderAll(void);
	static void	ProcMessage(GUIMSG& pMsg);

	// ����Ʈ �ڽ��� ����Ʈ�� �����Ҷ��� 
	// ����� �������� ������
	// ���� ���۵ɶ� 
	static void ReCalcEditList(IGUI_Object* pObj=NULL); // ���� ������ ����Ʈ �ڽ��� ����Ʈ�� ������ 
	static void NextEditBox(void);

	static void SetGuiFocus( const TCHAR* Name );
	static void NoneFocusSetting(void);
	static void MoveCenter(const TCHAR* Name);
	static IGUI_Object* GetCurrFocus(void);

//	IGUI_Object* GetModalFocus(void);
	static IGUIEvent* m_pEventHandler;
	// �޼��� �����ڸ� ������
	static void SetEventProc(IGUIEvent* pEvent ){ m_pEventHandler = pEvent; }
	// �ڽſ��� ���õ� �޼��� �����ڿ��� �������ֱ⸸��
	static void _SendMessage(eOM dwMsg, DWORD Param1, DWORD Param2 );

	// Ư�� GUI���� �޼����� ������ 
	// FINDGUI �� ���� _SendMessage�� ��ģ��
	static void _SendMessage(const TCHAR*szGUIName,eOM dwMsg, DWORD Param1, DWORD Param2 );

	static bool _CreateFromINI( const TCHAR* szName); // ���� INI �� �����Ȱ͵��� �߰�,������.
	static void _DestroyFromINI(const TCHAR* szName);// ���� �߰�,�����Ǿ� �մ°͵��� INI �� �ִ°͵鸸 ���ֹ���

	static bool CreateFromINI( const TCHAR* szName); // ���� INI �� �����Ȱ͵��� �߰�,������.
	static void DestroyFromINI(const TCHAR* szName);// ���� �߰�,�����Ǿ� �մ°͵��� INI �� �ִ°͵鸸 ���ֹ���
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

//  Add ���ϵ� �ϸ� �θ��� ��ǥ�� �°� ���� 
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

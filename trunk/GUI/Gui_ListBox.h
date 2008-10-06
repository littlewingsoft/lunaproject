
#pragma once
#include "IGui_Object.h"
#include "Gui_Ani.h"
#include "Gui_Button.h"
#include <string>
////////////////////////////////////////
// ��ũ���� ������ִ� �⺻������ ������ �޸��� ��,��,�� �� ��ư��Ȱ
class CGUI_ListBox;
class CGUI_ListBox_SCROLL : public IGUI_Object
{
	CGUI_Ani*		m_pBack;	// �Ѱ��� �ε��ؼ� Ƚ����ŭ �������Ƽ� ����.
	CGUI_Button*	m_pUp;			// Ŭ���̺�Ʈ �߻��� ���� ���ذ��� ������
	CGUI_Button*	m_pBar;		// ���� ���ذ��� ���� ���� ��ġ�� ���ϱ⵵ �ϰ� ������ذ��� �����ϱ⵵��. 
	CGUI_Button*	m_pDn;			// Ŭ�� �̺�Ʈ �߻��� ���� ���ذ��� ������
	DWORD			m_PointPos; // 0~100 ������ ������ذ� ��ũ�� ��� �ٿ� ��� ����� �����Դϴ�. 
	float _localx; // ���� �θ�� ���� ����� ��ġ
	float _localy; // ���� �θ�� ���� ����� ��ġ
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
// ����Ʈ�� Contents �߰� 
// ���� ������ �������� �ϳ��� ������ 0 
// ��� �߰��ɶ����� + ������ ũ�� 
// �߰��� ������ ��ŭ ���� �簢���� ũ�Ⱑ �� �߰��� 

#pragma warning(disable:4786) 
#include <vector>
using namespace std;

//class CGUI_ListBox_SCROLL;

class CGUI_ListBox : public IGUI_Object
{
	CGUI_ListBox_SCROLL*  m_pScroll;
	std::string			_ScrollName;
	int			m_LastScrollPos;
	RECT		m_ContentsRect; // ����Ʈ�ڽ� ������ ���������� ȭ�鿡 ���̴� ����, Ŭ���νþ���..*/
	int			m_FocusIndex;
	float		m_Interval_ToHEAD;
	float		m_Interval_ToTAIL;
	float		m_Interval_ToBODY;
	std::string		m_UpButtonImgFileName;
	std::string		m_DnButtonImgFileName;
	
public:
	DECL_CREATE(CGUI_ListBox )

		void SetScrollName(const char* szName){ _ScrollName= szName;}
	void SetFocusContentsIndex(int Index ); // ���� ���õȰ��� �ε����� ��ȯ�Ѵ�.
	int	 GetFocusContentsIndex( void ); // ���� ���õȰ��� �ε����� ��ȯ�Ѵ�.

	// �ٿ��ڽ��� �������ָ� �Ѷ����� ũ���..
	void	SetLineCnt( UINT LineCnt = 32 ); // ���� �����ش�� ������ ���۰� �����˴ϴ�.
	void	AddString( TCHAR *szStr,DWORD OutClr,DWORD FontClr,bool bFocusString = false); // ��Ʈ�� �߶� ���̱� �߰� 
	void	AddContents( IGUI_Object* pObj ); //  �ܺο��� �߰��Ҷ��� �̰����� �߰��� 
	void	RemoveAllContents(void);
	void	HEAD();
	void	TAIL();
	
	CGUI_ListBox();
	~CGUI_ListBox();

	void	Process();
	void	Render();
	bool	ProcMessage(GUIMSG& pMsg);

	// ����Ʈ �ڽ��� ��ũ�ѹ��� ��ġ�� �������� �޼ҵ尡 �ʿ���.
	// �Ʒ��� ��ũ�� �ٰ� �����¿��� �쿡 �پ������� ������ 
	// ��簪�� + �ϼ��� ����Ʈ �ڽ��� �־��� 
	// ��ġ�� ������̴�.���� ����Ʈ �ڽ��� ��ġ���� ������� ����.
	// ��ġ�� ������ ������. 
	// 1. ����Ʈ �ڽ� �� ��ũ�ѹ��� ������ �Ÿ� 
	// 2. ��ũ�� ���� ������ �⺻������ ����Ʈ �ڽ��� ����(or ����) ���� ���̰� ������
	//    ��� ���� ��ư�� ���� ����ŭ�̴� .
	// ���� ������ 

	// ��ũ�� �ٰ� �����ɶ��� �Ʒ� �޼ҵ带 ����.
	// ��ũ�� �ٰ� �����ǰ� ���� ���̻� �ʿ䰡 ����. 
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


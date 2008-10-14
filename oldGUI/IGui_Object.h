
#pragma once

#include "enum_CLSID.h"
#pragma warning(disable: 4786) 
#include <vector>
using namespace std;
#include "resmgr.h"
#include "objmsg.h"


enum GUICLSID 
	{
		IGUI=0,
		GUI_PANNEL,		// 각종 버튼등이 올라감 그냥 드레그 기능만 있음 
		GUI_PANNEL_CONSOLE,    // 기본적인 공지를 보여줄 콘솔창 추후 Html 도 지원예정 
		GUI_PANNEL_CONTEXTMENU,//is a pannel
		GUI_PANNEL_SHOWINFO, //is a pannel
		GUI_PANNEL_MAKEROOM,
		GUI_PANNEL_OPTION,

		GUI_EDITBOX, 
		GUI_TEXT,
		GUI_ANI,
		GUI_BUTTON, 
		GUI_CHECKBUTTON,
		GUI_RADIOBUTTON,
		GUI_LISTBOX,
		GUI_AVATAR
	};

	struct fRECT
	{
		float left;
		float top;
		float right;
		float bottom;
	};
#define REUSE      0x01
#define VOLATILE   0x02

//#define DECL_CREATE(xxx) 
#define DECL_CREATE(xxx) static xxx* Create(){ return new xxx; }

	class IGUI_Object
	{

	public:
		IGUI_Object();
		virtual ~IGUI_Object();



		typedef vector<IGUI_Object* > GUI_PTRLIST;
		typedef GUI_PTRLIST::iterator GUI_LISTITOR;

		void MoveGUI( float x, float y );

		virtual bool ProcMessage( GUIMSG& pMsg ) = 0;
		virtual void Process(void ) = 0;
		virtual void Render( void ) = 0;

		RES_ANI	m_Img;

		void	SetX( float X );
		void	SetY( float Y );
		float GetX(void);//inline 
		float GetY(void);
		float GetWidth(void);
		float GetHeight(void);

		void SetSrcRect(int l,int t,int r,int b);
		void SetDestRect( float l, float t, float r, float b);
		void GetDestRect(RECT&rt);
		void SetBoundRect(float l,float t,float r,float b);
		void GetBoundRect( fRECT& rt);

		inline BYTE	GetLayer(void);
		GUICLSID GetClsID();

		GUI_PTRLIST	GetChildList( void );

		void CalcViewRect( fRECT* ViewRT );

		bool IsModal(void);
		void	SetLastModal( IGUI_Object* pLastModal );
		IGUI_Object* GetLastModal(void);

		// 더이상 올라갈수없는 최고 부모를 찾는다
		// NULL 이라면 이미 자신이 최상위 부모인셈 
		IGUI_Object* GetSuperParent(void);
		IGUI_Object* GetParent(void);
		void	SetParent( IGUI_Object* pParent);
		void	AddChild(IGUI_Object* pChild );
		void	DestroyAllChild();
		void	SetLayer(BYTE Layer);
		void	SetModal( bool bl );
		void	SetClr(DWORD Clr);

		void SetName( const TCHAR* const Name );
		TCHAR* GetName(void);
		IGUI_Object* FindGui( const TCHAR* Name );

		void	Process_Child();
		void	Render_Child();
		bool	ProcMessage_Child(GUIMSG& pMsg );
		void	SetLifeMode(DWORD Flag){ m_LifeMode=  Flag;}
		inline DWORD   GetLifeMode()const { return m_LifeMode; }
		bool	IsShow()const{return m_bShow;}
		void	SetShow(bool bShow){m_bShow=bShow;}
	protected:

		TCHAR   m_Name[24]; // 한글 4글자 영,수 8글자까지 	
		RECT	m_SrcRect;		// 화면에 보여질 원본
		fRECT	m_DestRect;		// 화면에 보여질 대상영역 
		fRECT	m_BoundRect;	// 대상영역 
		fRECT	m_LastBoundRect;//  

		GUICLSID m_Gui_ClsID;
		GUI_PTRLIST	m_ChildList;
		IGUI_Object* m_pParent; // m_pParent 가 NULL 이면 자신은 최상위 부모 
//		IGUI_Object* m_pLastModal; // 마지막 모달값을 가지고 있는다..
		bool		m_bModal;
		DWORD		m_Clr;
		BYTE		m_Layer;
		bool		m_bShow;
		DWORD		m_LifeMode;
	};




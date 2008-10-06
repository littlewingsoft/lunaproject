// IGui_Object.cpp: implementation of the IGui_Object class.
//
//////////////////////////////////////////////////////////////////////
//#include "video_D3D9.h"
//#include <IGui_Object.h>
#include <guistdafx.h>
#ifdef _HTMLLOG
#include "HtmlLog.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

	IGUI_Object::IGUI_Object():m_LifeMode(VOLATILE),m_bShow(true),m_Gui_ClsID( IGUI )
	{ 
		m_Img.TexID = -1;
//		m_pLastModal = NULL;
		m_pParent = NULL;
		m_Layer = D3D_INTERFACE_LAYER0;
		m_bModal = false; 
		m_Clr = 0xffffffff; 

		SetSrcRect( 0,0,0,0 );
		SetDestRect(0,0,0,0);
		SetBoundRect( 0,0,0,0);

		//	_tcscpy( m_Name, _T("무용지물GUI"));
		m_Name[0]='U';
		m_Name[1]='N';
		m_Name[2]='U';
		m_Name[3]='S';
		m_Name[4]='E';
		m_Name[5]='D';
		m_Name[6]='\0';

	}

	IGUI_Object::~IGUI_Object()
	{
#ifdef _HTMLLOG
		//	g_HtmlLog.LogInfo( _T("%s 소멸됨"),m_Name );
#endif

		DestroyAllChild();
		//	if( m_Img.TexID != -1 )
		//	g_D3D.ReleaseANI( m_Img.TexID );
	}

	void	IGUI_Object::SetClr(D3DCOLOR Clr)
	{
		m_Clr = Clr; 
	}

	void	IGUI_Object::SetX( float X )
	{
		m_BoundRect.right = X + GetWidth() ;
		m_BoundRect.left = X;


		m_DestRect.left = m_BoundRect.left; 
		m_DestRect.top = m_BoundRect.top; 
		m_DestRect.right = m_BoundRect.right; 
		m_DestRect.bottom = m_BoundRect.bottom; 
	}

	void	IGUI_Object::SetY( float Y )
	{
		float Height = GetHeight() ;
		m_BoundRect.bottom = floorf(Y+ Height) ;
		m_BoundRect.top = Y;


		m_DestRect.left = m_BoundRect.left; 
		m_DestRect.top = m_BoundRect.top; 
		m_DestRect.right = m_BoundRect.right; 
		m_DestRect.bottom = m_BoundRect.bottom; 
	}

	float IGUI_Object::GetX(void)
	{
		return m_BoundRect.left; 
	}

	float IGUI_Object::GetY(void)
	{
		return m_BoundRect.top; 
	}

	float IGUI_Object::GetWidth(void)
	{
		return m_BoundRect.right - m_BoundRect.left;
	}

	float IGUI_Object::GetHeight(void)
	{
		return m_BoundRect.bottom - m_BoundRect.top;
	}



	void IGUI_Object::SetSrcRect(int l,int t,int r,int b)
	{
		m_SrcRect.left   = l;
		m_SrcRect.top    = t;
		m_SrcRect.right  = r;
		m_SrcRect.bottom = b;
	}

	void IGUI_Object::SetDestRect( float l, float t, float r, float b)
	{
		/*	float DestW = m_DestRect.right- m_DestRect.left;
		if( DestW <= 0.9f )
		DestW = r- l;
		float DestH = m_DestRect.bottom - m_DestRect.top;
		if( DestH <= 0.9f)
		DestH = b - t;*/
		m_DestRect.left		= l;
		m_DestRect.top		= t;
		m_DestRect.right	= r;
		m_DestRect.bottom	= b;
		//	m_DestRect.right	= m_DestRect.left+ DestW;
		//	m_DestRect.bottom	= m_DestRect.top + DestH;

	}

	void IGUI_Object::SetBoundRect(float l,float t,float r,float b)
	{
		m_BoundRect.left	= l;
		m_BoundRect.top		= t;
		m_BoundRect.right	= r;
		m_BoundRect.bottom	= b;

		m_LastBoundRect = m_BoundRect; 
		/*
		int DestW = m_DestRect.right- m_DestRect.left;		// 이걸로 해야 삑살은 안남..
		int DestH = m_DestRect.bottom - m_DestRect.top;
		m_DestRect.left		= l;
		m_DestRect.top		= t;
		m_DestRect.right	= m_DestRect.left+ DestW;
		m_DestRect.bottom	= m_DestRect.top + DestH;
		*/
	}

	void IGUI_Object::GetBoundRect( fRECT& rt)
	{
		rt = m_BoundRect;
	}

	inline BYTE	IGUI_Object::GetLayer(void)
	{
		return m_Layer; 
	}

	GUICLSID IGUI_Object::GetClsID()
	{
		return m_Gui_ClsID; 
	}

	IGUI_Object::GUI_PTRLIST	IGUI_Object::GetChildList( void ) 
	{
		return m_ChildList; 
	}

	bool IGUI_Object::IsModal(void) 
	{
		return m_bModal; 
	}

	//void	IGUI_Object::SetLastModal( IGUI_Object* pLastModal )
	//{ 
	//	m_pLastModal = pLastModal; 
	//}

	//IGUI_Object* IGUI_Object::GetLastModal(void) 
	//{
	//	return m_pLastModal; 
	//}

	void IGUI_Object::SetName( const TCHAR* const Name )
	{
		assert( _tcslen(Name ) > 0 );
		_tcscpy( m_Name,Name); 
	}

	TCHAR* IGUI_Object::GetName(void) 
	{
		return m_Name; 
	}

	IGUI_Object* IGUI_Object::FindGui( const TCHAR* Name )
	{
		GUI_LISTITOR it = m_ChildList.begin();
		IGUI_Object* pChild = NULL;
		while( it != m_ChildList.end() )
		{
			pChild = *it;
			if( _tcscmp( Name, pChild->GetName() ) == 0 ) return pChild;

			it++;
		}

		return NULL;
	}

	void IGUI_Object::SetModal( bool bl )
	{
		m_bModal = bl;

		GUI_LISTITOR it = m_ChildList.begin();
		IGUI_Object* pChild = NULL;
		while( it != m_ChildList.end() )
		{
			pChild = *it;
			pChild->SetModal( bl );
			it++;
		}
	}

	IGUI_Object* IGUI_Object::GetSuperParent(void)
	{
		IGUI_Object* pObj = this; ;//GetParent();
		while( 1 )
		{
			if( pObj->GetParent() == NULL ) break;
			pObj = pObj->GetParent();

		}

		return pObj;  
	}

	IGUI_Object* IGUI_Object::GetParent(void)
	{
		return m_pParent;  
	}

	void IGUI_Object:: SetParent( IGUI_Object* pParent)
	{
		assert( pParent != NULL );
		m_pParent = pParent;

		if( pParent->IsModal() )
			m_bModal = true;	// 모달의 속성을 이어 받는다.
		else 
			m_bModal = false;	

		m_Layer = pParent->GetLayer();	

		SetDestRect( 	pParent->GetX() + m_DestRect.left,
			pParent->GetY() + m_DestRect.top,
			pParent->GetX() + m_DestRect.right,
			pParent->GetY() + m_DestRect.bottom);

		SetBoundRect(	pParent->GetX() + m_BoundRect.left,
			pParent->GetY() + m_BoundRect.top,
			pParent->GetX() + m_BoundRect.right,
			pParent->GetY() + m_BoundRect.bottom
			);

		m_LastBoundRect = m_BoundRect;
	}

	void IGUI_Object::SetLayer( BYTE Layer )
	{
		m_Layer= Layer;

		GUI_LISTITOR it = m_ChildList.begin();
		IGUI_Object* pChild = NULL;
		while( it != m_ChildList.end() )
		{
			pChild = *it;
			pChild ->SetLayer( Layer );
			it++;
		}
	}


	// x,y는 현재 위치에서 이동할 양.
	void IGUI_Object::MoveGUI(float dx, float dy)
	{

		SetBoundRect(	m_BoundRect.left  + dx, (m_BoundRect.top    + dy), 
			m_BoundRect.right + dx, (m_BoundRect.bottom + dy)  );

		SetDestRect( m_DestRect.left+ dx, m_DestRect.top + (dy),
			m_DestRect.right+ dx, m_DestRect.bottom + (dy) );

		//char ccc[512]={0,};
		//sprintf( ccc,"%f\n",m_DestRect.bottom  - m_DestRect.top );
		//OutputDebugString(ccc);
		GUI_LISTITOR it = m_ChildList.begin();
		IGUI_Object* pChild = NULL;
		while( it != m_ChildList.end() )
		{
			pChild = *it;
			GUIMSG msg;
			msg.dwMsg = GUIMSG_MOVEWND; // 자식이 이메세지를 다시 MoveGUI를 호출하면 무한호출인셈 
			msg.dwParam1 = dx;
			msg.dwParam2 = dy; 
			pChild ->ProcMessage( msg );
			pChild -> MoveGUI( dx, dy );
			it++;
		}
	}

	void IGUI_Object:: AddChild( IGUI_Object* pChild ) 
	{
		assert( pChild != NULL );
		m_ChildList.push_back( pChild );
		pChild->SetParent( this );

		GUIMSG msg;
		msg.dwMsg = GUIMSG_CREATED; // 생성됨을 통보한다. 
		msg.dwParam2 = msg.dwParam1 = 0;
		pChild->ProcMessage( msg );
	};

	void IGUI_Object::DestroyAllChild(void)
	{
		IGUI_Object* pChild= NULL;
		GUI_LISTITOR it = m_ChildList.begin();
		GUI_LISTITOR end = m_ChildList.end();
		while( it != end ) 
		{
			pChild = *it;
			pChild->DestroyAllChild(); // 자식들 모두 죽어랏~ ㅋㅋㅋ 
			if( pChild )
			{
				GUIMSG msg;
				msg.dwMsg = GUIMSG_DESTROY;
				msg.dwParam2 = msg.dwParam1;
				pChild->ProcMessage(msg);
				delete pChild;
				pChild = NULL;
			}

			it++;
		}
		m_ChildList.clear();
	}


	void	IGUI_Object::Process_Child()
	{
		GUI_LISTITOR it = m_ChildList.begin();
		while( it != m_ChildList.end() )
		{
			if( (*it)->IsShow() )
			{
				(*it)->Process_Child();
				(*it)->Process();
			}
			it++;
		}
	}

	void	IGUI_Object::Render_Child()
	{
		GUI_LISTITOR it = m_ChildList.begin();
		while( it != m_ChildList.end() )
		{
			if( (*it)->IsShow() )
			{
				(*it)->Render();
				(*it)->Render_Child();
			}
			it++;
		}
	}

	bool IGUI_Object::ProcMessage_Child(GUIMSG& Msg )
	{
		GUI_LISTITOR it = m_ChildList.begin();
		while( it != m_ChildList.end() )
		{
			if( (*it)->ProcMessage_Child( Msg ) == true )
				return true;
			
			if( (*it)->ProcMessage( Msg ) == true )
				return true;
			it++;
		}

		return false;
	}

	void IGUI_Object::CalcViewRect( fRECT* ParentViewRt )
	{	
		//	return ;
		// DestRect 가 원상회복 되는 시점이면 
		// 크기가 본래의 BoundRect 보다 작을때는 부모의 창과 비교하여 
		// Bound 가 안에 둘다 들어가면 
		RECT LastView = m_SrcRect;

		// Bound가 부모의 밖에 나가면 Src 는 가감이생긴다.
		// 밖에 나가지 않는다면 0 또는 
		if( (int)m_BoundRect.left > ParentViewRt->left  ) 
			m_SrcRect.left= 0;
		if( (int)m_BoundRect.left<= ParentViewRt->left)
			m_SrcRect.left= (long)(ParentViewRt->left- (int)m_BoundRect.left) ;

		if( (int)m_BoundRect.right<= ParentViewRt->right) // 안에 있을때 
			m_SrcRect.right= m_Img.sprInfo.nWidth ;

		if( (int)m_BoundRect.right> ParentViewRt->right ) 
			m_SrcRect.right= (long)( m_Img.sprInfo.nWidth - ((int)m_BoundRect.right- ParentViewRt->right) );

		if( (int)m_BoundRect.top > ParentViewRt->top  ) 
			m_SrcRect.top = 0;
		if( (int)m_BoundRect.top    <= ParentViewRt->top  )
			m_SrcRect.top = (long)(ParentViewRt->top  - (int)m_BoundRect.top) ;

		if( (int)m_BoundRect.bottom<= ParentViewRt->bottom)
			m_SrcRect.bottom= m_Img.sprInfo.nHeight ;

		if( (int)m_BoundRect.bottom  > ParentViewRt->bottom ) 
			m_SrcRect.bottom = (long)(m_Img.sprInfo.nHeight - ((int)m_BoundRect.bottom - ParentViewRt->bottom)) ;


		// 나의 Bound 가 부모의 렉트에 다 들어오면 Src 는 보정이 된다.



		// 보정합니다. ----------
		if( m_SrcRect.left     < 0 ) 
			m_SrcRect.left    = 0;
		if( m_SrcRect.left    > m_Img.sprInfo.nWidth ) 
			m_SrcRect.left    = m_Img.sprInfo.nWidth;

		if( m_SrcRect.right < 0 )
			m_SrcRect.right = 0;
		if( m_SrcRect.right > m_Img.sprInfo.nWidth ) 
			m_SrcRect.right = m_Img.sprInfo.nWidth;

		if(	m_SrcRect.top < 0 ) 
			m_SrcRect.top= 0;
		if( m_SrcRect.top> m_Img.sprInfo.nHeight )
			m_SrcRect.top= m_Img.sprInfo.nHeight ;

		if( m_SrcRect.bottom < 0 )
			m_SrcRect.bottom = 0;
		if( m_SrcRect.bottom > m_Img.sprInfo.nHeight ) 
			m_SrcRect.bottom = m_Img.sprInfo.nHeight;


		m_DestRect.left   += int(m_SrcRect.left   - LastView.left ) ;
		m_DestRect.right  += int(m_SrcRect.right  - LastView.right ) ;
		int top = int(m_SrcRect.top    - LastView.top );
		if( top != 0 ) 
			int a=0;
		m_DestRect.top    += top;
		int bot = int(m_SrcRect.bottom - LastView.bottom );
		if( bot != 0 )
			int b = 0;
		m_DestRect.bottom += bot;

	}

	void IGUI_Object::GetDestRect(RECT&rt)
	{
		rt.left   = (long)m_DestRect.left;
		rt.top    = (long)m_DestRect.top;
		rt.right  = (long)m_DestRect.right;
		rt.bottom = (long)m_DestRect.bottom;
	}


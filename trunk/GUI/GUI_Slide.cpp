
#include "stdafx.h"


CGUI_Slide* CGUI_Slide::_pCurrentDownSlide = NULL;
CGUI_Slide::CGUI_Slide(void):CGUI_Button()
{
	_Style=HORIZON;
}

CGUI_Slide::~CGUI_Slide(void)
{
}
int CGUI_Slide::GetRate(){ return _Rate; }
void CGUI_Slide::SetRate(int rate)
{
	if( rate<0) rate = 0;
	if( rate>100) rate = 100;

	_Rate = rate;
	float frate = rate / 100.0f;


	IGUI_Object* pObj = GetParent();
	float result=pObj->GetX()+this->_AllowRegion_Start+ ( _AllowRegion_End- _AllowRegion_Start-GetWidth())*frate;
	SetX( result );
	CGUI_MGR::_SendMessage( GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,_Rate );
	// 자기 최고 부모가 있다면 찾아서 이벤트 통지
	IGUI_Object* pParent1 = GetSuperParent();
	if( pParent1 != NULL )
		CGUI_MGR::_SendMessage( pParent1->GetName(), GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,_Rate );//m_Event
}

bool CGUI_Slide::ProcMessage(GUIMSG& Msg)
{
	// 비활성화된 상태라면 GUIMSG_ENABLE를 제외하고 모두 반사
	if( m_State == INACTIVE && Msg.dwMsg != GUIMSG_ENABLE ) 
		return false;

	switch ( Msg.dwMsg )
	{
	case GUIMSG_CREATED:
		{
			
		}
		return true;

	case GUIMSG_DISABLE:
		{
			m_State = INACTIVE;
		}
		return true;

	case GUIMSG_DESTROY:
		{
		}
		return true;

	case GUIMSG_CLOSE:
		{
			//CGUI_MGR::NoneFocusSetting();
			DestroyAllChild();
		}
		return false;

	case GUIMSG_DRAG:
		{
			// 실제 마우스 위치로 스크롤 바 영역의 100 분율기준 위치를 구하고
			// 그 100분율 기준을 실제 스크롤 바로 환산 적용한다. 이부분은 되어 있으므로 
			// 스크롤바 영역에서의 100분율 위치만 구해줌.
			// 체크 기준은 버튼의 영역 내부이거나 이전에 눌린 기억이 있다면 오케이
			int mousex = Msg.dwParam1;
			int mousey = Msg.dwParam2;
			POINT pt={mousex,mousey};
			
			RECT rt={m_DestRect.left,m_DestRect.top,m_DestRect.right,m_DestRect.bottom};
			if(_pCurrentDownSlide != this )
				return false;
			if(  m_State != DOWN  &&  PtInRect(&rt,pt)  == FALSE )
					 return false;

			int relativex = GetX();
			IGUI_Object* pParent = GetParent();
			if( pParent )
			relativex  = GetX() - pParent->GetX();
			float Bar_X = relativex  - _AllowRegion_Start;
			float Bar_Width = GetWidth();
			float newx = 0.0f;
			float Enable_Region = (_AllowRegion_End -_AllowRegion_Start)-GetWidth();
			DWORD PointRate = (Bar_X/Enable_Region) *100.0f;

			int intervalx = this->GetFirstClick().x;
			int x =int( mousex - intervalx ); // 간격을 유지해줘야 합니다.

			if( x < pParent->GetX()+_AllowRegion_Start )// 는 같거나 커야 하고 
			{
				x = int(pParent->GetX()+_AllowRegion_Start); 
			}
			else if( x+Bar_Width>=  pParent->GetX() + _AllowRegion_End )
			{
				x = pParent->GetX()+_AllowRegion_End - Bar_Width;
			}

			SetX( float(x) ); //m_BoundRect.top+

			// 포인트를 움직였으므로 
			// 스크롤이 움직인 만큼 영향을 준다. 
			// 포인트가 움직일수 있는 영역은 전체 DESTRECT 에서 위아래 각각 point_Height /2 한 만큼 이다.
			// 그만큼을 이동가능한 영역의 전체로 보고 현재 위치는 전체에서의 상대적인 위치의 2048 분율로 한다.
			// 2의 승수 승이므로 / 연산시 소숫점 5자리 까지는 오차가 생기지 않음 
			// 현재 마우스 위치???? 드레그 시에는 100분율로 역시 이동함... 
			_Rate = PointRate;
			CGUI_MGR::_SendMessage( GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,PointRate );
			// 자기 최고 부모가 있다면 찾아서 이벤트 통지
			IGUI_Object* pParent1 = GetSuperParent();
			if( pParent1 != NULL )
				CGUI_MGR::_SendMessage( pParent1->GetName(), GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,PointRate );//m_Event
		}
		return true;

	case GUIMSG_MLDOWN:
		{
			POINT pt={g_MouseMGR.GetX(),g_MouseMGR.GetY() };
			// 현재 이동가능 영역안에 있다면 버튼을 움직여 버려
			_pCurrentDownSlide = this;
			bool bInSpace = false;
			IGUI_Object* pSuper = GetSuperParent();
			int regionx = 0;
			int regiony = 0;
			if( pSuper)
			{
				regionx = pSuper->GetX();
			}
			RECT rt0 = { regionx+_AllowRegion_Start, m_DestRect.top, regionx+_AllowRegion_End,m_DestRect.bottom } ;
			if( PtInRect( &rt0,pt) )
			{
				// 마우스 위치는 이동버튼의 중간위치 이며
				// 중간이상의 위치면 그 차이만큼이 위치가 된다 ( 써글 )
				float half = (GetWidth()/2);
				float res  = pt.x;

				if( res-half < rt0.left )
					res = (rt0.left ) ;
				else if( res+GetWidth() > rt0.right )
					res = rt0.right - GetWidth();
				else 
					res -= half;

				SetX( res  );
				bInSpace = true;
			}
			RECT rt = { m_DestRect.left,m_DestRect.top,m_DestRect.right,m_DestRect.bottom };
			if( PtInRect( &rt, pt ) || bInSpace )
			{
				CGUI_MGR::SetGuiFocus( m_Name );	// 부모에게 포커스를 준다.
				m_FirstClick.x = pt.x - m_BoundRect.left;
				m_FirstClick.y = pt.y - m_BoundRect.top ;
				m_State = DOWN;
				g_sndmgr.Play( "SND_CLICK" );

				GUIMSG msg;
				msg.dwMsg = GUIMSG_DRAG;
				msg.dwParam1 = pt.x ;
				msg.dwParam2 = pt.y;
				this->ProcMessage(msg);
				return true;
			}
		}
		break;

	case GUIMSG_MLUP:
		{
			_pCurrentDownSlide = NULL;
			POINT pt={g_MouseMGR.GetX(),g_MouseMGR.GetY() };
			RECT rt = { m_DestRect.left,m_DestRect.top,m_DestRect.right,m_DestRect.bottom };

			if( PtInRect( &rt, pt ) )
			{
				m_State = FOCUSON;
				
				//if( IsEvent() == true )
				//{
				//	CGUI_MGR::_SendMessage( GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,0 );//m_Event
				//	// 자기 최고 부모가 있다면 찾아서 이벤트 통지
				//	IGUI_Object* pParent = GetSuperParent();
				//	if( pParent != NULL )
				//	CGUI_MGR::_SendMessage( pParent->GetName(), GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,0 );//m_Event
				//}
				return true;
			}else
				m_State = NORMAL;
			

		}break;

	}
	return false;
}

void CGUI_Slide::Render()
{
	if( m_Img.TexID != -1 )
	{
		RECT tmp={m_DestRect.left,m_DestRect.top, m_DestRect.right,m_DestRect.bottom };
		g_D3D.BltSprite( m_Layer ,m_Img.TexID, NULL, tmp, m_Clr, D3D_NORMALSTATE); //D3D_ALPHASTATE //D3D_INTERFACE_LAYER0
	}
}
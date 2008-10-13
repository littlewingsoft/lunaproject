
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
	// �ڱ� �ְ� �θ� �ִٸ� ã�Ƽ� �̺�Ʈ ����
	IGUI_Object* pParent1 = GetSuperParent();
	if( pParent1 != NULL )
		CGUI_MGR::_SendMessage( pParent1->GetName(), GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,_Rate );//m_Event
}

bool CGUI_Slide::ProcMessage(GUIMSG& Msg)
{
	// ��Ȱ��ȭ�� ���¶�� GUIMSG_ENABLE�� �����ϰ� ��� �ݻ�
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
			// ���� ���콺 ��ġ�� ��ũ�� �� ������ 100 �������� ��ġ�� ���ϰ�
			// �� 100���� ������ ���� ��ũ�� �ٷ� ȯ�� �����Ѵ�. �̺κ��� �Ǿ� �����Ƿ� 
			// ��ũ�ѹ� ���������� 100���� ��ġ�� ������.
			// üũ ������ ��ư�� ���� �����̰ų� ������ ���� ����� �ִٸ� ������
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
			int x =int( mousex - intervalx ); // ������ ��������� �մϴ�.

			if( x < pParent->GetX()+_AllowRegion_Start )// �� ���ų� Ŀ�� �ϰ� 
			{
				x = int(pParent->GetX()+_AllowRegion_Start); 
			}
			else if( x+Bar_Width>=  pParent->GetX() + _AllowRegion_End )
			{
				x = pParent->GetX()+_AllowRegion_End - Bar_Width;
			}

			SetX( float(x) ); //m_BoundRect.top+

			// ����Ʈ�� ���������Ƿ� 
			// ��ũ���� ������ ��ŭ ������ �ش�. 
			// ����Ʈ�� �����ϼ� �ִ� ������ ��ü DESTRECT ���� ���Ʒ� ���� point_Height /2 �� ��ŭ �̴�.
			// �׸�ŭ�� �̵������� ������ ��ü�� ���� ���� ��ġ�� ��ü������ ������� ��ġ�� 2048 ������ �Ѵ�.
			// 2�� �¼� ���̹Ƿ� / ����� �Ҽ��� 5�ڸ� ������ ������ ������ ���� 
			// ���� ���콺 ��ġ???? �巹�� �ÿ��� 100������ ���� �̵���... 
			_Rate = PointRate;
			CGUI_MGR::_SendMessage( GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,PointRate );
			// �ڱ� �ְ� �θ� �ִٸ� ã�Ƽ� �̺�Ʈ ����
			IGUI_Object* pParent1 = GetSuperParent();
			if( pParent1 != NULL )
				CGUI_MGR::_SendMessage( pParent1->GetName(), GUIMSG_SCROLL_HORIZON_DRAG,(DWORD)this,PointRate );//m_Event
		}
		return true;

	case GUIMSG_MLDOWN:
		{
			POINT pt={g_MouseMGR.GetX(),g_MouseMGR.GetY() };
			// ���� �̵����� �����ȿ� �ִٸ� ��ư�� ������ ����
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
				// ���콺 ��ġ�� �̵���ư�� �߰���ġ �̸�
				// �߰��̻��� ��ġ�� �� ���̸�ŭ�� ��ġ�� �ȴ� ( ��� )
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
				CGUI_MGR::SetGuiFocus( m_Name );	// �θ𿡰� ��Ŀ���� �ش�.
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
				//	// �ڱ� �ְ� �θ� �ִٸ� ã�Ƽ� �̺�Ʈ ����
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
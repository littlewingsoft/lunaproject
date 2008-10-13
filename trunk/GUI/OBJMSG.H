
#pragma once 

enum eOM
{	
		OM_NONE,
		GUIMSG_CREATED, // GUI Object 가 생성될때 날라옴.. 리소스 생성 시점이 됨 
		GUIMSG_DESTROY, // 소멸 하기 바로 전에 날라옴 자신이 생성한 모든 리소스 소멸 시점이됨
		GUIMSG_ENABLE,	// GUI를 영구적으로 활성화 
		GUIMSG_DISABLE, // GUI를 영구적으로 비활성화 GUIMSG_ENABLE를 제외하곤 모든 메세지를 리턴함 
		GUIMSG_VK, // 키보드 메세지 메인 메세지 프로시져에서 들어옴
		GUIMSG_CLOSE,		// 창닫으라는 메세지 주로 부모가 자식들에게 전송함 

		GUIMSG_SCROLL_PREV, // 
		GUIMSG_SCROLL_NEXT,

		GUIMSG_MHOLD,	// 마우스가 이동안할때 한번 들어오는 메세지
		GUIMSG_MMOVE,	// 마우스가 이동시 들어오는 메세지 
		GUIMSG_DRAG	,	// 클릭후 이동시 들어오는 메세지 
		GUIMSG_MOVEWND, // 창을 움직일때의 메세지. 부모창에게 보내면 부모창은 자기 자식창에게 전달함. 

		GUIMSG_MLDOWN,			GUIMSG_MRDOWN,	
		GUIMSG_MLUP	,			GUIMSG_MRUP,	
		GUIMSG_MLDOUBLICK,		GUIMSG_MRDOUBLICK,	


		//GUIMSG_BUTTON_DOWN,	GUIMSG_BUTTON_UP,	// 버튼이 눌릴때 떼일떼 또는 이벤트가 발생할때 
		GUIMSG_BUTTON_EVENT,

		GUIMSG_SCROLL_VERTICAL_DRAG, // 수직 dwParam1 =  1~100 까지 100단계
		GUIMSG_SCROLL_VERTICAL_UP,	GUIMSG_SCROLL_VERTICAL_DOWN,
		
		GUIMSG_SCROLL_HORIZON_DRAG,	// 수평 dwParam1 = 1~100까지 100 단계
		GUIMSG_SCROLL_HORIZON_LEFT,	GUIMSG_SCROLL_HORIZON_RIGHT,

		GUIMSG_FOCUSENTER,
		GUIMSG_FOCUSLEAVE,// GUI Define End
		
		GUIMSG_WINDOW_SMALL,
		GUIMSG_WINDOW_TOGGLE,
		GUIMSG_WINDOW_CLOSE,

		//GUIMSG_REFRESH_GUISETTING, // 테스트 할때만 쓰는것임둥. 가장최근에 읽어들인 GUI 설정 파일을 새로 읽어들임 
		//GUIMSG 끝

		OM_HOLD,		// 오브젝트 메세지 
		OM_MOVE	,
		OM_ATTK, 
		OM_DEADMOVE,
		OM_DEAD,

		
		// 아템 먹을시 효과들 
		OM_ITEM_FADEOUT, // 사라질때 모든 아이템이 공통적으로 쓸 효과 

		OM_ITEM_SLOWER,
		OM_ITEM_FASTER,
		OM_ITEM_POWER,
		OM_ITEM_FREEZEME,
		OM_ITEM_FREEZEALL,
		OM_ITEM_REVERSECONTROL,
		
		// 회복 관련 
		OM_NORMAL_SPEED,// 본래 속도로 돌아가라 
		OM_NORMAL_POWER, // 원래 파워로 돌아가라, 유닛의 파워를 원래대로 돌려라 
		OM_NORMAL_UNFREEZME,	// 나의 유닛이 얼음에서 풀려나는 메세지, 얼음에서 풀려나라 
		OM_NORMAL_UNFREEZALL,	// 나를 제외한 모든 유닛이 얼음에서 풀려나는 메세지, 
		OM_NORMAL_CONTROL, // 조종 반대에서 풀려나는 메세지 


		OM_FAIL, // 이거에 넘어가면 문제 있음 
};

struct JMMSG
{
	eOM		dwMsg;		//	메세지의 타입
	int		dwParam1;	//	파라미터 1,2
	int		dwParam2;
	JMMSG():dwMsg(OM_FAIL),dwParam1(0),dwParam2(0){}
};

typedef JMMSG GUIMSG;


struct OBJMSG : public JMMSG
{

	DWORD	SenderID;	//	보낸이의 아이디
	DWORD	ReceiverID;	//  받을이의 아이디 
	DWORD	SendTime;	//	이 메세지큐에들어간 시간
	OBJMSG():SenderID(0),ReceiverID(0),SendTime(0){}
};

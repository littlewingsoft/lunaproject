
#include <stdafx.h>
#include "Icommand.h"

#include "OBJMSG.h"
#include "share_protocol\Network_Define.h"
#include "Scene_Lobby.h"
//#include <windows.h>

ICommand::ICommand():m_pParentObject(NULL),m_pParentCommand( NULL)
{

	
}

ICommand::~ICommand()
{
	if( m_pParentCommand  )
	{
		delete m_pParentCommand ;
		m_pParentCommand = NULL;
	}
}

void ICommand::SetParent( IGUI_Object* pParent )
{
	assert( pParent!= NULL); 
	m_pParentObject = pParent; 
}


//------------------------------------------------- 여기부턴 콘크리트 

CCommand_CloseWindow::CCommand_CloseWindow()
{
	m_pParentObject = NULL;
	m_pParentCommand = NULL;
}

CCommand_CloseWindow::CCommand_CloseWindow( ICommand* pCommand )
{
	m_pParentCommand = pCommand;
}
CCommand_CloseWindow::~CCommand_CloseWindow()
{
}

void CCommand_CloseWindow::excute()
{
	if( m_pParentCommand != NULL )
	m_pParentCommand ->excute();

	assert( m_pParentObject != NULL );
	IGUI_Object* pParent = m_pParentObject->GetParent(); 
	// 자신을 소유한건 분명 버튼일것임.
	// 버튼의 부모(판넬)이  바로 이 커맨드를 날려줄 객체 임
	// 그러기 위해선 버튼의 부모의 부모가 NULL( 최상위객체) 라는 것을 증명하여야 하므로
	// 재귀적으로 계속 타고 올라가서 NULL 이 되면 루프를 빠져나오는..
	// 바로 자신(커맨드)를 소유한 버튼(부모) 의 최상위 ( 판넬등) 을 찾는것이 관건이다.
	while( NULL != pParent->GetParent() )
	{
		pParent = pParent->GetParent();
	}

	assert( pParent != NULL && "Don't Find ParentGUI" );
	OBJMSG msg;
	msg.dwMsg = GUIMSG_CLOSE;
	pParent->ProcMessage( msg );   // 부모에게 보내면 자식들부터 없애고
									// 최후엔 자신이 리스트에서뿅~
}

CCommand_MakeRoom::CCommand_MakeRoom()
{
	m_pParentObject = NULL;
	m_pParentCommand = NULL;
}
CCommand_MakeRoom::~CCommand_MakeRoom(){}
void CCommand_MakeRoom::excute()
{
	if( m_pParentCommand != NULL )
	m_pParentCommand ->excute();

	// 현재 부모창의 방이름과 암호를 가지고서 방을 생성한다.
	// 엔터키를 칠때는 현재 모달 포커스를 가진 것을 닫는다.
	// 엔터키를 칠때는 현재 모달창을 닫아 버리고 모달창은 널로 넣는다.
	bool bl = true;
	CGUI_Pannel* pMakeRoom=  GUI_CreateObj<CGUI_Pannel>( _T("방만들기창"), _T(".\\IMG\\LOBBY\\MAKEROOM.ani"),276-28,40-28,-1,-1 ); 
	pMakeRoom->SetModal(true);
	CGUI_MGR::AddGUI( pMakeRoom ); 
	

	
	CGUI_Edit* pEditRoomPass = GUI_CreateObj<CGUI_Edit>( _T("방암호입력창"), _T(".\\IMG\\etc\\WhiteFade.ani"),70,133,170,18 );
	pEditRoomPass->SetAttirb( CGUI_Edit::PASSWD );
	pEditRoomPass->SetLimitStr( MAX_ROOM_PASSWORD -1 );
	pEditRoomPass->SetBoundRect(70,133,70+170,133+18); 
	pMakeRoom ->AddChild( pEditRoomPass );

	
	CGUI_Edit* pEditRoomName =GUI_CreateObj<CGUI_Edit>( _T("방이름입력창"),_T(".\\IMG\\etc\\WhiteFade.ani"),70,85,170,18 );
	pEditRoomName->SetAttirb( CGUI_Edit::NORMAL );
	pEditRoomName->SetLimitStr( MAX_ROOM_NAME -1 );
	pEditRoomName->SetString( _T("우리 함께해요~"));
	pEditRoomName->SetBoundRect(70,85,70+170,85+18); 
	pMakeRoom->AddChild( pEditRoomName );

	CGUI_Button* pButtonMakeRoomOk =GUI_CreateObj<CGUI_Button>( _T("방만들기창확인"),_T(".\\IMG\\GUI\\Button_RedOk.Ani"),30,177,-1,-1 );
	pMakeRoom ->AddChild( pButtonMakeRoomOk  );
	CGUI_Button* pButtonMakeRoomCancel = GUI_CreateObj<CGUI_Button>(_T("방만들기창취소"),_T(".\\IMG\\GUI\\Button_Cancel.Ani"),146,177,-1,-1);
	pMakeRoom ->AddChild( pButtonMakeRoomCancel ); 

	CGUI_MGR::SetGuiFocus( _T("방이름입력창") );
	OBJMSG msg;
	msg.dwMsg = GUIMSG_FOCUSENTER;
	pEditRoomName->ProcMessage( msg );
}

void CCommand_PasswordCheck::excute() // 창닫기를 할때는 이걸먼저 한뒤 패스워드 체크를 합니다.
{
	if( m_pParentCommand != NULL )
		m_pParentCommand ->excute();

	CGUI_MGR::FindGui( _T("암호입력") );
}

//------------------------------------------------

CCommand_GAMEEXIT::CCommand_GAMEEXIT()
{}
CCommand_GAMEEXIT::	~CCommand_GAMEEXIT()
{}
void CCommand_GAMEEXIT::excute()
{
	CSCENE::SetExitGAME( TRUE );
}

//-------------------------------------------------


CCommand_GOTOLOBBY::CCommand_GOTOLOBBY()
{}
CCommand_GOTOLOBBY::~CCommand_GOTOLOBBY()
{}

void CCommand_GOTOLOBBY::excute()
{
	CSCENE::SetNextScene( &g_LobbyScene );
}



//호스트와의 접속을 취소 합니다-------------------------
CCommand_Cancel_ConnectingToHost::CCommand_Cancel_ConnectingToHost()
{}
CCommand_Cancel_ConnectingToHost::~CCommand_Cancel_ConnectingToHost()
{}
void CCommand_Cancel_ConnectingToHost::excute()
{
//	CSCENE::m_GameSocket.Close();
}


CCommand_LOGIN_PASSFOCUS::CCommand_LOGIN_PASSFOCUS()
{}
CCommand_LOGIN_PASSFOCUS::~CCommand_LOGIN_PASSFOCUS()
{}
void CCommand_LOGIN_PASSFOCUS::excute()
{
	CGUI_Edit* pTmp =(CGUI_Edit*) CGUI_MGR::FindGui( _T("비번입력창") );
	if( pTmp != NULL )
	{
		CGUI_MGR::SetGuiFocus( pTmp->GetName() );
		pTmp->ClearString();
	}
}

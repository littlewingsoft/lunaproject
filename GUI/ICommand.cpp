
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


//------------------------------------------------- ������� ��ũ��Ʈ 

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
	// �ڽ��� �����Ѱ� �и� ��ư�ϰ���.
	// ��ư�� �θ�(�ǳ�)��  �ٷ� �� Ŀ�ǵ带 ������ ��ü ��
	// �׷��� ���ؼ� ��ư�� �θ��� �θ� NULL( �ֻ�����ü) ��� ���� �����Ͽ��� �ϹǷ�
	// ��������� ��� Ÿ�� �ö󰡼� NULL �� �Ǹ� ������ ����������..
	// �ٷ� �ڽ�(Ŀ�ǵ�)�� ������ ��ư(�θ�) �� �ֻ��� ( �ǳڵ�) �� ã�°��� �����̴�.
	while( NULL != pParent->GetParent() )
	{
		pParent = pParent->GetParent();
	}

	assert( pParent != NULL && "Don't Find ParentGUI" );
	OBJMSG msg;
	msg.dwMsg = GUIMSG_CLOSE;
	pParent->ProcMessage( msg );   // �θ𿡰� ������ �ڽĵ���� ���ְ�
									// ���Ŀ� �ڽ��� ����Ʈ������~
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

	// ���� �θ�â�� ���̸��� ��ȣ�� ������ ���� �����Ѵ�.
	// ����Ű�� ĥ���� ���� ��� ��Ŀ���� ���� ���� �ݴ´�.
	// ����Ű�� ĥ���� ���� ���â�� �ݾ� ������ ���â�� �η� �ִ´�.
	bool bl = true;
	CGUI_Pannel* pMakeRoom=  GUI_CreateObj<CGUI_Pannel>( _T("�游���â"), _T(".\\IMG\\LOBBY\\MAKEROOM.ani"),276-28,40-28,-1,-1 ); 
	pMakeRoom->SetModal(true);
	CGUI_MGR::AddGUI( pMakeRoom ); 
	

	
	CGUI_Edit* pEditRoomPass = GUI_CreateObj<CGUI_Edit>( _T("���ȣ�Է�â"), _T(".\\IMG\\etc\\WhiteFade.ani"),70,133,170,18 );
	pEditRoomPass->SetAttirb( CGUI_Edit::PASSWD );
	pEditRoomPass->SetLimitStr( MAX_ROOM_PASSWORD -1 );
	pEditRoomPass->SetBoundRect(70,133,70+170,133+18); 
	pMakeRoom ->AddChild( pEditRoomPass );

	
	CGUI_Edit* pEditRoomName =GUI_CreateObj<CGUI_Edit>( _T("���̸��Է�â"),_T(".\\IMG\\etc\\WhiteFade.ani"),70,85,170,18 );
	pEditRoomName->SetAttirb( CGUI_Edit::NORMAL );
	pEditRoomName->SetLimitStr( MAX_ROOM_NAME -1 );
	pEditRoomName->SetString( _T("�츮 �Բ��ؿ�~"));
	pEditRoomName->SetBoundRect(70,85,70+170,85+18); 
	pMakeRoom->AddChild( pEditRoomName );

	CGUI_Button* pButtonMakeRoomOk =GUI_CreateObj<CGUI_Button>( _T("�游���âȮ��"),_T(".\\IMG\\GUI\\Button_RedOk.Ani"),30,177,-1,-1 );
	pMakeRoom ->AddChild( pButtonMakeRoomOk  );
	CGUI_Button* pButtonMakeRoomCancel = GUI_CreateObj<CGUI_Button>(_T("�游���â���"),_T(".\\IMG\\GUI\\Button_Cancel.Ani"),146,177,-1,-1);
	pMakeRoom ->AddChild( pButtonMakeRoomCancel ); 

	CGUI_MGR::SetGuiFocus( _T("���̸��Է�â") );
	OBJMSG msg;
	msg.dwMsg = GUIMSG_FOCUSENTER;
	pEditRoomName->ProcMessage( msg );
}

void CCommand_PasswordCheck::excute() // â�ݱ⸦ �Ҷ��� �̰ɸ��� �ѵ� �н����� üũ�� �մϴ�.
{
	if( m_pParentCommand != NULL )
		m_pParentCommand ->excute();

	CGUI_MGR::FindGui( _T("��ȣ�Է�") );
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



//ȣ��Ʈ���� ������ ��� �մϴ�-------------------------
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
	CGUI_Edit* pTmp =(CGUI_Edit*) CGUI_MGR::FindGui( _T("����Է�â") );
	if( pTmp != NULL )
	{
		CGUI_MGR::SetGuiFocus( pTmp->GetName() );
		pTmp->ClearString();
	}
}

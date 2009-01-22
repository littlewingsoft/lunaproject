// DiffDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DiffDlg.h"


CFileDropListCtrl	g_kList ;
CDiffDlg* g_pkMainDlg= 0;
CRITICAL_SECTION cs;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDiffDlg 대화 상자

CDiffDlg::CDiffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiffDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pkMainDlg = this;
}

void CDiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//g_kList = new CFileDropListCtrl();

	DDX_Control(pDX, IDC_LIST3, g_kList);//
}

BEGIN_MESSAGE_MAP(CDiffDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDiffDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CDiffDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDiffDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDiffDlg::OnBnClickedButton4)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDiffDlg 메시지 처리기

BOOL CDiffDlg::OnInitDialog()
{
	InitializeCriticalSection( &cs );
	m_bCancelThread = FALSE;
	m_bDestroy = FALSE;
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//CListCtrl* aa= (CListCtrl*)GetDlgItem( IDC_LIST3 );

	//int nIndex = aa->InsertItem( LVIF_TEXT|LVIF_STATE, 0, L"aa",0,LVIS_SELECTED,0,0); 
	//aa->SetItemText(nIndex, 1, L"bbbfffffffffffffffffffffffffffffffffffffffffffffff" ); 
	//aa->SetItemText(nIndex, 2, L"181818181818" ); 
//	aa->DeleteAllItems();
	//DragAcceptFiles();

	CFileDropListCtrl::DROPLISTMODE dropMode;
	dropMode.iMask =	CFileDropListCtrl::DL_ACCEPT_FILES | 
						CFileDropListCtrl::DL_ACCEPT_FOLDERS | 
						CFileDropListCtrl::DL_ALLOW_DUPLICATES;
	g_kList.SetDropMode(dropMode);

	g_kList.InsertColumn(0, L"FileName",0,256,-1 );
	g_kList.InsertColumn(1, L"CRC32 CheckSum",0,120,-1 );
	g_kList.InsertColumn(2, L"FullPath",0,420,-1 );

	g_pkMainDlg->SetDlgItemInt( IDC_EDIT1, 0 );	
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDiffDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDiffDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDiffDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDiffDlg::OnBnClickedButton1()
{
	CFileDialog fileDlg( TRUE);

 int iReturn = fileDlg.DoModal();

}



void CDiffDlg::OnDestroy()
{
//	SuspendThread(g_hThread ); // thread 멈춤.

//	ResumeThread( g_hThread );//다시 시작시킨다.
//	WaitForSingleObject( g_kList.m_hEvent, INFINITE ); //메인스레드(현재 창)은 대기
	//스레드에서 SetEvent 해줄때까지 기다린다.

//	EnterCriticalSection( &cs );
//	LeaveCriticalSection( &cs );
	m_bCancelThread = TRUE; // 플레그 설정한다.	
	CDialog::OnDestroy();

	DeleteCriticalSection(&cs );
}

void CDiffDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_hThread =  0;
	m_bCancelThread = true;
	g_kList.DeleteAllItems();
	g_pkMainDlg->SetDlgItemInt( IDC_EDIT1, 0 );	
	g_pkMainDlg->SetDlgItemInt( IDC_EDIT2, 0 );	
	CProgressCtrl* pkPro = (CProgressCtrl*)g_pkMainDlg->GetDlgItem( IDC_PROGRESS1 );
	pkPro->SetPos(0);
}


void CDiffDlg::OnBnClickedButton3()
{
}

void CDiffDlg::OnBnClickedButton4()
{
}

void CDiffDlg::OnClose()
{
	if( g_hThread == 0 )
	{	// 쓰레드가 돌지 않는다면 바로 종료.
		PostMessage(WM_CLOSE, 0, 0);
		CDialog::OnClose();
	}else
	{
		// 쓰레드가 돌고있다면 이 문맥으로 넘어감.
		int ret = MessageBox( _T("Until Processed Are you Have Do it now?"), _T("NOTICE") , MB_YESNO );

		// 지금끝낼꺼냐고 
		// 물으면 스레드가 동작중인지 체크후 동작중이라면비정상 종료후 Cialog::OnClose 하게끔 만든다.
		if( ret == IDYES )
		{
			m_bDestroy = true;
			m_bCancelThread = true;
		}
		else
		{
			m_bDestroy = false;
			m_bCancelThread = false;
		}
	}
	
}

BOOL CDiffDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//switch( pMsg->message )
	//{
	//case WM_DESTROY:
	//case WM_CLOSE:
	//	return TRUE;
	//	//OnDestroy();
	//}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDiffDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch( nIDEvent )
	{
	case WM_CLOSE:
		OnClose();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

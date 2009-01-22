// DiffDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "DiffDlg.h"


CFileDropListCtrl	g_kList ;
CDiffDlg* g_pkMainDlg= 0;
CRITICAL_SECTION cs;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CDiffDlg ��ȭ ����

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


// CDiffDlg �޽��� ó����

BOOL CDiffDlg::OnInitDialog()
{
	InitializeCriticalSection( &cs );
	m_bCancelThread = FALSE;
	m_bDestroy = FALSE;
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDiffDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
//	SuspendThread(g_hThread ); // thread ����.

//	ResumeThread( g_hThread );//�ٽ� ���۽�Ų��.
//	WaitForSingleObject( g_kList.m_hEvent, INFINITE ); //���ν�����(���� â)�� ���
	//�����忡�� SetEvent ���ٶ����� ��ٸ���.

//	EnterCriticalSection( &cs );
//	LeaveCriticalSection( &cs );
	m_bCancelThread = TRUE; // �÷��� �����Ѵ�.	
	CDialog::OnDestroy();

	DeleteCriticalSection(&cs );
}

void CDiffDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	{	// �����尡 ���� �ʴ´ٸ� �ٷ� ����.
		PostMessage(WM_CLOSE, 0, 0);
		CDialog::OnClose();
	}else
	{
		// �����尡 �����ִٸ� �� �������� �Ѿ.
		int ret = MessageBox( _T("Until Processed Are you Have Do it now?"), _T("NOTICE") , MB_YESNO );

		// ���ݳ������İ� 
		// ������ �����尡 ���������� üũ�� �������̶������� ������ Cialog::OnClose �ϰԲ� �����.
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch( nIDEvent )
	{
	case WM_CLOSE:
		OnClose();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

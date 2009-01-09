// DiffDlg.cpp : ���� ����
//

#include "stdafx.h"


CFileDropListCtrl	g_pkList ;
CDiffDlg* g_pkMainDlg= 0;
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
	//g_pkList = new CFileDropListCtrl();

	DDX_Control(pDX, IDC_LIST3, g_pkList);//
}

BEGIN_MESSAGE_MAP(CDiffDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDiffDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CDiffDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDiffDlg �޽��� ó����

BOOL CDiffDlg::OnInitDialog()
{
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
	g_pkList.SetDropMode(dropMode);

	g_pkList.InsertColumn(0, L"FileName",0,256,-1 );
	g_pkList.InsertColumn(1, L"CRC32 CheckSum",0,120,-1 );
	g_pkList.InsertColumn(2, L"FullPath",0,420,-1 );

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
	CDialog::OnDestroy();

//	delete g_pkList;
//	g_pkList = 0;
}

void CDiffDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	g_pkList.DeleteAllItems();
	g_pkMainDlg->SetDlgItemInt( IDC_EDIT1, 0 );	
}

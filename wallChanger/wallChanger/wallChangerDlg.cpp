// wallChangerDlg.cpp : ���� ����
//

#include "stdafx.h"

#include "Option.h"
#include "ImgTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Option* g_Option =0;
ImgTree* g_ImgTree=0 ;
TiXmlDocument g_servXml;

CwallChangerDlg* g_pkMainDlg = 0;
CwallChangerDlg::CwallChangerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CwallChangerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pkMainDlg = this;
}


void CwallChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CwallChangerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CwallChangerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CwallChangerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()



void CwallChangerDlg::Print_Now( const TCHAR* str )
{
	CListBox* pkBox = (CListBox*)GetDlgItem( IDC_LIST2);
	int ret = pkBox->GetCurSel();
	CString cstr;
	pkBox->GetText( -1, cstr );
	cstr = str;
	//pkBox->SetCurSel(ret);
}

void CwallChangerDlg::Print( const TCHAR* str )
{
	CListBox* pkBox = (CListBox*)GetDlgItem( IDC_LIST2);
	int ret = pkBox->InsertString( -1, str );
	pkBox->SetCurSel(ret);
}
void CwallChangerDlg::PresetPaper()
{
	if( g_ImgTree->urlTable.empty() )
	{
		//g_ImgTree->MakeUrlTable();
		return;
	}

	size_t index = rand() % g_ImgTree->urlTable.size();
	
	TCHAR szUniCode[256]={0,};
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, g_ImgTree->urlTable[index].c_str(), g_ImgTree->urlTable[index].size(), szUniCode, 256);

	CString URL =  szUniCode ; //_T("http://pds105.cafe.daum.net/image/3/cafe/2008/12/16/13/53/4947344dc5dd4");

	
	HRESULT hr = ::URLDownloadToFile( NULL, URL, _T("c:\\img.jpg"), 0, 0);
	Print( _T("[INFO] �̹��� �ٿ�ε���") );
}

void CwallChangerDlg::ChangeWallPaper()
{
	
	// servXml ���� ������ ���ǿ� ���� ���� �˻��ع���.
	PresetPaper();

	// Get a pointer to the IActiveDesktop interface.
	// We use this to set the wallpaper when not using .BMP files.
	IActiveDesktop* pIActiveDesktop = NULL;
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, 
		IID_IActiveDesktop, (void**) &pIActiveDesktop);

	if (FAILED(hr))
		pIActiveDesktop = NULL;

	CString cstrTileWallpaper,cstrWallpaperStyle ;
		//if (iPosition == 0)
		//{
		//		// Tiled
		//		cstrTileWallpaper = "1";
		//		cstrWallpaperStyle = "1";
		//	}
		//	else if (iPosition == 1)
		//	{
		//		// Center
				cstrTileWallpaper = _T("0");
				cstrWallpaperStyle = _T("1");
		//	}
		//	else
		//	{
		//		// Stretch
		//		cstrTileWallpaper = "0";
		//		cstrWallpaperStyle = "2";
		//	}
	if( pIActiveDesktop == 0 )
	{
			// Set registry values for the wallpaper position.
			Registry::SetRegString( _T("Control Panel\\Desktop"), _T("TileWallpaper"), cstrTileWallpaper);
			Registry::SetRegString( _T("Control Panel\\Desktop"), _T("WallpaperStyle"), cstrWallpaperStyle);

			// Change the wallpaper.
			BOOL ret = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)L"c:\\img.jpg", 
				SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);	
	}
	else
	{
		WALLPAPEROPT wpOptions;
		COMPONENTSOPT compOptions;

		compOptions.dwSize = sizeof(COMPONENTSOPT);
		compOptions.fActiveDesktop = TRUE;
		compOptions.fEnableComponents = TRUE;

		pIActiveDesktop->GetDesktopItemOptions(&compOptions,0);
		pIActiveDesktop->SetDesktopItemOptions(&compOptions, 0);

		// Set the wallpaper position.
		wpOptions.dwSize = sizeof(WALLPAPEROPT);
//		if (iPosition == 0)
//			wpOptions.dwStyle = WPSTYLE_TILE;
//		else if (iPosition == 1)
			wpOptions.dwStyle = WPSTYLE_CENTER;
//		else
//			wpOptions.dwStyle = WPSTYLE_STRETCH;
//		pIActiveDesktop->GetWallpaperOptions(&wpOptions, 0);
		pIActiveDesktop->SetWallpaperOptions(&wpOptions, 0);
//		wchar_t buff[256]={0,};
//		pIActiveDesktop->GetWallpaper( buff, 256,AD_GETWP_LAST_APPLIED );// AD_GETWP_IMAGE AD_GETWP_BMP 

		// Set the wallpaper image.
		pIActiveDesktop->SetWallpaper( _T("c:\\img.jpg"), 0) ;

		// Apply all changes.
		pIActiveDesktop->ApplyChanges(AD_APPLY_ALL);
		Print( _T("[INFO] ��漳�� �Ϸ�~!") );

		// Release the IActiveDesktop interface.
		pIActiveDesktop->Release();
		pIActiveDesktop = NULL;

	CoUninitialize();
	}

}

void CwallChangerDlg::SettingOption()
{
	//�α����ߴٸ� �ڱ� ������ xml ������ ����.
}

void CwallChangerDlg::DownLoadXml()
{
//	CString URL = _T("http://pds105.cafe.daum.net/image/3/cafe/2008/12/16/13/53/4947344dc5dd4");
//	HRESULT hr = ::URLDownloadToFile( NULL, URL, _T("wallpaperList.Xml"), 0, 0 );
	//xml �ٿ����.
	g_servXml.LoadFile( "wallpaperList.xml" );
	Print( _T("DownLoad Xml" ) );
	SettingOption();
	

}

void CwallChangerDlg::SetDelayTimer()
{
	KillTimer(100);
	if( g_Option )
	SetTimer( 100, g_Option->m_DelayTime * 1000 ,0 );
}

#include <algorithm>
#include "wallChangerDlg.h"
// CwallChangerDlg �޽��� ó����

BOOL CwallChangerDlg::OnInitDialog()
{
	std::auto_ptr<int> Apk(new int);

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
	DownLoadXml();

	{
	g_ImgTree = new ImgTree;
	g_ImgTree->Create( ImgTree::IDD, &m_Tab );
	m_Tab.AddTab( g_ImgTree, _T("MYFOLDER" ) );	
	}

	{
	g_Option = new Option;
	g_Option->Create( Option::IDD, &m_Tab );
	m_Tab.AddTab( g_Option, _T("Option" ) );
	}

	m_Tab.SetMouseOverColor( RGB(0xff,0x10,0x10));



	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}



void CwallChangerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CwallChangerDlg::OnPaint()
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
HCURSOR CwallChangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CwallChangerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch( nIDEvent )
	{
	case 100:
		{
			ChangeWallPaper();
			Print(_T("����ȭ�� �ٲ����") );
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}



void CwallChangerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if( g_Option )
	{
		delete g_Option;
		g_Option= 0;
	}
	
	if( g_ImgTree )
	{
		delete g_ImgTree;
		g_ImgTree=0;
	}

}


BOOL CwallChangerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	switch( pMsg ->message )
	{
	case WM_KEYDOWN:
		switch( pMsg->wParam )
		{
		case VK_RETURN:
			return TRUE;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

// stop
void CwallChangerDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(100);
}


// Play
void CwallChangerDlg::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( g_ImgTree )
	g_ImgTree->MakeUrlTable();

	SetDelayTimer();

}

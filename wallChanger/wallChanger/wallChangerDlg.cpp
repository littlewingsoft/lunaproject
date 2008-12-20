// wallChangerDlg.cpp : 구현 파일
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
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CwallChangerDlg::OnTcnSelchangeTab1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()




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
	Print( _T("[INFO] 이미지 다운로드중") );
}

void CwallChangerDlg::ChangeWallPaper()
{
	
	// servXml 에서 정해진 조건에 따라 마구 검색해버림.
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
		Print( _T("[INFO] 배경설정 완료~!") );

		// Release the IActiveDesktop interface.
		pIActiveDesktop->Release();
		pIActiveDesktop = NULL;

	CoUninitialize();
	}

}

void CwallChangerDlg::SettingOption()
{
//	CComboBox* pkCombo = (CComboBox*)GetDlgItem( IDC_COMBO2 );
//	pkCombo->SetCurSel(0);	
}

void CwallChangerDlg::DownLoadXml()
{
//	CString URL = _T("http://pds105.cafe.daum.net/image/3/cafe/2008/12/16/13/53/4947344dc5dd4");
//	HRESULT hr = ::URLDownloadToFile( NULL, URL, _T("wallpaperList.Xml"), 0, 0 );
	//xml 다운받음.
	g_servXml.LoadFile( "wallpaperList.xml" );
	SettingOption();
}

void CwallChangerDlg::SetDelayTimer()
{
	KillTimer( 100 );
	SetTimer( 100, g_Option->m_DelayTime*60*1000, 0 );
}

#include <algorithm>
// CwallChangerDlg 메시지 처리기

BOOL CwallChangerDlg::OnInitDialog()
{
	std::auto_ptr<int> Apk(new int);

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

//	CComboBox* pkCombo = (CComboBox*)GetDlgItem( IDC_COMBO3 );
//	pkCombo->SetCurSel(0);
	

	//SetDlgItemInt( IDC_EDIT1, 5 );
	
	
	//SaveCurrentWallPicture() ; // 현재 프로그램이 종료되면 원래 바탕화면으로 되돌리기 위해 저장시켜놓는다.

	DownLoadXml();
	ChangeWallPaper();
	SetDelayTimer();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CwallChangerDlg::OnPaint()
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
HCURSOR CwallChangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CwallChangerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch( nIDEvent )
	{
	case 100:
		{
			ChangeWallPaper();
			Print(_T("바탕화면 바뀌었다") );
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CwallChangerDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CwallChangerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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

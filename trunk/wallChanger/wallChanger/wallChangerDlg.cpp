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
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CwallChangerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CwallChangerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


//CString GetImageExt( CString& fName )
//{
//   Gdiplus::GdiplusStartupInput gdiplusStartupInput;
//   ULONG_PTR gdiplusToken;
//   Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//
//   Gdiplus::Image* img = ::new Gdiplus::Image( fName );
//   UINT imageHeight = img->GetHeight();
//   //printf("The height of the image is %u.\n", imageHeight);
//   Gdiplus::ImageType tp = img->GetType();
//
//   ::delete img;
//   Gdiplus::GdiplusShutdown(gdiplusToken);
//
//   CString ext= _T("jpg");
//   
//   if( tp == Gdiplus::ImageTypeMetafile )
//	ext = _T("gif");
//
//   return ext;
//}

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

CString g_ImgName;

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

	CString fName= _T("atmpimg.jpg");
	HRESULT hr = ::URLDownloadToFile( NULL, URL, fName, 0, 0);
	// 파일을 읽어서 타입을 알아내야 한다.
	//jpg,png,gif ?
//	CString ext = GetImageExt( fName );//_T("");//

	
	g_ImgName = fName;//+_T(".")+ext;
	//int ret = _trename( fName.GetString(), g_ImgName.GetString() );
	/*switch( ret )
	{
	case EACCES :
		break;
	case EINVAL :
		break;
	case ENOENT :
		break;
	}*/
	
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
			BOOL ret = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, ((PVOID)(g_ImgName.GetBuffer(128))),  //(void*) ((TCHAR*)
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
		pIActiveDesktop->SetWallpaper( g_ImgName, 0) ;

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
	//로그인했다면 자기 계정의 xml 파일을 얻어옴.
}

void CwallChangerDlg::DownLoadXml()
{
//	CString URL = _T("http://pds105.cafe.daum.net/image/3/cafe/2008/12/16/13/53/4947344dc5dd4");
//	HRESULT hr = ::URLDownloadToFile( NULL, URL, _T("wallpaperList.Xml"), 0, 0 );
	//xml 다운받음.
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


BOOL CwallChangerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(100);
}


// Play
void CwallChangerDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( g_ImgTree )
	g_ImgTree->MakeUrlTable();

	SetDelayTimer();

}


typedef BOOL (CALLBACK* REQUESTCALLBACK)(LPCTSTR, UINT, UINT);

BOOL UploadFile(CString &Filename, REQUESTCALLBACK pcbfn /* = NULL */) 
{
	//http://jungmoona.gotdns.com/
	#define BASE_UPLOAD_PATH "img" 

		CString m_Host( "http://jungmoona.gotdns.com/" );
      CInternetSession m_Session;
      CHttpConnection *pServer = NULL;
      CHttpFile *pFile = NULL;
      CString BaseFilename;
      DWORD dwTotalLen;
      LPVOID lpBuf[1024];
      CString m_ServerPath;
      CFileException E;
      CStdioFile UpFile;
      CString str;

      UINT bytesRead = 0;
      UINT totalRead = 0;

      TRACE0("UploadFile()\n");
            
      //m_Response.RemoveAll();
      //ResponseIndex = 0;
      
      if(!UpFile.Open(Filename, CFile::modeRead | CFile::typeBinary | CFile::shareExclusive, &E)) {
            if(E.m_cause == CFileException::fileNotFound) {
                  TRACE0("Upload file cannot be found\n");
                  str.Format(_T("Error:%d:The file to upload could not be found"),-1);
                  //m_Response.Add(str);   
            }
            else {
                  TRACE0("Upload file could not be opened\n");
                  str.Format(_T("Error:%d:The file to upload could not be opened"),-1);
                  //m_Response.Add(str);   
            }
            return FALSE;
     }
      dwTotalLen = (DWORD)UpFile.GetLength();
      if(dwTotalLen == 0) {
            TRACE0("Upload file is empty\n");
            str.Format(_T("Error:%d:The file to upload is empty"),-1);
            return FALSE;
      }

      BaseFilename = Filename;
      if(Filename.Find('\\') > -1) {
            BaseFilename = Filename.Right(Filename.GetLength() - (Filename.ReverseFind('\\') + 1));
      }
      m_ServerPath.Format(_T("%s/%s"), BASE_UPLOAD_PATH, BaseFilename);

      try {

            pServer = m_Session.GetHttpConnection(m_Host);
            pFile = pServer->OpenRequest( CHttpConnection::HTTP_VERB_PUT, m_ServerPath );//, USER_AGENT, 1, NULL, HTTP_VERSION, INTERNET_FLAG_NO_CACHE_WRITE); 
            pFile->SendRequestEx(dwTotalLen);

            do {
                  
                  if((bytesRead = UpFile.Read(lpBuf, 1023))) {
                        pFile->Write(lpBuf, bytesRead);
                        totalRead += bytesRead;

                        if (pcbfn)
                        {
                              if(!pcbfn(Filename, totalRead, dwTotalLen)) {
                                    UpFile.Close();
                                    
                                    pFile->EndRequest();
                                    pFile->Close();
                                    pServer->Close();
                                                      
                                    if(pFile != NULL) delete pFile;
                                    if(pServer != NULL) delete pServer;
                                                      
                                    m_Session.Close();
                                                      
                                    str.Format(_T("Error:%d:Upload terminated by user"), -1); //ERROR_USER_TERMINATION
                                    return FALSE;                                          
                              }
                        }
                  }
            } while(bytesRead > 0);
      }
      catch(CInternetException *e) {
            TCHAR szErr[1024];
            e->GetErrorMessage(szErr, 1024);
                  
            str.Format(_T("Error:%d:%s"), -1, szErr); //ERROR_FILE_UPLOAD
            //m_Response.Add(str);
            TRACE1("Upload Error: %s\n", str);
            e->Delete();
            return FALSE;
      }
      
      pFile->EndRequest();
      pFile->Close();
      pServer->Close();

      if (pFile != NULL) delete pFile;
      if (pServer != NULL) delete pServer;
      
      m_Session.Close();
      
      if(totalRead != dwTotalLen) {
            str.Format(_T("Error:%d:Write error on upload"), -1);//ERROR_FILE_UPLOAD
            //m_Response.Add(str);
            return FALSE;
      }
      
      UrlEncode(BaseFilename);
      m_Query.Format(_T("action=verifyupload&filename=%s&filesize=%u"), BaseFilename, dwTotalLen);
      
      return Request(m_Query, VERIFY_UPLOAD_ACTION);
}
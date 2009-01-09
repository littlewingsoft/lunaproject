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
	// ������ �о Ÿ���� �˾Ƴ��� �Ѵ�.
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
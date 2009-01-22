/////////////////////////////////////////////////////////////////////////////
//
//	CFileFileDropListCtrl - Enhanced CListCtrl that accepts and
//							filters dropped files/folders.
//
//	Jan 2000, Stuart Carter, stuart.carter@hotmail.com
//
//	You're free to use, modify and distribute this code
//	as long as credit is given...
//
//		Thanks to:
//		Handling of droppped files modified from:
//			CDropEdit, 1997 Chris Losinger
//			http://www.codeguru.com/editctrl/filedragedit.shtml
//
//		Shortcut expansion code modified from :
//			CShortcut, 1996 Rob Warner
//
//
//	History:
//
//	Version	Date		Author				Description
//	-------	----------	-------------------	--------------------------------
//	1.0		20-01-2000	Stuart Carter		Initial
//

#include "stdafx.h"
#include "FileDropListCtrl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <afxdisp.h>	// OLE stuff
#include <shlwapi.h>	// Shell functions (PathFindExtension() in this case)
#include <afxpriv.h>	// ANSI to/from Unicode conversion macros

#include <process.h>

#include "DiffDlg.h"
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

HANDLE g_hThread = 0;
HDROP g_hdRop = 0;

std::set<int> duplicateSet;
/////////////////////////////////////////////////////////////////////////////

CFileDropListCtrl::CFileDropListCtrl()
{
	CloseHandle( m_hEvent );
	m_hEvent = CreateEvent( 0, TRUE, FALSE, L"event" );
	ResetEvent( m_hEvent );
	//
	// Default drop mode
	//
	m_dropMode.iMask = DL_ACCEPT_FILES | DL_ACCEPT_FOLDERS;
	m_dropMode.pfnCallback = NULL;

	//
	// Initialize OLE libraries
	//
	m_bMustUninitOLE = FALSE;
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm) // TRUE if OleUninitialize needs to be called
	{
		HRESULT hr = ::OleInitialize(NULL);
		if (FAILED(hr))
			// Or something of your choosing...
			AfxMessageBox(_T("OLE initialization failed.\n\nMake sure that the OLE libraries are the correct version."));
		else
			m_bMustUninitOLE = TRUE;
	}
}

CFileDropListCtrl::~CFileDropListCtrl()
{
	CloseHandle( m_hEvent );

	if(m_bMustUninitOLE)
	{
		::OleUninitialize();
	}
}

BEGIN_MESSAGE_MAP(CFileDropListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CFileDropListCtrl)
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////

int CFileDropListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//
	// Register ourselves as a drop target for files
	//
	DragAcceptFiles(TRUE);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
//	PUBLIC SetDropMode()
//
//	Specify how the control will react to dropped files/folders.
//
//	Return value:
//		FALSE:	the structure was not populated correctly, 
//				the default settings will be used.
//		TRUE:	changes to the drop mode accepted
//
//
//

BOOL CFileDropListCtrl::SetDropMode(const CFileDropListCtrl::DROPLISTMODE& dropMode)
{
	//
	// If they want to use a callback, ensure they also
	// specified the address of a function...
	//
	if(dropMode.iMask & DL_USE_CALLBACK && dropMode.pfnCallback == NULL)
	{
		// Must specify a function if using DL_USE_CALLBACK flag
		ASSERT(FALSE);
		return FALSE;
	}

	m_dropMode = dropMode;
	return TRUE;
}



void CFileDropListCtrl::RecursiveFind(const CString& rootPath )
{
	if( g_pkMainDlg->m_bDestroy )
	{
		g_pkMainDlg->SetDlgItemInt(IDC_EDIT1, 0 );
		DeleteAllItems();
		g_hThread=0;
		g_pkMainDlg->OnClose();
		return ;
	}

	// 현재 지정된currPath 의 모든 파일과 폴더를 돌아요.
	WIN32_FIND_DATA lpFindFileData;
	HANDLE hFirstFile = FindFirstFile( rootPath, &lpFindFileData );
	//cassert( hFirstFile != INVALID_HANDLE_VALUE );

	while( 1 )
	{
		if( g_pkMainDlg->m_bCancelThread )
		{
			g_pkMainDlg->SetDlgItemInt(IDC_EDIT1, 0 );
			DeleteAllItems();
			break;
		}


		size_t offset = rootPath.Find( L"*" );
		CString fullpath  = rootPath.Left( offset );
		CString name = lpFindFileData.cFileName;
		fullpath += name;

		if( name == L"." || name == L".." || name == L".svn" )
			goto findNext;

		if( lpFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) // directory
		{
			CString searchPath = fullpath + L"\\*";
			RecursiveFind( searchPath );
		}
		else //file
		{
			InsertRecord( fullpath );
		}

findNext:
		if( FindNextFile( hFirstFile, &lpFindFileData ) == FALSE )
		{
			break;
		}

	}

	if( g_pkMainDlg->m_bDestroy )
	{
		g_hThread=0;
		g_pkMainDlg->OnClose();
	}

}

void CFileDropListCtrl::InsertRecord( CString& csPathname )
{
	wchar_t szDrv[16];
	wchar_t szDir[256];
	wchar_t szFileName[64];
	wchar_t szExt[16];
	_wsplitpath_s( csPathname.GetBuffer(),szDrv,16,szDir,256,szFileName,64,szExt,16);
	CString csExt = szExt;
	csExt.MakeLower();
	if( csExt != L".mp3" )
		return;

	CString name = szFileName;
	name += szExt;
	int index = InsertItem( 0,name );
	CString fullDir = szDrv;
	fullDir += "/" ;
	fullDir +=szDir;
	SetItemText( index, 1, L"CrC32" );
	SetItemText( index, 2, csPathname );
}

unsigned int __stdcall ThreadFunc(void *ArgList) 
{
	CFileDropListCtrl* pkList = (CFileDropListCtrl*) ArgList;

	//
	// Get the number of pathnames (files or folders) dropped
	//
	UINT nNumFilesDropped = DragQueryFile(g_hdRop, 0xFFFFFFFF, NULL, 0);

	//
	// Iterate through the pathnames and process each one
	//
	TCHAR szFilename[MAX_PATH + 1];
	CString csPathname;
	CString csExpandedFilename;

	g_pkMainDlg->m_bCancelThread = false;

	for (UINT nFile = 0 ; nFile < nNumFilesDropped; nFile++)
	{
		//	WaitForSingleObject( pkList->m_hEvent , INFINITE );
		//	EnterCriticalSection( &cs );

		if( g_pkMainDlg->m_bCancelThread )
		{
			pkList->DeleteAllItems();
			break;
		}

		DragQueryFile( g_hdRop, nFile, szFilename, MAX_PATH + 1);

		csPathname = szFilename;
		//csExpandedFilename = pkList->ExpandShortcut(csPathname);
		//if(!csExpandedFilename.IsEmpty())
		//{
		//	csPathname = csExpandedFilename;
		//}

		UINT iPathType = 0;
		if( pkList->ValidatePathname(csPathname, iPathType))
		{
			switch( iPathType  )
			{
			case CFileDropListCtrl::DL_FOLDER_TYPE:
				pkList->RecursiveFind( csPathname +L"\\" + L"*" );
				break;
			case CFileDropListCtrl::DL_FILE_TYPE:
				{
					pkList->InsertRecord( csPathname );
				}
				break;
			}
			//
			// By default, we insert the filename into the list
			// ourselves, but if our parent wants to do something flashy
			// with it (maybe get the filesize and insert that as an extra
			// column), they will have installed a callback for each
			// droppped item
			//
			if(pkList->m_dropMode.iMask & CFileDropListCtrl::DL_USE_CALLBACK)
			{
				//
				// Let them know about this list control and the item
				// droppped onto it
				//
				if(pkList->m_dropMode.pfnCallback)
					pkList->m_dropMode.pfnCallback(pkList, csPathname, iPathType);
			}
			else
			{

				//InsertPathname(csPathname);

			}
		}


	}



	if( g_pkMainDlg->m_bDestroy )
	{
		g_pkMainDlg->SetDlgItemInt(IDC_EDIT1, 0 );
		pkList->DeleteAllItems();

		g_pkMainDlg->OnClose();


		return -1;
	}


	DragFinish(g_hdRop);
	g_pkMainDlg->SetDlgItemInt( IDC_EDIT1, pkList->GetItemCount() );



	pkList->ReCalcCrC();

	g_pkMainDlg->SetDlgItemInt( IDC_EDIT2, pkList->GetItemCount() - duplicateSet.size() );
	g_hThread=0;
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
//
//	Handle the WM_DROPFILES message
//

void CFileDropListCtrl::OnDropFiles(HDROP dropInfo)
{
	g_hdRop = dropInfo;

	// Thread 를 돌려야 함.
	{
		ResetEvent( m_hEvent );
		unsigned int ThreadId=0;
		g_hThread = (HANDLE)_beginthreadex( NULL, 0, ThreadFunc, (void*)this, 0, &ThreadId);
		if (g_hThread == 0) 
		{
			return ;
		}

		// crc 를 가지고 충돌 되는 것만 재계산.
	}




}



void CFileDropListCtrl::ReCalcCrC()
{
	CProgressCtrl* pkPro = (CProgressCtrl*)g_pkMainDlg->GetDlgItem( IDC_PROGRESS1 );
	int cnt = this->GetItemCount();
	pkPro->SetRange(0,cnt);
	bool bCloseMainDlg = false;

	for( int n=0; n< cnt; n++)
	{
		if( g_pkMainDlg->m_bCancelThread )
		{
			break;
		}
		//event 를 체크해봐서 false 라면 아예 break 하게 해버림.
		// 한번 도는 동안은 Lock!!!
		CString csFullPath = this->GetItemText( n,2 );	
		DWORD crc = FileTocrc32( csFullPath.GetBuffer() );
		duplicateSet.insert(crc);
		CString tmp;
		tmp.Format( L"0x%08x" , crc );
		this->SetItemText( n,1, tmp );
		pkPro->SetPos( n );

		g_pkMainDlg->SetDlgItemInt( IDC_EDIT2, GetItemCount() - duplicateSet.size() );

		SetHotItem(n);  // 0번째 아이템을 선택한다.
		EnsureVisible(n, FALSE);  //0번째 아이템을 확실하게 보여준다

	}

	if( g_pkMainDlg->m_bCancelThread )
	{
		pkPro->SetPos(0);
		g_pkMainDlg->SetDlgItemInt(IDC_EDIT1, 0 );
		DeleteAllItems();
	}

	if( g_pkMainDlg->m_bDestroy )
	{
		g_hThread=0;
		g_pkMainDlg->OnClose();
	}
}

//////////////////////////////////////////////////////////////////
//
//	ExpandShortcut()
//
//	Uses IShellLink to expand a shortcut.
//
//	Return value:
//		the expanded filename, or "" on error or if filename
//		wasn't a shortcut
//
//	Adapted from CShortcut, 1996 by Rob Warner
//	rhwarner@southeast.net
//	http://users.southeast.net/~rhwarner

CString CFileDropListCtrl::ExpandShortcut(CString& csFilename) const
{
	USES_CONVERSION;		// For T2COLE() below
	CString csExpandedFile;

	//
	// Make sure we have a path
	//
	if(csFilename.IsEmpty())
	{
		ASSERT(FALSE);
		return csExpandedFile;
	}

	//
	// Get a pointer to the IShellLink interface
	//
	HRESULT hr;
	IShellLink* pIShellLink;

	hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
		IID_IShellLink, (LPVOID*) &pIShellLink);

	if (SUCCEEDED(hr))
	{

		//
		// Get a pointer to the persist file interface
		//
		IPersistFile* pIPersistFile;
		hr = pIShellLink->QueryInterface(IID_IPersistFile, (LPVOID*) &pIPersistFile);

		if (SUCCEEDED(hr))
		{
			//
			// Load the shortcut and resolve the path
			//
			// IPersistFile::Load() expects a UNICODE string
			// so we're using the T2COLE macro for the conversion
			//
			// For more info, check out MFC Technical note TN059
			// (these macros are also supported in ATL and are
			// so much better than the ::MultiByteToWideChar() family)
			//
			hr = pIPersistFile->Load(T2COLE(csFilename), STGM_READ);

			if (SUCCEEDED(hr))
			{
				WIN32_FIND_DATA wfd;
				hr = pIShellLink->GetPath(csExpandedFile.GetBuffer(MAX_PATH),
					MAX_PATH,
					&wfd,
					SLGP_UNCPRIORITY);

				csExpandedFile.ReleaseBuffer(-1);
			}
			pIPersistFile->Release();
		}
		pIShellLink->Release();
	}

	return csExpandedFile;
}


//////////////////////////////////////////////////////////////////
//
//	ValidatePathname()
//
//	Checks if a pathname is valid based on these options set:
//		Allow directories to be dropped
//		Allow files to be dropped
//		Only allow files with a certain extension to be dropped
//
//	Return value:
//		TRUE:	the pathname is suitable for selection, or
//		FALSE:	the pathname failed the checks.
//
//		If successful, iPathType specifies the type of path
//		validated - either a file or a folder.

BOOL CFileDropListCtrl::ValidatePathname(const CString& csPathname, UINT& iPathType) const
{
	//
	// Get some info about that path so we can filter out dirs
	// and files if need be
	//
	BOOL bValid = FALSE;

	struct _stat buf;
	int result = _tstat( csPathname, &buf );
	if( result == 0 )
	{
		//
		// Do we have a directory? (if we want dirs)
		//
		if ((m_dropMode.iMask & DL_ACCEPT_FOLDERS) &&
			((buf.st_mode & _S_IFDIR) == _S_IFDIR)) 
		{
			bValid = TRUE;
			iPathType = DL_FOLDER_TYPE;
		} 
		else if ((m_dropMode.iMask & DL_ACCEPT_FILES) &&
			((buf.st_mode & _S_IFREG) == _S_IFREG)) 
		{
			// 
			// We've got a file and files are allowed.
			//
			iPathType = DL_FILE_TYPE;

			//
			// Now if we are filtering out specific types,
			// check the file extension.
			//
			if(m_dropMode.iMask & DL_FILTER_EXTENSION)
			{
				LPTSTR pszFileExt = PathFindExtension(csPathname);
				if(CString(pszFileExt).CompareNoCase(m_dropMode.csFileExt) == 0)
				{
					bValid = TRUE;
				}
			}
			else
			{
				bValid = TRUE;
			}
		}
	}

	return bValid;
}


//////////////////////////////////////////////////////////////////
//
//	InsertPathname()
//
//	This is used to insert a dropped item when a callback function
//	hasn't been specified.
//
//	It also checks if duplicate files are allowed to be inserted
//	and does the necessary.
//


int CFileDropListCtrl::InsertPathname(const CString& csFilename)
{
	if(!(m_dropMode.iMask & DL_ALLOW_DUPLICATES))
	{
		//
		// We don't allow duplicate pathnames, so
		// see if this one is already in the list.
		//
		LVFINDINFO lvInfo;
		lvInfo.flags = LVFI_STRING;
		lvInfo.psz = csFilename;

		if(FindItem(&lvInfo, -1) != -1)
			return -1;
	}

	return InsertItem(0, csFilename);
}

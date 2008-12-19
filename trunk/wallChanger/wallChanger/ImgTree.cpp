// ImgTree.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "wallChanger.h"
#include "ImgTree.h"


// ImgTree 대화 상자입니다.

IMPLEMENT_DYNAMIC(ImgTree, CDialog)

ImgTree::ImgTree(CWnd* pParent /*=NULL*/)
	: CDialog(ImgTree::IDD, pParent)
{

}

ImgTree::~ImgTree()
{
}

void ImgTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	HTREEITEM hParent = m_Tree.InsertItem( _T("HOME" ) );
	m_Tree.InsertItem( _T("동물" ),hParent );
	m_Tree.InsertItem( _T("연예인" ),hParent );
	m_Tree.InsertItem( _T("바보" ),hParent );
	m_Tree.InsertItem( _T("엽기" ),hParent );
	m_Tree.Expand( hParent, TVE_EXPAND );
	SettingTree();
}


void ImgTree::SettingTree()
{
	TiXmlDocument doc("wallpaperList.xml");
	if( doc.LoadFile() == false )
		return;

	TiXmlElement* pkChild= (TiXmlElement*)doc.FirstChild("ROOT")->FirstChild();
	
	m_Tree.DeleteAllItems();
	
	HTREEITEM hRoot = m_Tree.InsertItem( _T("내폴더" ) );

	while( pkChild )
	{
		const char* szText = pkChild->Value();
		TCHAR szUniCode[256]={0,};
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szText, strlen(szText), szUniCode, 256);
		m_Tree.InsertItem( szUniCode, hRoot );
		pkChild=(TiXmlElement*)pkChild->NextSibling();
	}
	m_Tree.Expand( hRoot, TVE_EXPAND );

	{
		TiXmlElement* pkChild= (TiXmlElement*)doc.FirstChild("FRIEND");
		if( pkChild )
		{
		const char* szText = pkChild->Value();
		TCHAR szUniCode[256]={0,};
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szText, strlen(szText), szUniCode, 256);
		m_Tree.InsertItem( szUniCode );
		}
	}
}


BEGIN_MESSAGE_MAP(ImgTree, CDialog)
	ON_NOTIFY(TVN_ITEMCHANGED, IDC_TREE1, &ImgTree::OnTvnItemChangedTree1)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &ImgTree::OnNMClickTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &ImgTree::OnTvnSelchangedTree1)
	ON_NOTIFY(TVN_ITEMCHANGING, IDC_TREE1, &ImgTree::OnTvnItemChangingTree1)
END_MESSAGE_MAP()


// ImgTree 메시지 처리기입니다.

void ImgTree::OnTvnItemChangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void ImgTree::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int ti = 0;
	*pResult = 0;
	// NM_CLICK message handler를 다음과 같이 작성한다. 
	// mouse click이 일어났을 때의 mouse pointer의 위치를 가져 온다. 
	DWORD cur_pos = ::GetMessagePos(); 
	CPoint point(LOWORD(cur_pos), HIWORD(cur_pos)); 
	// mouse pointer의 위치를 tree control의 좌표로 바꾸어 준다. 
	m_Tree.ScreenToClient(&point); 
	UINT nFlags = 0; 
	// click이 일어난 위치가 tree control의 check box였는지를 확인한다. 
	// 만약 click이 tree control의 check box였다면 CTreeCtrl::HitTest의 두 번째 parameter에
	// TVHT_ONITEMSTATEICON mask가 1로 setting 된다.

	HTREEITEM hItem = m_Tree.HitTest( point, &nFlags);
	
		if ((hItem != NULL) && (TVHT_ONITEMSTATEICON & nFlags))
		{
			// get the state
			BOOL checked = FALSE;
			checked = TreeView_GetCheckState( m_Tree.GetSafeHwnd(), hItem ); // 현재상황의 반대로 하면됨. 현재 체크가 안되있는데 체크 하려는것임.
			HTREEITEM hParent = m_Tree.GetParentItem( hItem );                        
			while( hParent )
			{
				TreeView_SetCheckState( m_Tree.GetSafeHwnd(), hParent, !checked );
				hParent = m_Tree.GetParentItem( hParent );
			}

			HTREEITEM hChild = m_Tree.GetChildItem( hItem );
			while( hChild )
			{
				TreeView_SetCheckState( m_Tree.GetSafeHwnd(), hChild , !checked );
				hChild = m_Tree.GetNextSiblingItem( hChild );
			}

			m_Tree.SelectItem( hItem );
		}

}

void ImgTree::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void ImgTree::OnTvnItemChangingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE *>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

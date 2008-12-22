// ImgTree.cpp : ���� �����Դϴ�.
#include "stdafx.h"
#include "wallChanger.h"
#include "ImgTree.h"


// ImgTree ��ȭ �����Դϴ�.



IMPLEMENT_DYNAMIC(ImgTree, CDialog)

ImgTree::ImgTree(CWnd* pParent /*=NULL*/)
: CDialog(ImgTree::IDD, pParent)
{

}

ImgTree::~ImgTree()
{
	m_ImageList.DeleteImageList();
}

void ImgTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMGTREE1, m_Tree);

	HTREEITEM hParent = m_Tree.InsertItem( _T("HOME" ) );
	m_Tree.InsertItem( _T("����" ),hParent );
	m_Tree.InsertItem( _T("������" ),hParent );
	m_Tree.InsertItem( _T("�ٺ�" ),hParent );
	m_Tree.InsertItem( _T("����" ),hParent );

	BOOL bRet = m_ImageList.Create( 16, 16, ILC_COLOR32, 0, 0 );

	TCHAR szTemp[MAX_PATH];
	GetSystemDirectory( szTemp, MAX_PATH );

	_tcscat_s( szTemp, MAX_PATH, _T( "\\Shell32.dll" ) );

	HICON hIconSmall;
	UINT ret = ExtractIconEx( szTemp, 3, NULL, &hIconSmall, 1 );
	m_ImageList.Add( hIconSmall );

	HICON hIconSmall2;
	ret = ExtractIconEx( szTemp, 4, NULL, &hIconSmall2, 1 );
	m_ImageList.Add( hIconSmall2 );

	SettingTree();

	SetTimer( 18, 1, 0 );

}

void ImgTree::OnTimer( UINT_PTR nID )
{
	if( nID == 18 )
	{
		OnBnClickedButton1();
		KillTimer( 18 );
	}
	CDialog::OnTimer(nID);
}

TiXmlNode* FindNode( TiXmlNode* pkParent, char* szNodeName )
{
	if( pkParent == 0 ) return 0;

	int t = pkParent->Type();
	if( t== TiXmlNode::ELEMENT )
	{
		char* szValue = (char*)pkParent->Value();
		if( strcmp( szNodeName, szValue ) == 0)
			return pkParent;
	}

	TiXmlNode* pkFocus = 0;
	for ( pkFocus = pkParent->FirstChild(); pkFocus != 0; pkFocus= pkFocus->NextSibling() ) 
	{
		char* szValue = (char* )pkFocus->Value();
		TiXmlNode* pkRet = FindNode( pkFocus, szNodeName ) ;
		if( pkRet )
			return pkRet;
	}

	return 0;
}

void SetCheckChildren( CTreeCtrl& tree,BOOL bChecked,  HTREEITEM hItem  )
{
	while( hItem )
	{
		TreeView_SetCheckState( tree.GetSafeHwnd(), hItem , bChecked );
		if( tree.ItemHasChildren( hItem ) )
		{
			SetCheckChildren( tree, bChecked, tree.GetChildItem( hItem ) );
		}
		hItem = tree.GetNextSiblingItem( hItem );
	}
}

void AddItem_recursiveChildren(CTreeCtrl& tree, HTREEITEM hParent , TiXmlNode* pkNode )
{
	if( hParent==0 )
		return;

	while( pkNode )
	{
		const char* szText = pkNode->Value();
		TCHAR szUniCode[256]={0,};
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szText, strlen(szText), szUniCode, 256);
		if( strcmp( szText, "IMG" ) != 0 )
		{
			HTREEITEM hItem = tree.InsertItem( szUniCode, hParent );

			if( pkNode->NoChildren() == false ) // �ڽ��� ������ 
			{
				TiXmlNode* pkchild =pkNode->FirstChild();	
				AddItem_recursiveChildren( tree, hItem, pkchild );
			}
		}
		pkNode = (TiXmlElement*)pkNode ->NextSibling();
	}

}

void ImgTree::SettingTree()
{
	if( g_servXml.LoadFile() == false )
		return;

	TiXmlElement* pkNode_MyFolderChild = (TiXmlElement*)g_servXml.FirstChild("ROOT")->FirstChild();

	m_Tree.DeleteAllItems();
	m_Tree.SetImageList( &m_ImageList, TVSIL_NORMAL );
	HTREEITEM hRoot = m_Tree.InsertItem( _T("������" ),0,0 );
	// pkChild �� ��� �ڽ��� ��ȸ �ϸ鼭 ������-> ���ȣ��� ����ڽ� �߰�
	AddItem_recursiveChildren( m_Tree, hRoot, pkNode_MyFolderChild );

	m_Tree.Expand( hRoot, TVE_EXPAND );

	HTREEITEM hFriend = m_Tree.InsertItem( _T("FRIEND" ) );
	TiXmlElement* pkNode_Friend= (TiXmlElement*)g_servXml.FirstChild("FRIEND");
	pkNode_Friend= pkNode_Friend->FirstChildElement();
	AddItem_recursiveChildren(m_Tree,hFriend,pkNode_Friend );
	m_Tree.Expand( hFriend, TVE_EXPAND );	

}


BEGIN_MESSAGE_MAP(ImgTree, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_IMGTREE1, &ImgTree::OnNMClickTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &ImgTree::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_IMGTREE1, &ImgTree::OnNMDblclkImgtree1)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_IMGTREE1, &ImgTree::OnTvnEndlabeleditImgtree1)
END_MESSAGE_MAP()
// ImgTree �޽��� ó�����Դϴ�.

void SetCheckparent( CTreeCtrl& tree,BOOL orgChecked, BOOL& bSiblingCheck,  HTREEITEM hItem  )
{
		CString text =L"";
		HTREEITEM hParentItem = tree.GetParentItem( hItem ); // ���� �� ������ �ְ� ù��°�� ã�ƾ� �ϹǷ� �Ѵܰ� ���� �ö󰡼� �ٷ� ù��° ������ ã�ƿ�.
		if( hParentItem == 0 )
			return;

		text = tree.GetItemText( hParentItem  );

		HTREEITEM hSibling = tree.GetChildItem( hParentItem  );
		while( hSibling )
		{
			CString text = tree.GetItemText( hSibling );
			if( hItem != hSibling  )
			bSiblingCheck |= TreeView_GetCheckState( tree.GetSafeHwnd(), hSibling ); 
			hSibling = tree.GetNextSiblingItem( hSibling );
		}

		// �θ� ����.
		if( bSiblingCheck == orgChecked )
		{
			HTREEITEM hParent = tree.GetParentItem( hItem );                        
			if( hParent )
			{
				TreeView_SetCheckState( tree.GetSafeHwnd(), hParent, 	bSiblingCheck	 );

				// ���� �θ� �� �ִٸ� �� �θ��� ���������� üũ��.
				SetCheckparent( tree, orgChecked, bSiblingCheck, hParent );
			}
		}

}

void ImgTree::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{



}

void ImgTree:: FindCheckedLeafItem ( CTreeCtrl& tree, HTREEITEM  hChild )
{

	while( hChild  )
	{
		if( tree.ItemHasChildren( hChild ) )
			FindCheckedLeafItem( tree, tree.GetChildItem(hChild) );
		else
		{
			BOOL checked = TreeView_GetCheckState( tree.GetSafeHwnd(), hChild ); 
			if( checked )
			{
				CString text = tree.GetItemText( hChild );

				const TCHAR* tc = (const TCHAR*)text ;
				char szBuff[128]={0,};
				WideCharToMultiByte( CP_ACP, 0, tc, -1, szBuff, 128, NULL, NULL);
				TiXmlElement* pkFocus = (TiXmlElement*)FindNode( &g_servXml, szBuff ); //img ��带 ������ �ִ� �����θ�. ���� �� ����� �ڽĵ��� �� �߰��ϸ鳡.
				pkFocus = pkFocus->FirstChildElement();
				while( pkFocus )
				{
					std::string url = pkFocus->Attribute("URL");
					if( url.empty() == false )
						urlTable.push_back( url );

					pkFocus = pkFocus->NextSiblingElement();
				}
			}
		}

		hChild =  tree.GetNextSiblingItem( hChild );
	}

}

void ImgTree::MakeUrlTable()
{
	HTREEITEM hRoot = m_Tree.GetRootItem();
	urlTable.clear();
	FindCheckedLeafItem( m_Tree, hRoot ); //check �� leaf �������� ��� �˻��Ѵ�.
}


void ImgTree::OnBnClickedButton1()
{
	HTREEITEM hRoot = m_Tree.GetRootItem();
	BOOL checked = TreeView_GetCheckState( m_Tree.GetSafeHwnd(), hRoot ); // �����Ȳ�� �ݴ�� �ϸ��. ���� üũ�� �ȵ��ִµ� üũ �Ϸ��°���.
	SetCheckChildren( m_Tree, !checked, hRoot );

}

void ImgTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL ImgTree::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	switch( pMsg->message )
	{
	case WM_LBUTTONDOWN:
		{
			int ti = 0;
			// mouse click�� �Ͼ�� ���� mouse pointer�� ��ġ�� ���� �´�. 
			DWORD cur_pos = ::GetMessagePos(); 
			CPoint point(LOWORD(cur_pos), HIWORD(cur_pos)); 
			// mouse pointer�� ��ġ�� tree control�� ��ǥ�� �ٲپ� �ش�. 
			m_Tree.ScreenToClient(&point); 
			UINT nFlags = 0; 
			// click�� �Ͼ ��ġ�� tree control�� check box�������� Ȯ���Ѵ�. 
			// ���� click�� tree control�� check box���ٸ� CTreeCtrl::HitTest�� �� ��° parameter��
			// TVHT_ONITEMSTATEICON mask�� 1�� setting �ȴ�.

			HTREEITEM hItem = m_Tree.HitTest( point, &nFlags);
			//*pResult = 0;
			if ((hItem != NULL) && (TVHT_ONITEM& nFlags))//TVHT_ONITEMSTATEICON 
			{
				// get the state
				BOOL checked = FALSE;
				checked = TreeView_GetCheckState( m_Tree.GetSafeHwnd(), hItem ); // �����Ȳ�� �ݴ�� �ϸ��. ���� üũ�� �ȵ��ִµ� üũ �Ϸ��°���.
				//if( TVHT_ONITEMSTATEICON & nFlags )
				checked = !checked;
				
				if( (TVHT_ONITEMLABEL & nFlags ) | (TVHT_ONITEMICON &nFlags ) )
				TreeView_SetCheckState( m_Tree.GetSafeHwnd(), hItem, checked );

				// �� ������� üũ ���°� ��� �������� �� �θ� ������ ��ģ��.
				BOOL bSiblingCheck = checked;
				SetCheckparent( m_Tree, checked, bSiblingCheck, hItem );

				// �ڽļ���.
				HTREEITEM hChild = m_Tree.GetChildItem( hItem );
				SetCheckChildren( m_Tree,  checked, hChild );

				m_Tree.SelectItem( hItem );
				//		*pResult = 1;
			}
		}
		return FALSE;
	case WM_LBUTTONDBLCLK:
		return TRUE;
	}


	return CDialog::PreTranslateMessage(pMsg);
}

void ImgTree::OnNMDblclkImgtree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	if( (int)*pResult == 1 )
	OnNMClickTree1( pNMHDR, pResult );
	*pResult = 0;
}

void ImgTree::OnTvnEndlabeleditImgtree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	*pResult = 0;
}

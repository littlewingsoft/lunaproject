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
}

void ImgTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	HTREEITEM hParent = m_Tree.InsertItem( _T("HOME" ) );
	m_Tree.InsertItem( _T("����" ),hParent );
	m_Tree.InsertItem( _T("������" ),hParent );
	m_Tree.InsertItem( _T("�ٺ�" ),hParent );
	m_Tree.InsertItem( _T("����" ),hParent );
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
	
	HTREEITEM hRoot = m_Tree.InsertItem( _T("������" ) );

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


// ImgTree �޽��� ó�����Դϴ�.

void ImgTree::OnTvnItemChangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

void ImgTree::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int ti = 0;
	*pResult = 0;
	// NM_CLICK message handler�� ������ ���� �ۼ��Ѵ�. 
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
	
		if ((hItem != NULL) && (TVHT_ONITEMSTATEICON & nFlags))
		{
			// get the state
			BOOL checked = FALSE;
			checked = TreeView_GetCheckState( m_Tree.GetSafeHwnd(), hItem ); // �����Ȳ�� �ݴ�� �ϸ��. ���� üũ�� �ȵ��ִµ� üũ �Ϸ��°���.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

void ImgTree::OnTvnItemChangingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

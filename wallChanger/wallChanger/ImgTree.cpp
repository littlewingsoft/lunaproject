// ImgTree.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "wallChanger.h"
#include "ImgTree.h"


// ImgTree 대화 상자입니다.

TiXmlDocument doc("wallpaperList.xml");

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
	DDX_Control(pDX, IDC_IMGTREE1, m_Tree);

	DWORD style = m_Tree.GetStyle();
	style &= TVS_HASBUTTONS;
	

	HTREEITEM hParent = m_Tree.InsertItem( _T("HOME" ) );
	m_Tree.InsertItem( _T("동물" ),hParent );
	m_Tree.InsertItem( _T("연예인" ),hParent );
	m_Tree.InsertItem( _T("바보" ),hParent );
	m_Tree.InsertItem( _T("엽기" ),hParent );
	//m_Tree.Expand( hParent, TVE_EXPAND );
	SettingTree();
}

//void dump_to_stdout( TiXmlNode* pParent, const char* szStr )
//{
//	if ( !pParent ) return;
//
//	TiXmlNode* pChild;
//	TiXmlText* pText;
//	int t = pParent->Type();
//
//	switch ( t )
//	{
//	case TiXmlNode::DOCUMENT:
//		//printf( "Document" );
//		break;
//
//	case TiXmlNode::ELEMENT:
//		strcmp( pParent->Value(), 
//		//printf( "Element [%s]", pParent->Value() );
//		//num=dump_attribs_to_stdout(pParent->ToElement(), indent+1);
//		switch(num)
//		{
//			//case 0:  printf( " (No attributes)"); break;
//			//case 1:  printf( "%s1 attribute", getIndentAlt(indent)); break;
//			//default: printf( "%s%d attributes", getIndentAlt(indent), num); break;
//		}
//		break;
//
//	case TiXmlNode::COMMENT:
//		//printf( "Comment: [%s]", pParent->Value());
//		break;
//
//	case TiXmlNode::UNKNOWN:
//		//printf( "Unknown" );
//		break;
//
//	case TiXmlNode::TEXT:
//		pText = pParent->ToText();
//		//printf( "Text: [%s]", pText->Value() );
//		break;
//
//	case TiXmlNode::DECLARATION:
//		//printf( "Declaration" );
//		break;
//	default:
//		break;
//	}
//	//printf( "\n" );
//	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
//	{
//		dump_to_stdout( pChild, indent+1 );
//	}
//}

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

void ImgTree::SettingTree()
{

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

		HTREEITEM hFriend = m_Tree.InsertItem( _T("FRIEND" ) );
		TiXmlElement* pkChild= (TiXmlElement*)doc.FirstChild("FRIEND");
		pkChild = pkChild->FirstChildElement();
		
		while( pkChild )
		{
			const char* szText = pkChild->Value();
			TCHAR szUniCode[256]={0,};
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szText, strlen(szText), szUniCode, 256);
			m_Tree.InsertItem( szUniCode , hFriend);
			pkChild = (TiXmlElement*)pkChild->NextSibling();
		}
	}

	
	TiXmlNode* pkNode = FindNode( &doc, "김사랑");//
	if( pkNode )
	{
		int a=0;
	}
}


BEGIN_MESSAGE_MAP(ImgTree, CDialog)
//	ON_NOTIFY(TVN_ITEMCHANGED, IDC_TREE1, &ImgTree::OnTvnItemChangedTree1)
	ON_NOTIFY(NM_CLICK, IDC_IMGTREE1, &ImgTree::OnNMClickTree1)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &ImgTree::OnTvnSelchangedTree1)
	//ON_NOTIFY(TVN_ITEMCHANGING, IDC_TREE1, &ImgTree::OnTvnItemChangingTree1)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ImgTree 메시지 처리기입니다.

//void ImgTree::OnTvnItemChangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}

void SetCheckChildren( CTreeCtrl& tree,bool bChecked,  HTREEITEM hItem  )
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
			SetCheckChildren( m_Tree,  !checked, hChild );

			
			m_Tree.SelectItem( hItem );
		}

		MakeUrlTable();
}

void ImgTree:: FindCheckedLeafItem ( CTreeCtrl& tree, HTREEITEM  hChild )
{

	while( hChild  )
	{
		if( tree.ItemHasChildren( hChild ) )
			FindCheckedLeafItem( tree, tree.GetChildItem(hChild) );
		else
		{
			BOOL checked = TreeView_GetCheckState( tree.GetSafeHwnd(), hChild ); // 현재상황의 반대로 하면됨. 현재 체크가 안되있는데 체크 하려는것임.
			if( checked )
			{
				CString text = tree.GetItemText( hChild );

				const TCHAR* tc = (const TCHAR*)text ;
				char szBuff[128]={0,};
				WideCharToMultiByte( CP_ACP, 0, tc, -1, szBuff, 128, NULL, NULL);
				TiXmlElement* pkFocus = (TiXmlElement*)FindNode( &doc, szBuff ); //img 노드를 가지고 있는 직접부모. 이제 이 노드의 자식들을 다 추가하면끝.
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
	FindCheckedLeafItem( m_Tree, hRoot ); //check 된 leaf 아이템을 모두 검색한다.
}

//
//void ImgTree::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}
//
//void ImgTree::OnTvnItemChangingTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE *>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}

int ImgTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

//	CREATESTRUCT cs ;
//	cs.

	
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	return 0;
}

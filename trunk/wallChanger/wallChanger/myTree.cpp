// myTree.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wallChanger.h"
#include "myTree.h"


// myTree 대화 상자입니다.

IMPLEMENT_DYNAMIC(myTree, CDialog)

myTree::myTree(CWnd* pParent /*=NULL*/)
	: CDialog(myTree::IDD, pParent)
{

}

myTree::~myTree()
{
}

void myTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(myTree, CDialog)
END_MESSAGE_MAP()


// myTree 메시지 처리기입니다.

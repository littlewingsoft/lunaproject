// myTree.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wallChanger.h"
#include "myTree.h"


// myTree ��ȭ �����Դϴ�.

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


// myTree �޽��� ó�����Դϴ�.

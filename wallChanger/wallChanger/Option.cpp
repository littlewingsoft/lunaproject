// Option.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wallChanger.h"
#include "Option.h"


// Option ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Option, CDialog)

Option::Option(CWnd* pParent /*=NULL*/)
	: CDialog(Option::IDD, pParent)
{

}

Option::~Option()
{
}

void Option::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Option, CDialog)
END_MESSAGE_MAP()


// Option �޽��� ó�����Դϴ�.

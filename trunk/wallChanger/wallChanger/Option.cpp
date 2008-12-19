// Option.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wallChanger.h"
#include "Option.h"


// Option 대화 상자입니다.

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


// Option 메시지 처리기입니다.

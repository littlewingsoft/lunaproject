// Option.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wallChanger.h"
#include "Option.h"


// Option 대화 상자입니다.

IMPLEMENT_DYNAMIC(Option, CDialog)

Option::Option(CWnd* pParent /*=NULL*/)
	: CDialog(Option::IDD, pParent)
	, m_DelayTime(0)
{

}

Option::~Option()
{
}

void Option::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DELAYTIME, m_DelayTime);
	DDV_MinMaxUInt(pDX, m_DelayTime, 1, 60);

	m_DelayTime = 5;
	SetDlgItemInt( IDC_EDIT_DELAYTIME, m_DelayTime );
}


BEGIN_MESSAGE_MAP(Option, CDialog)
	ON_EN_CHANGE(IDC_EDIT_DELAYTIME, &Option::OnEnChangeEditDelaytime)
END_MESSAGE_MAP()


// Option 메시지 처리기입니다.

void Option::OnEnChangeEditDelaytime()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	g_pkMainDlg->KillTimer(100);
	g_pkMainDlg->SetTimer( 100, m_DelayTime * 1000 ,0 );
	
}

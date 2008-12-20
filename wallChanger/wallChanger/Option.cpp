// Option.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wallChanger.h"
#include "Option.h"


// Option ��ȭ �����Դϴ�.

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


// Option �޽��� ó�����Դϴ�.

void Option::OnEnChangeEditDelaytime()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� �������ϰ�  ����ũ�� OR �����Ͽ� ������
	// ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ���ؾ߸�
	// �ش� �˸� �޽����� �����ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	g_pkMainDlg->KillTimer(100);
	g_pkMainDlg->SetTimer( 100, m_DelayTime * 1000 ,0 );
	
}

#pragma once
#include "afxwin.h"


// Option ��ȭ �����Դϴ�.

class Option : public CDialog
{
	DECLARE_DYNAMIC(Option)

public:
	Option(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Option();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	UINT m_DelayTime;
//	afx_msg void OnEnChangeEditDelaytime();
	// WallPaper �� ���� 3������ ���� ����.
	CComboBox m_Style;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnEnChangeEditDelaytime();
};
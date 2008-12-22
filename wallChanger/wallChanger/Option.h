#pragma once
#include "afxwin.h"


// Option 대화 상자입니다.

class Option : public CDialog
{
	DECLARE_DYNAMIC(Option)

public:
	Option(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Option();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	UINT m_DelayTime;
//	afx_msg void OnEnChangeEditDelaytime();
	// WallPaper 의 형태 3가지에 대한 설명.
	CComboBox m_Style;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnEnChangeEditDelaytime();
};

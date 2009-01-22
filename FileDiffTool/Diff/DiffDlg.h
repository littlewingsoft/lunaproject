// DiffDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"


// CDiffDlg 대화 상자
class CDiffDlg : public CDialog
{
// 생성입니다.
public:
	CDiffDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIFF_DIALOG };
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	
// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton2();

		BOOL m_bDestroy;
		BOOL m_bCancelThread;
		afx_msg void OnBnClickedButton3();
		afx_msg void OnBnClickedButton4();
		afx_msg void OnClose();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
};
extern CDiffDlg* g_pkMainDlg;
extern CRITICAL_SECTION cs;
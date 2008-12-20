// wallChangerDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"


// CwallChangerDlg ��ȭ ����
class CwallChangerDlg : public CDialog
{
// �����Դϴ�.
public:
	CwallChangerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WALLCHANGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	void SetDelayTimer();
	void DownLoadXml();
	void SettingOption();
	void ChangeWallPaper();
	void PresetPaper();
	void Print( const TCHAR* str );
// �����Դϴ�.
protected:
	HICON m_hIcon;

	
	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	// tab control 
	CXTabCtrl m_Tab;
	afx_msg void OnDestroy();
};

extern CwallChangerDlg* g_pkMainDlg;
#pragma once
#include "afxcmn.h"


// ImgTree 대화 상자입니다.

class ImgTree : public CDialog
{
	DECLARE_DYNAMIC(ImgTree)

public:
	ImgTree(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ImgTree();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;

	void SettingTree();
	afx_msg void OnTvnItemChangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemChangingTree1(NMHDR *pNMHDR, LRESULT *pResult);
};

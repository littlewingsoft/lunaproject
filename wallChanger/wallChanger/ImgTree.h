#pragma once
#include "afxcmn.h"


// ImgTree ��ȭ �����Դϴ�.

class ImgTree : public CDialog
{
	DECLARE_DYNAMIC(ImgTree)

public:
	ImgTree(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ImgTree();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;

	void SettingTree();
	afx_msg void OnTvnItemChangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemChangingTree1(NMHDR *pNMHDR, LRESULT *pResult);
};

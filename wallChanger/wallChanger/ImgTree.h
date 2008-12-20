#pragma once
#include "afxcmn.h"
#include <string>
#include <vector>


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

	void FindCheckedLeafItem ( CTreeCtrl& tree, HTREEITEM  hItem );
public:
	void MakeUrlTable();
	CTreeCtrl m_Tree;
	
	std::vector<std::string > urlTable; //basic_string<TCHAR>
	void SettingTree();
//	afx_msg void OnTvnItemChangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTvnItemChangingTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

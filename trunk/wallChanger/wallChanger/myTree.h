#pragma once


// myTree ��ȭ �����Դϴ�.

class myTree : public CDialog
{
	DECLARE_DYNAMIC(myTree)

public:
	myTree(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~myTree();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WALLCHANGER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

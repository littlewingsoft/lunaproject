#pragma once


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
};

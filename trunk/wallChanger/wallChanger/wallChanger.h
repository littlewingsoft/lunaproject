// wallChanger.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CwallChangerApp:
// �� Ŭ������ ������ ���ؼ��� wallChanger.cpp�� �����Ͻʽÿ�.
//

class CwallChangerApp : public CWinApp
{
public:
	CwallChangerApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CwallChangerApp theApp;
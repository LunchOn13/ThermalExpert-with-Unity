
// MFCi3system.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFCi3systemApp:
// �� Ŭ������ ������ ���ؼ��� MFCi3system.cpp�� �����Ͻʽÿ�.
//

class CMFCi3systemApp : public CWinApp
{
public:
	CMFCi3systemApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFCi3systemApp theApp;


// KSipServerMonitor.h : KSipServerMonitor ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CKSipServerMonitorApp:
// �� Ŭ������ ������ ���ؼ��� KSipServerMonitor.cpp�� �����Ͻʽÿ�.
//

class CKSipServerMonitorApp : public CWinAppEx
{
public:
	CKSipServerMonitorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKSipServerMonitorApp theApp;

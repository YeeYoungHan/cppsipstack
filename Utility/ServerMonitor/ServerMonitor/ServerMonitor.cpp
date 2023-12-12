/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "stdafx.h"
#include "afxwinappex.h"
#include "ServerMonitor.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ServerMonitorDoc.h"
#include "ServerMonitorView.h"

#include "Setup.h"
#include "LogInDlg.h"
#include "TcpSocket.h"
#include "Directory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerMonitorApp

BEGIN_MESSAGE_MAP(CServerMonitorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CServerMonitorApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()

#include "ServerMonitorAbout.hpp"

// CServerMonitorApp 생성

CServerMonitorApp::CServerMonitorApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CServerMonitorApp 개체입니다.

CServerMonitorApp theApp;


// CServerMonitorApp 초기화

BOOL CServerMonitorApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// XML 파일을 읽는다.
	char szXmlFileName[1024];

	_snprintf( szXmlFileName, sizeof(szXmlFileName), "%s\\%s", CDirectory::GetProgramDirectory(), XML_FILENAME );
	if( gclsMonitorSetup.Read( szXmlFileName ) == false )
	{
		AfxMessageBox( gclsMonitorSetup.GetErrorMessage() );
		return FALSE;
	}
	
	SetRegistryKey(_T("ServerMonitor"));
	LoadStdProfileSettings(0);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	gclsSetup.GetFile();

	if( gclsLogInDlg.DoModal() != IDOK )
	{
		return FALSE;
	}

	if( gclsSocket.Connect( gclsLogInDlg.m_strIp, gclsLogInDlg.m_iPort ) == FALSE )
	{
		MessageBox( NULL, _T("Connect Server Error"), _T("Error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	StartMonitorThread();

	InitContextMenuManager();
	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ServerMonitTYPE,
		RUNTIME_CLASS(CServerMonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CServerMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	for( int i = 1; i < gclsMonitorSetup.GetCount(); ++i )
	{
		OnFileNew();
	}

	return TRUE;
}

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CServerMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CServerMonitorApp 사용자 지정 로드/저장 메서드

void CServerMonitorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CServerMonitorApp::LoadCustomState()
{
}

void CServerMonitorApp::SaveCustomState()
{
}

// CServerMonitorApp 메시지 처리기

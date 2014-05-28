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
#include "SipTest.h"
#include "SipTestDlg.h"
#include "Setup.h"
#include "TestInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SipTestDlgAbout.hpp"
#include "SipTestDlgSip.hpp"
#include "SipTestDlgUtil.hpp"

CSipTestDlg::CSipTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipTestDlg::IDD, pParent)
	, m_bCallerLogin(false), m_bCalleeLogin(false), m_bCallee2Login(false), m_bTest(false)
	, m_strSipServerIp(_T(""))
	, m_iSipServerPort(5060)
	, m_strSipDomain(_T(""))
	, m_strCallerId(_T(""))
	, m_strCallerPassWord(_T(""))
	, m_strCalleeId(_T(""))
	, m_strCalleePassWord(_T(""))
	, m_strLog(_T(""))
	, m_strPercent(_T(""))
	, m_bUse2Media(FALSE)
	, m_strCalleeId2(_T(""))
	, m_strCalleePassWord2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSipTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIP_SERVER_IP, m_strSipServerIp);
	DDX_Text(pDX, IDC_SIP_SERVER_PORT, m_iSipServerPort);
	DDX_Text(pDX, IDC_SIP_DOMAIN, m_strSipDomain);
	DDX_Text(pDX, IDC_CALLER_ID, m_strCallerId);
	DDX_Text(pDX, IDC_CALLER_PW, m_strCallerPassWord);
	DDX_Text(pDX, IDC_CALLEE_ID, m_strCalleeId);
	DDX_Text(pDX, IDC_CALLEE_PW, m_strCalleePassWord);
	DDX_Control(pDX, IDC_START_SIP_STACK, m_btnStartSipStack);
	DDX_Control(pDX, IDC_STOP_SIP_STACK, m_btnStopSipStack);
	DDX_Control(pDX, IDC_START_TEST, m_btnStartTest);
	DDX_Control(pDX, IDC_STOP_TEST, m_btnStopTest);
	DDX_Text(pDX, IDC_LOG, m_strLog);
	DDX_Control(pDX, IDC_PROGRESS, m_clsProgress);
	DDX_Text(pDX, IDC_PERCENT, m_strPercent);
	DDX_Control(pDX, IDC_LOG, m_txtLog);
	DDX_Check(pDX, IDC_USE_TWO_MEDIA, m_bUse2Media);
	DDX_Control(pDX, IDC_USE_TWO_MEDIA, m_chkUse2Media);
	DDX_Text(pDX, IDC_CALLEE_ID2, m_strCalleeId2);
	DDX_Text(pDX, IDC_CALLEE_PW2, m_strCalleePassWord2);
}

BEGIN_MESSAGE_MAP(CSipTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SIP_MESSAGE_ID, &CSipTestDlg::OnSipMessage)
	ON_MESSAGE(SIP_TEST_ID, &CSipTestDlg::OnTestMessage)
	ON_BN_CLICKED(IDC_START_SIP_STACK, &CSipTestDlg::OnBnClickedStartSipStack)
	ON_BN_CLICKED(IDC_STOP_SIP_STACK, &CSipTestDlg::OnBnClickedStopSipStack)
	ON_BN_CLICKED(IDC_START_TEST, &CSipTestDlg::OnBnClickedStartTest)
	ON_BN_CLICKED(IDC_STOP_TEST, &CSipTestDlg::OnBnClickedStopTest)
	ON_BN_CLICKED(IDC_CLEAR_LOG, &CSipTestDlg::OnBnClickedClearLog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSipTestDlg message handlers

BOOL CSipTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_clsProgress.SetRange( 0, 100 );

	gclsSetup.Get();

	m_strSipServerIp = gclsSetup.m_strSipServerIp.c_str();
	m_iSipServerPort = gclsSetup.m_iSipServerPort;
	m_strSipDomain = gclsSetup.m_strSipDomain.c_str();
	m_strCallerId = gclsSetup.m_strCallerId.c_str();
	m_strCallerPassWord = gclsSetup.m_strCallerPassWord.c_str();
	m_strCalleeId = gclsSetup.m_strCalleeId.c_str();
	m_strCalleePassWord = gclsSetup.m_strCalleePassWord.c_str();
	m_strCalleeId2 = gclsSetup.m_strCalleeId2.c_str();
	m_strCalleePassWord2 = gclsSetup.m_strCalleePassWord2.c_str();

	UpdateData(FALSE);

	m_btnStopSipStack.EnableWindow( FALSE );
	m_btnStartTest.EnableWindow( FALSE );
	m_btnStopTest.EnableWindow( FALSE );

#ifndef USE_MEDIA_LIST
	m_chkUse2Media.EnableWindow( FALSE );
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSipTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSipTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSipTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**
 * @ingroup SipTest
 * @brief �׽�Ʈ ��� �̺�Ʈ ���� �ڵ鷯
 * @param wParam 
 * @param lParam 
 * @returns 0 �� �����Ѵ�.
 */
LRESULT CSipTestDlg::OnTestMessage( WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case WM_TEST_END:
		m_btnStartTest.EnableWindow( TRUE );
		m_btnStopTest.EnableWindow( FALSE );
		break;
	case WM_TEST_MSG:
		SetLog( "%s", lParam );
		break;
	}

	return 0;
}

/**
 * @ingroup SipTest
 * @brief SIP stack �� �����Ѵ�.
 */
void CSipTestDlg::OnBnClickedStartSipStack()
{
	// �Է°��� ��ȿ���� �˻��Ѵ�.
	UpdateData(TRUE);

	if( CheckInput( m_strSipServerIp, "SIP Server IP" ) == false ||
			CheckInput( m_strCallerId, "Caller ID" ) == false ||
			CheckInput( m_strCalleeId, "Callee ID" ) == false ) 
	{
		return;
	}

	if( m_strCallerId == m_strCalleeId )
	{
		MessageBox( "CallerId is equal to CalleeId", "ID Error", MB_OK | MB_ICONERROR );
		return;
	}

	// ��ȿ���� �ʴ� �Է°��� �����Ѵ�.
	if( m_strSipDomain.IsEmpty() ) m_strSipDomain = m_strSipServerIp;
	if( m_iSipServerPort <= 0 || m_iSipServerPort > 65535 ) m_iSipServerPort = 5060;

	UpdateData(FALSE);

	// ���� ���Ͽ� �����Ѵ�.
	gclsSetup.m_strSipServerIp = m_strSipServerIp;
	gclsSetup.m_iSipServerPort = m_iSipServerPort;
	gclsSetup.m_strSipDomain = m_strSipDomain;
	gclsSetup.m_strCallerId = m_strCallerId;
	gclsSetup.m_strCallerPassWord = m_strCallerPassWord;
	gclsSetup.m_strCalleeId = m_strCalleeId;
	gclsSetup.m_strCalleePassWord = m_strCalleePassWord;
	gclsSetup.m_strCalleeId2 = m_strCalleeId2;
	gclsSetup.m_strCalleePassWord2 = m_strCalleePassWord2;

	gclsSetup.Put();

	// SipStack �� �����Ѵ�.
	m_clsSipUserAgentMFC.SetWindowHandle( GetSafeHwnd() );
	m_clsSipUserAgentMFC.SetCallBack( this );

	CSipStackSetup clsSetup;
	CSipServerInfo clsInfo;

	GetLocalIp( clsSetup.m_strLocalIp );

	clsInfo.m_strIp = m_strSipServerIp;
	clsInfo.m_iPort = m_iSipServerPort;
	clsInfo.m_strDomain = m_strSipDomain;
	clsInfo.m_strUserId = m_strCallerId;
	clsInfo.m_strPassWord = m_strCallerPassWord;

	gclsSipUserAgent.InsertRegisterInfo( clsInfo );

	clsInfo.m_strUserId = m_strCalleeId;
	clsInfo.m_strPassWord = m_strCalleePassWord;

	gclsSipUserAgent.InsertRegisterInfo( clsInfo );

	if( gclsSetup.m_strCalleeId2.empty() == false )
	{
		clsInfo.m_strUserId = m_strCalleeId2;
		clsInfo.m_strPassWord = m_strCalleePassWord2;

		gclsSipUserAgent.InsertRegisterInfo( clsInfo );
	}

	bool bSuccess = false;

	for( int i = 0; i < 100; ++i )
	{
		clsSetup.m_iLocalUdpPort = i + 10000;

		if( gclsSipUserAgent.Start( clsSetup, &m_clsSipUserAgentMFC ) )
		{
			bSuccess = true;
			break;
		}
	}

	if( bSuccess == false )
	{
		MessageBox( "sip stack start error", "Error", MB_OK );
		return;
	}

	m_btnStartSipStack.EnableWindow( FALSE );
	m_btnStopSipStack.EnableWindow( TRUE );
}

/**
 * @ingroup SipTest
 * @brief SIP stack �� �����Ѵ�.
 */
void CSipTestDlg::OnBnClickedStopSipStack()
{
	if( gclsSipUserAgent.Stop() == false )
	{
		MessageBox( "sip stack stop error", "Error", MB_OK );
		return;
	}

	m_btnStartSipStack.EnableWindow( TRUE );
	m_btnStopSipStack.EnableWindow( FALSE );
	m_btnStartTest.EnableWindow( FALSE );
	m_btnStopTest.EnableWindow( FALSE );

	m_bTest = false;
}

/**
 * @ingroup SipTest
 * @brief �׽�Ʈ�� �����Ѵ�.
 */
void CSipTestDlg::OnBnClickedStartTest()
{
	UpdateData(TRUE);

#ifndef USE_MEDIA_LIST
	if( m_bUse2Media )
	{
		MessageBox( "rebuild SipUserAgent with USE_MEDIA_LIST", "Error", MB_OK | MB_ICONERROR );
		return;
	}
#endif

	m_strPercent = "0 %";

#ifdef USE_MEDIA_LIST
	gclsSetup.m_bUseTwoMedia = ( m_bUse2Media ? true : false );
	gclsSetup.Put();
#endif

	UpdateData(FALSE);

	m_clsProgress.SetPos( 0 );

	if( StartTestThread( GetSafeHwnd() ) == false )
	{
		MessageBox( "Start Test thread error", "Error", MB_OK | MB_ICONERROR );
		return;
	}
	
	m_bTest = true;

	m_btnStartTest.EnableWindow( FALSE );
	m_btnStopTest.EnableWindow( TRUE );
}

/**
 * @ingroup SipTest
 * @brief �׽�Ʈ�� �����Ѵ�.
 */
void CSipTestDlg::OnBnClickedStopTest()
{
	StopTestThread();
	
	m_bTest = false;

	m_btnStartTest.EnableWindow( TRUE );
	m_btnStopTest.EnableWindow( FALSE );
}

/**
 * @ingroup SipTest
 * @brief �α׸� �ʱ�ȭ��Ų��.
 */
void CSipTestDlg::OnBnClickedClearLog()
{
	m_clsMutex.acquire();

	m_strLog = "";
	UpdateData(FALSE);

	m_clsMutex.release();
}

/**
 * @ingroup SipTest
 * @brief ������ ���� �̺�Ʈ �ڵ鷯
 */
void CSipTestDlg::OnDestroy()
{
	m_clsSipUserAgentMFC.SetWindowHandle( 0 );

	gclsSipUserAgent.Stop();
	gclsSipUserAgent.Final();

	__super::OnDestroy();
}

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
#include "SipSpeed.h"
#include "SipSpeedDlg.h"
#include "Setup.h"
#include "SipTest.h"
#include "CallIdMap.h"
#include "SipUserAgentMFCDefine.h"
#include "SipUserAgentMFCEventClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSipUserAgent		 gclsSipUserAgent;

#include "SipSpeedDlgAbout.hpp"
#include "SipSpeedDlgSip.hpp"
#include "SipSpeedDlgSipEvent.hpp"
#include "SipSpeedDlgUtil.hpp"

CSipSpeedDlg::CSipSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipSpeedDlg::IDD, pParent)
	, m_bCallerLogin(false), m_bCalleeLogin(false), m_bTest(false), m_iCallSuccess(0), m_iCallError(0)
	, m_strSipServerIp(_T(""))
	, m_iSipServerPort(5060)
	, m_strSipDomain(_T(""))
	, m_strCallerId(_T(""))
	, m_strCallerPassWord(_T(""))
	, m_strCalleeId(_T(""))
	, m_strCalleePassWord(_T(""))
	, m_iCallTotalCount(100)
	, m_iCallConcurrentCount(10)
	, m_strLog(_T(""))
	, m_strPercent(_T(""))
	, m_iTestType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSipSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIP_SERVER_IP, m_strSipServerIp);
	DDX_Text(pDX, IDC_SIP_SERVER_PORT, m_iSipServerPort);
	DDX_Text(pDX, IDC_SIP_DOMAIN, m_strSipDomain);
	DDX_Text(pDX, IDC_CALLER_ID, m_strCallerId);
	DDX_Text(pDX, IDC_CALLER_PW, m_strCallerPassWord);
	DDX_Text(pDX, IDC_CALLEE_ID, m_strCalleeId);
	DDX_Text(pDX, IDC_CALLEE_PW, m_strCalleePassWord);
	DDX_Text(pDX, IDC_CALL_TOTAL_COUNT, m_iCallTotalCount);
	DDX_Text(pDX, IDC_CALL_CONCURRENT_COUNT, m_iCallConcurrentCount);
	DDX_Control(pDX, IDC_START_SIP_STACK, m_btnStartSipStack);
	DDX_Control(pDX, IDC_STOP_SIP_STACK, m_btnStopSipStack);
	DDX_Control(pDX, IDC_START_TEST, m_btnStartTest);
	DDX_Control(pDX, IDC_STOP_TEST, m_btnStopTest);
	DDX_Text(pDX, IDC_LOG, m_strLog);
	DDX_Control(pDX, IDC_PROGRESS, m_clsProgress);
	DDX_Text(pDX, IDC_PERCENT, m_strPercent);
	DDX_Control(pDX, IDC_LOG, m_txtLog);
	DDX_Radio(pDX, IDC_ONE_TO_ONE_CALL, m_iTestType);
}

BEGIN_MESSAGE_MAP(CSipSpeedDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START_SIP_STACK, &CSipSpeedDlg::OnBnClickedStartSipStack)
	ON_BN_CLICKED(IDC_STOP_SIP_STACK, &CSipSpeedDlg::OnBnClickedStopSipStack)
	ON_BN_CLICKED(IDC_START_TEST, &CSipSpeedDlg::OnBnClickedStartTest)
	ON_BN_CLICKED(IDC_STOP_TEST, &CSipSpeedDlg::OnBnClickedStopTest)
	ON_MESSAGE(SIP_MESSAGE_ID, &CSipSpeedDlg::OnSipMessage)
	ON_MESSAGE(SIP_TEST_ID, &CSipSpeedDlg::OnTestMessage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CLEAR_LOG, &CSipSpeedDlg::OnBnClickedClearLog)
END_MESSAGE_MAP()


// CSipSpeedDlg message handlers

BOOL CSipSpeedDlg::OnInitDialog()
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
	m_iCallTotalCount = gclsSetup.m_iCallTotalCount;
	m_iCallConcurrentCount = gclsSetup.m_iCallConcurrentCount;
	m_iTestType = gclsSetup.m_iTestType;

	UpdateData(FALSE);

	m_btnStopSipStack.EnableWindow( FALSE );
	m_btnStartTest.EnableWindow( FALSE );
	m_btnStopTest.EnableWindow( FALSE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSipSpeedDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSipSpeedDlg::OnPaint()
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
HCURSOR CSipSpeedDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/**
 * @ingroup SipSpeed
 * @brief SIP stack 을 시작한다.
 */
void CSipSpeedDlg::OnBnClickedStartSipStack()
{
	// 입력값이 유효한지 검사한다.
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

	// 유효하지 않는 입력값을 수정한다.
	if( m_strSipDomain.IsEmpty() ) m_strSipDomain = m_strSipServerIp;
	if( m_iSipServerPort <= 0 || m_iSipServerPort > 65535 ) m_iSipServerPort = 5060;
	if( m_iCallTotalCount <= 0 ) m_iCallTotalCount = 100;
	if( m_iCallConcurrentCount <= 0 ) m_iCallConcurrentCount = 10;

	UpdateData(FALSE);

	// 설정 파일에 저장한다.
	gclsSetup.m_strSipServerIp = m_strSipServerIp;
	gclsSetup.m_iSipServerPort = m_iSipServerPort;
	gclsSetup.m_strSipDomain = m_strSipDomain;
	gclsSetup.m_strCallerId = m_strCallerId;
	gclsSetup.m_strCallerPassWord = m_strCallerPassWord;
	gclsSetup.m_strCalleeId = m_strCalleeId;
	gclsSetup.m_strCalleePassWord = m_strCalleePassWord;
	gclsSetup.m_iCallTotalCount = m_iCallTotalCount;
	gclsSetup.m_iCallConcurrentCount = m_iCallConcurrentCount;
	gclsSetup.m_iTestType = m_iTestType;

	gclsSetup.Put();

	// SipStack 을 시작한다.
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

	bool bSuccess = false;

	clsSetup.m_iUdpThreadCount = 20;
	clsSetup.m_iStackExecutePeriod = 100;
	clsSetup.m_iTimerD = 2000;
	clsSetup.m_iTimerJ = 2000;

	for( int i = 0; i < 100; ++i )
	{
		clsSetup.m_iLocalUdpPort = i + 10000;

		if( gclsSipUserAgent.Start( clsSetup, this ) )
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
 * @ingroup SipSpeed
 * @brief SIP stack 을 중지시킨다.
 */
void CSipSpeedDlg::OnBnClickedStopSipStack()
{
	gclsSipUserAgent.Stop();

	m_btnStartSipStack.EnableWindow( TRUE );
	m_btnStopSipStack.EnableWindow( FALSE );
	m_btnStartTest.EnableWindow( FALSE );
	m_btnStopTest.EnableWindow( FALSE );

	m_bTest = false;
}

/**
 * @ingroup SipSpeed
 * @brief 테스트를 시작한다.
 */
void CSipSpeedDlg::OnBnClickedStartTest()
{
	UpdateData(TRUE);

	if( m_iCallTotalCount <= 0 ) m_iCallTotalCount = 100;
	if( m_iCallConcurrentCount <= 0 ) m_iCallConcurrentCount = 10;
	m_strPercent = "0 %";

	UpdateData(FALSE);

	m_clsProgress.SetPos( 0 );

	gclsSetup.m_iCallTotalCount = m_iCallTotalCount;
	gclsSetup.m_iCallConcurrentCount = m_iCallConcurrentCount;
	gclsSetup.m_iTestType = m_iTestType;

	gclsSetup.Put();

	m_iCallSuccess = 0;
	m_iCallError = 0;

	gclsSipUserAgent.m_clsSipStack.AddCallBack( this );

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
 * @ingroup SipSpeed
 * @brief 테스트를 중지한다.
 */
void CSipSpeedDlg::OnBnClickedStopTest()
{
	StopTestThread();
}

void CSipSpeedDlg::OnDestroy()
{
	gclsSipUserAgent.Stop();
	gclsSipUserAgent.Final();

	__super::OnDestroy();
}

/**
 * @ingroup SipSpeed
 * @brief 테스트 결과 이벤트 수신 핸들러
 * @param wParam 
 * @param lParam 
 * @returns 0 을 리턴한다.
 */
LRESULT CSipSpeedDlg::OnTestMessage( WPARAM wParam, LPARAM lParam )
{
	if( wParam == WM_TEST_END )
	{
		double fCps = 0.0;

		if( lParam > 0 )
		{
			fCps = (double)( m_iCallSuccess + m_iCallError ) / ( (double)lParam / 1000 );
		}

		SetLog( "call success(%d) / error(%d) / cps(%.3f) / time(%d ms)", m_iCallSuccess, m_iCallError, fCps, lParam );

		m_bTest = false;

		m_btnStartTest.EnableWindow( TRUE );
		m_btnStopTest.EnableWindow( FALSE );

		gclsSipUserAgent.m_clsSipStack.DeleteCallBack( this );
	}

	return 0;
}

/**
 * @ingroup SipSpeed
 * @brief 로그를 삭제한다.
 */
void CSipSpeedDlg::OnBnClickedClearLog()
{
	m_clsMutex.acquire();

	m_strLog = "";
	UpdateData(FALSE);

	m_clsMutex.release();
}

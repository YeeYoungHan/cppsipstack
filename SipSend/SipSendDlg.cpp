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
#include "SipSend.h"
#include "SipSendDlg.h"
#include "Setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSipUserAgent		 gclsSipUserAgent;

#include "SipSendDlgSip.hpp"

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSipSendDlg dialog




CSipSendDlg::CSipSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipSendDlg::IDD, pParent)
	, m_strSipServerIp(_T(""))
	, m_iSipServerPort(5060)
	, m_strSipDomain(_T(""))
	, m_strCallerId(_T(""))
	, m_strCallerPassWord(_T(""))
	, m_strSip(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSipSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIP_SERVER_IP, m_strSipServerIp);
	DDX_Text(pDX, IDC_SIP_SERVER_PORT, m_iSipServerPort);
	DDX_Text(pDX, IDC_SIP_DOMAIN, m_strSipDomain);
	DDX_Text(pDX, IDC_CALLER_ID, m_strCallerId);
	DDX_Text(pDX, IDC_CALLER_PW, m_strCallerPassWord);
	DDX_Control(pDX, IDC_START_SIP_STACK, m_btnStartSipStack);
	DDX_Control(pDX, IDC_STOP_SIP_STACK, m_btnStopSipStack);
	DDX_Text(pDX, IDC_SIP, m_strSip);
}

BEGIN_MESSAGE_MAP(CSipSendDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSipSendDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_START_SIP_STACK, &CSipSendDlg::OnBnClickedStartSipStack)
	ON_BN_CLICKED(IDC_STOP_SIP_STACK, &CSipSendDlg::OnBnClickedStopSipStack)
	ON_BN_CLICKED(IDC_SEND, &CSipSendDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CSipSendDlg message handlers

BOOL CSipSendDlg::OnInitDialog()
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

	gclsSetup.Get();

	m_strSipServerIp = gclsSetup.m_strSipServerIp.c_str();
	m_iSipServerPort = gclsSetup.m_iSipServerPort;
	m_strSipDomain = gclsSetup.m_strSipDomain.c_str();
	m_strCallerId = gclsSetup.m_strCallerId.c_str();
	m_strCallerPassWord = gclsSetup.m_strCallerPassWord.c_str();

	UpdateData(FALSE);

	m_btnStopSipStack.EnableWindow( FALSE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSipSendDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSipSendDlg::OnPaint()
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
HCURSOR CSipSendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSipSendDlg::OnBnClickedOk()
{
}

void CSipSendDlg::OnBnClickedStartSipStack()
{
	UpdateData(TRUE);

	if( CheckInput( m_strSipServerIp, "SIP Server IP" ) == false ||
			CheckInput( m_strCallerId, "SIP User" ) == false ) 
	{
		return;
	}

	// 유효하지 않는 입력값을 수정한다.
	if( m_strSipDomain.IsEmpty() ) m_strSipDomain = m_strSipServerIp;
	if( m_iSipServerPort <= 0 || m_iSipServerPort > 65535 ) m_iSipServerPort = 5060;

	UpdateData(FALSE);

	// 설정 파일에 저장한다.
	gclsSetup.m_strSipServerIp = m_strSipServerIp;
	gclsSetup.m_iSipServerPort = m_iSipServerPort;
	gclsSetup.m_strSipDomain = m_strSipDomain;
	gclsSetup.m_strCallerId = m_strCallerId;
	gclsSetup.m_strCallerPassWord = m_strCallerPassWord;
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

	bool bSuccess = false;

	for( int i = 0; i < 100; ++i )
	{
		clsSetup.m_iLocalUdpPort = i + 5060;

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

void CSipSendDlg::OnBnClickedStopSipStack()
{
	gclsSipUserAgent.Stop();

	m_btnStartSipStack.EnableWindow( TRUE );
	m_btnStopSipStack.EnableWindow( FALSE );
}

void CSipSendDlg::OnBnClickedSend()
{
	UpdateData(TRUE);

	char	szText[8192];
	int		iPos = 0, iLen = m_strSip.GetLength();

	memset( szText, 0, sizeof(szText) );
	for( int i = 0; i < iLen; ++i )
	{
		if( m_strSip.GetAt( i ) == 0x0d ) continue;
		if( m_strSip.GetAt( i ) == 0x0a )
		{
			szText[iPos++] = 0x0d;
			szText[iPos++] = m_strSip.GetAt( i );
		}
		else
		{
			szText[iPos++] = m_strSip.GetAt( i );
		}
	}

	if( szText[iPos-1] != 0x0a )
	{
		szText[iPos++] = 0x0d;
		szText[iPos++] = 0x0a;
	}

	// Content-Length 를 수정한다.
	CSipMessage clsMessage;

	clsMessage.Parse( szText, iPos );

	const char * pszPos = strstr( szText, "\r\n\r\n" );
	if( pszPos )
	{
		clsMessage.m_strBody.clear();
		clsMessage.m_strBody.append( pszPos + 4 );
		clsMessage.m_iContentLength = clsMessage.m_strBody.length();
	}

	memset( szText, 0, sizeof(szText) );
	iPos = clsMessage.ToString( szText, sizeof(szText) );

	gclsSipUserAgent.m_clsSipStack.Send( szText, m_strSipServerIp, m_iSipServerPort, E_SIP_UDP );
}

bool CSipSendDlg::CheckInput( CString & strInput, const char * pszName )
{
	if( strInput.IsEmpty() )
	{
		CString	strText;

		strText.Format( "Please ~ insert %s", pszName ); 

		MessageBox( strText, "Input Error", MB_OK | MB_ICONERROR );
		return false;
	}

	return true;
}

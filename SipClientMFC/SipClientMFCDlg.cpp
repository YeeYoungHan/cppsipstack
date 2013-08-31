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
#include "SipClientMFC.h"
#include "SipClientMFCDlg.h"
#include "SipSetupDlg.h"
#include "Setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSipClientMFCDlg dialog

CSipClientMFCDlg::CSipClientMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipClientMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSipClientMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_STACK, m_btnStartStack);
	DDX_Control(pDX, IDC_STOP_STACK, m_btnStopStack);
	DDX_Control(pDX, IDC_START_CALL, m_btnStartCall);
	DDX_Control(pDX, IDC_STOP_CALL, m_btnStopCall);
	DDX_Control(pDX, IDC_ACCEPT_CALL, m_btnAcceptCall);
}

BEGIN_MESSAGE_MAP(CSipClientMFCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETUP, &CSipClientMFCDlg::OnBnClickedSetup)
	ON_BN_CLICKED(IDC_START_STACK, &CSipClientMFCDlg::OnBnClickedStartStack)
	ON_BN_CLICKED(IDC_STOP_STACK, &CSipClientMFCDlg::OnBnClickedStopStack)
	ON_BN_CLICKED(IDC_START_CALL, &CSipClientMFCDlg::OnBnClickedStartCall)
	ON_BN_CLICKED(IDC_STOP_CALL, &CSipClientMFCDlg::OnBnClickedStopCall)
	ON_BN_CLICKED(IDC_ACCEPT_CALL, &CSipClientMFCDlg::OnBnClickedAcceptCall)
	ON_MESSAGE(SIP_MESSAGE_ID, &CSipClientMFCDlg::OnSipMessage)
END_MESSAGE_MAP()


// CSipClientMFCDlg message handlers

BOOL CSipClientMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_btnStartCall.EnableWindow( FALSE );
	m_btnStopCall.EnableWindow( FALSE );
	m_btnAcceptCall.EnableWindow( FALSE );
	m_btnStopStack.EnableWindow( FALSE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSipClientMFCDlg::OnPaint()
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
HCURSOR CSipClientMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSipClientMFCDlg::OnOK()
{

}

void CSipClientMFCDlg::OnBnClickedSetup()
{
	CSipSetupDlg clsDlg;

	clsDlg.DoModal();
}

void CSipClientMFCDlg::OnBnClickedStartStack()
{
	if( gclsSetup.IsSet() == false )
	{
		MessageBox( "Please~ click setup button", "Information", MB_OK );
		return;
	}

	m_clsSipUserAgentMFC.SetWindowHandle( GetSafeHwnd() );
	m_clsSipUserAgentMFC.SetCallBack( this );

	CSipStackSetup clsSetup;
	CSipServerInfo clsInfo;

	clsInfo.m_strIp = gclsSetup.m_strSipServerIp;
	clsInfo.m_iPort = gclsSetup.m_iSipServerPort;
	clsInfo.m_strDomain = gclsSetup.m_strSipDomain;
	clsInfo.m_strUserId = gclsSetup.m_strUserId;
	clsInfo.m_strPassWord = gclsSetup.m_strPassWord;

	m_clsSipUserAgent.InsertRegisterInfo( clsInfo );
	if( m_clsSipUserAgent.Start( clsSetup, &m_clsSipUserAgentMFC ) == false )
	{
		MessageBox( "sip stack start error", "Error", MB_OK );
		return;
	}

	m_btnStartStack.EnableWindow( FALSE );
	m_btnStopStack.EnableWindow( TRUE );
}

void CSipClientMFCDlg::OnBnClickedStopStack()
{
}

void CSipClientMFCDlg::OnBnClickedStartCall()
{
}

void CSipClientMFCDlg::OnBnClickedStopCall()
{
}

void CSipClientMFCDlg::OnBnClickedAcceptCall()
{
}

LRESULT CSipClientMFCDlg::OnSipMessage( WPARAM wParam, LPARAM lParam )
{
	return 0;
}

void CSipClientMFCDlg::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
}

bool CSipClientMFCDlg::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	return true;
}

void CSipClientMFCDlg::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
}

void CSipClientMFCDlg::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
}

void CSipClientMFCDlg::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

void CSipClientMFCDlg::EventCallEnd( const char * pszCallId, int iSipStatus )
{
}

void CSipClientMFCDlg::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

bool CSipClientMFCDlg::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	return false;
}

bool CSipClientMFCDlg::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	return false;
}

bool CSipClientMFCDlg::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	return false;
}

void CSipClientMFCDlg::EventCallBackThreadEnd( int iThreadId )
{
}

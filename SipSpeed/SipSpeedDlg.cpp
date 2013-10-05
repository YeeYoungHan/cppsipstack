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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSipSpeedDlg dialog




CSipSpeedDlg::CSipSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipSpeedDlg::IDD, pParent)
	, m_strSipServerIp(_T(""))
	, m_iSipServerPort(0)
	, m_strSipDomain(_T(""))
	, m_strCallerId(_T(""))
	, m_strCallerPassWord(_T(""))
	, m_strCalleeId(_T(""))
	, m_strCalleePassWord(_T(""))
	, m_iCallTotalCount(0)
	, m_iCallConcurrentCount(0)
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

	// TODO: Add extra initialization here

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


void CSipSpeedDlg::OnBnClickedStartSipStack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSipSpeedDlg::OnBnClickedStopSipStack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSipSpeedDlg::OnBnClickedStartTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSipSpeedDlg::OnBnClickedStopTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

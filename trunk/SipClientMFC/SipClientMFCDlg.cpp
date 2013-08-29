
// SipClientMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SipClientMFC.h"
#include "SipClientMFCDlg.h"
#include "SipSetupDlg.h"

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
END_MESSAGE_MAP()


// CSipClientMFCDlg message handlers

BOOL CSipClientMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSipClientMFCDlg::OnBnClickedStopStack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSipClientMFCDlg::OnBnClickedStartCall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSipClientMFCDlg::OnBnClickedStopCall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSipClientMFCDlg::OnBnClickedAcceptCall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

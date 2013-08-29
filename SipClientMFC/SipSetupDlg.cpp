// SipSetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SipClientMFC.h"
#include "SipSetupDlg.h"
#include "Setup.h"

// CSipSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSipSetupDlg, CDialog)

CSipSetupDlg::CSipSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipSetupDlg::IDD, pParent)
	, m_strSipServerIp(_T(""))
	, m_iSipServerPort(0)
	, m_strSipDomain(_T(""))
	, m_strUserId(_T(""))
	, m_strPassWord(_T(""))
{

}

CSipSetupDlg::~CSipSetupDlg()
{
}

void CSipSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIP_SERVER_IP, m_strSipServerIp);
	DDX_Text(pDX, IDC_SIP_SERVER_PORT, m_iSipServerPort);
	DDX_Text(pDX, IDC_SIP_DOMAIN, m_strSipDomain);
	DDX_Text(pDX, IDC_USER_ID, m_strUserId);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassWord);
}


BEGIN_MESSAGE_MAP(CSipSetupDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSipSetupDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CSipSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strSipServerIp = gclsSetup.m_strSipServerIp.c_str();
	m_iSipServerPort = gclsSetup.m_iSipServerPort;
	m_strSipDomain = gclsSetup.m_strSipDomain.c_str();
	m_strUserId = gclsSetup.m_strUserId.c_str();
	m_strPassWord = gclsSetup.m_strPassWord.c_str();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CSipSetupDlg 메시지 처리기입니다.

void CSipSetupDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	if( m_strSipServerIp.GetLength() == 0 )
	{
		MessageBox( "SIP server IP is not inserted", "Error", MB_OK );
		return;
	}

	if( m_iSipServerPort <= 0 || m_iSipServerPort > 65535 )
	{
		MessageBox( "SIP server Port is not correct", "Error", MB_OK );
		return;
	}

	if( m_strUserId.GetLength() == 0 )
	{
		MessageBox( "User ID is not inserted", "Error", MB_OK );
		return;
	}

	if( m_strSipDomain.GetLength() == 0 )
	{
		m_strSipDomain = m_strSipServerIp;
	}

	gclsSetup.m_strSipServerIp = m_strSipServerIp;
	gclsSetup.m_iSipServerPort = m_iSipServerPort;
	gclsSetup.m_strSipDomain = m_strSipDomain;
	gclsSetup.m_strUserId = m_strUserId;
	gclsSetup.m_strPassWord = m_strPassWord;

	gclsSetup.Put();

	OnOK();
}

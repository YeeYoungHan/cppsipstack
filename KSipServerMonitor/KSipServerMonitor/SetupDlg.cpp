// SetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KSipServerMonitor.h"
#include "SetupDlg.h"
#include "LogInDlg.h"

// CSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
	, m_iPeriod(0)
{
	m_iPeriod = gclsLogInDlg.m_iPeriod;
}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PERIOD, m_iPeriod);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetupDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetupDlg 메시지 처리기입니다.

void CSetupDlg::OnBnClickedOk()
{
	UpdateData( TRUE );

	if( m_iPeriod <= 0 || m_iPeriod > 60 )
	{
		MessageBox( _T("Invalid period. please insert 1 ~ 60"), _T("Info"), MB_OK | MB_ICONINFORMATION );
		return;
	}

	gclsLogInDlg.m_iPeriod = m_iPeriod;

	OnOK();
}

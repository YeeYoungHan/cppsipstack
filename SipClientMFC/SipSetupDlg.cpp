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
#include "SipSetupDlg.h"
#include "Setup.h"
#include "SipTransport.h"

// CSipSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSipSetupDlg, CDialog)

CSipSetupDlg::CSipSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipSetupDlg::IDD, pParent)
	, m_strSipServerIp(_T(""))
	, m_iSipServerPort(0)
	, m_strSipDomain(_T(""))
	, m_strUserId(_T(""))
	, m_strPassWord(_T(""))
	, m_strUserAgent(_T(""))
	, m_strLocalIp(_T(""))
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
	DDX_Text(pDX, IDC_SIP_DOMAIN2, m_strUserAgent);
	DDX_Control(pDX, IDC_PROTOCOL, m_clsProtocol);
	DDX_Text(pDX, IDC_LOCAL_IP, m_strLocalIp);
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
	m_strUserAgent = gclsSetup.m_strUserAgent.c_str();

	m_clsProtocol.AddString( S_SIP_UDP );
	m_clsProtocol.AddString( S_SIP_TCP );

	const char * pszProtocol = gclsSetup.m_strProtocol.c_str();

	if( !strcmp( pszProtocol, S_SIP_TCP ) )
	{
		m_clsProtocol.SetCurSel(1);
	}
	else
	{
		m_clsProtocol.SetCurSel(0);
	}

	m_strLocalIp = gclsSetup.m_strLocalIp.c_str();

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
	gclsSetup.m_strUserAgent = m_strUserAgent;

	int iSel = m_clsProtocol.GetCurSel();
	
	switch( iSel )
	{
	case 0: gclsSetup.m_strProtocol = S_SIP_UDP; break;
	case 1: gclsSetup.m_strProtocol = S_SIP_TCP; break;
	}

	gclsSetup.m_strLocalIp = m_strLocalIp;

	gclsSetup.Put();

	OnOK();
}

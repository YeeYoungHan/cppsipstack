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
#include "ServerMonitor.h"
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

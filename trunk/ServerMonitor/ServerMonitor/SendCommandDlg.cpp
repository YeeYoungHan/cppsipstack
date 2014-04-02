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
#include "SendCommandDlg.h"
#include "TcpSocket.h"
#include "MonitorSetup.h"

// CSendCommandDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSendCommandDlg, CDialog)

CSendCommandDlg::CSendCommandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendCommandDlg::IDD, pParent)
{

}

CSendCommandDlg::~CSendCommandDlg()
{
}

void CSendCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_clsCommand);
}


BEGIN_MESSAGE_MAP(CSendCommandDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSendCommandDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSendCommandDlg 메시지 처리기입니다.

void CSendCommandDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CString strCommand;

	int iCurSel = m_clsCommand.GetCurSel();
	if( iCurSel == LB_ERR )
	{
		m_clsCommand.GetWindowText( strCommand );

		if( strCommand.GetLength() == 0 )
		{
			MessageBox( "command is not inserted", "Error", MB_OK | MB_ICONERROR );
			return;
		}
	}
	else
	{
		m_clsCommand.GetLBText( iCurSel, strCommand );
	}

	gclsSocket.SendCommand( strCommand );

	OnOK();
}

BOOL CSendCommandDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( gclsMonitorSetup.m_clsSendCommandList.size() > 0 )
	{
		SEND_COMMAND_LIST::iterator	it;

		for( it = gclsMonitorSetup.m_clsSendCommandList.begin(); it != gclsMonitorSetup.m_clsSendCommandList.end(); ++it )
		{
			m_clsCommand.AddString( it->c_str() );
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

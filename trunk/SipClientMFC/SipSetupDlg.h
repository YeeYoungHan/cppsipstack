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

#pragma once

// CSipSetupDlg ��ȭ �����Դϴ�.

class CSipSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSipSetupDlg)

public:
	CSipSetupDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSipSetupDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SIPSETUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_strSipServerIp;
	int m_iSipServerPort;
	CString m_strSipDomain;
	CString m_strUserId;
	CString m_strPassWord;
};

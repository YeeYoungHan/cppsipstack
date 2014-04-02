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
#include "afxwin.h"


// CSendCommandDlg ��ȭ �����Դϴ�.

class CSendCommandDlg : public CDialog
{
	DECLARE_DYNAMIC(CSendCommandDlg)

public:
	CSendCommandDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSendCommandDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SEND_COMMAND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_clsCommand;
	virtual BOOL OnInitDialog();
};

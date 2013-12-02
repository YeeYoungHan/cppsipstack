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

#include "SipSpeedVersion.h"

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
	CFont m_clsFont;

	DECLARE_MESSAGE_MAP()
public:
	CString m_strVersion;
	afx_msg void OnStnClickedBlog();
	afx_msg void OnStnClickedHomepage();
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strVersion(SIP_SPEED_VERSION)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_STN_CLICKED(IDC_BLOG, &CAboutDlg::OnStnClickedBlog)
	ON_STN_CLICKED(IDC_HOMEPAGE, &CAboutDlg::OnStnClickedHomepage)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT sttFont;

	GetFont()->GetLogFont(&sttFont);
	sttFont.lfUnderline = TRUE;
	m_clsFont.CreateFontIndirect(&sttFont);

	GetDlgItem(IDC_BLOG)->SetFont(&m_clsFont);
	GetDlgItem(IDC_HOMEPAGE)->SetFont(&m_clsFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAboutDlg::OnStnClickedBlog()
{
	ShowWebBrowser( "http://blog.naver.com/websearch" );
}

void CAboutDlg::OnStnClickedHomepage()
{
	ShowWebBrowser( "http://dev.naver.com/projects/sipstack" );
}

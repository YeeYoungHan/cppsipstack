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

#include "SipUserAgent.h"
#include "afxwin.h"

// CSipSendDlg dialog
class CSipSendDlg : public CDialog, ISipUserAgentCallBack, ISipStackCallBack
{
// Construction
public:
	CSipSendDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SIPSEND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strSipServerIp;
	int m_iSipServerPort;
	CString m_strSipDomain;
	CString m_strCallerId;
	CString m_strCallerPassWord;
	CString m_strSip;

	BOOL m_bChangeCallId;
	BOOL m_bChangeViaBranch;

	CButton m_btnStartSipStack;
	CButton m_btnStopSipStack;

	LRESULT OnSipMessage( WPARAM wParam, LPARAM lParam );

	virtual void EventRegister( CSipServerInfo * pclsInfo, int iStatus );
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp );
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus );

	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage );
	virtual bool SendTimeout( int iThreadId, CSipMessage * pclsMessage );

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedStartSipStack();
	afx_msg void OnBnClickedStopSipStack();
	afx_msg void OnBnClickedSend();

	bool CheckInput( CString & strInput, const char * pszName );

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_txtSip;
};

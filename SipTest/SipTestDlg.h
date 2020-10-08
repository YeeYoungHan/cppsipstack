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
#include "SipUserAgentMFC.h"
#include "afxwin.h"

// CSipTestDlg dialog
class CSipTestDlg : public CDialog, ISipUserAgentCallBack
{
// Construction
public:
	CSipTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SIPTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	CSipUserAgentMFC m_clsSipUserAgentMFC;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// SipTestDlgSip.hpp
	bool	m_bCallerLogin, m_bCalleeLogin, m_bCallee2Login;
	bool	m_bTest;

	LRESULT OnSipMessage( WPARAM wParam, LPARAM lParam );

	virtual void EventRegister( CSipServerInfo * pclsInfo, int iStatus );
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp );
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus );
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRemoteRtp, CSipCallRtp * pclsLocalRtp );
	virtual bool EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer );
	virtual bool EventBlindTransfer( const char * pszCallId, const char * pszReferToId );
	virtual void EventTransferResponse( const char * pszCallId, int iSipStatus );
	virtual bool EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage );

	// SipTestDlgUtil.hpp
	CSipMutex	m_clsMutex;

	bool CheckInput( CString & strInput, const char * pszName );
	void SetLog( const char * fmt, ... );
	void SetPercent( );

	// SipTestDlg.cpp
	CString m_strSipServerIp;
	int m_iSipServerPort;
	CString m_strSipDomain;
	CString m_strCallerId;
	CString m_strCallerPassWord;
	CString m_strCalleeId;
	CString m_strCalleePassWord;
	CButton m_btnStartSipStack;
	CButton m_btnStopSipStack;
	CButton m_btnStartTest;
	CButton m_btnStopTest;
	CString m_strLog;
	CProgressCtrl m_clsProgress;
	CString m_strPercent;
	CEdit m_txtLog;
	BOOL m_bUse2Media;

	LRESULT OnTestMessage( WPARAM wParam, LPARAM lParam );
	afx_msg void OnBnClickedStartSipStack();
	afx_msg void OnBnClickedStopSipStack();
	afx_msg void OnBnClickedStartTest();
	afx_msg void OnBnClickedStopTest();
	afx_msg void OnBnClickedClearLog();
	afx_msg void OnDestroy();
	CButton m_chkUse2Media;
	CString m_strCalleeId2;
	CString m_strCalleePassWord2;
	BOOL m_bCallEstablishedTest;
	BOOL m_bCallCancelTest;
	BOOL m_bCallDeclineTest;
	BOOL m_bCallBlindTransferTest;
	BOOL m_bCallScreenedTransferTest;
};

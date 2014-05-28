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
#include "SipUserAgent.h"
#include "SipUserAgentMFC.h"

// CSipClientMFCDlg dialog
class CSipClientMFCDlg : public CDialog, ISipUserAgentCallBack
{
// Construction
public:
	CSipClientMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SIPCLIENTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	CSipUserAgentMFC m_clsSipUserAgentMFC;
	CSipUserAgent		 m_clsSipUserAgent;
	std::string			 m_strLocalIp;
	std::string			 m_strCallId;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CButton m_btnStartStack;
	CButton m_btnStopStack;
	CButton m_btnStartCall;
	CButton m_btnStopCall;
	CButton m_btnAcceptCall;
	CString m_strLog;
	CString m_strCallNumber;

	afx_msg void OnBnClickedSetup();
	afx_msg void OnBnClickedStartStack();
	afx_msg void OnBnClickedStopStack();
	afx_msg void OnBnClickedStartCall();
	afx_msg void OnBnClickedStopCall();
	afx_msg void OnBnClickedAcceptCall();

	LRESULT OnSipMessage( WPARAM wParam, LPARAM lParam );
	void SetLog( const char * fmt, ... );

	virtual void EventRegister( CSipServerInfo * pclsInfo, int iStatus );
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp );
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus );
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual bool EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer );
	virtual bool EventBlindTransfer( const char * pszCallId, const char * pszReferToId );
	virtual bool EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage );
	afx_msg void OnDestroy();
};

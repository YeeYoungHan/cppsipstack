
// SipSpeedDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CSipSpeedDlg dialog
class CSipSpeedDlg : public CDialog
{
// Construction
public:
	CSipSpeedDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SIPSPEED_DIALOG };

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
	CString m_strCalleeId;
	CString m_strCalleePassWord;
	int m_iCallTotalCount;
	int m_iCallConcurrentCount;
	CButton m_btnStartSipStack;
	CButton m_btnStopSipStack;
	CButton m_btnStartTest;
	CButton m_btnStopTest;
	afx_msg void OnBnClickedStartSipStack();
	afx_msg void OnBnClickedStopSipStack();
	afx_msg void OnBnClickedStartTest();
	afx_msg void OnBnClickedStopTest();
};

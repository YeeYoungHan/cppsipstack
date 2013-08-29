
// SipClientMFCDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CSipClientMFCDlg dialog
class CSipClientMFCDlg : public CDialog
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

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnBnClickedSetup();
	CButton m_btnStartStack;
	CButton m_btnStopStack;
	CButton m_btnStartCall;
	CButton m_btnStopCall;
	CButton m_btnAcceptCall;
	afx_msg void OnBnClickedStartStack();
	afx_msg void OnBnClickedStopStack();
	afx_msg void OnBnClickedStartCall();
	afx_msg void OnBnClickedStopCall();
	afx_msg void OnBnClickedAcceptCall();
};

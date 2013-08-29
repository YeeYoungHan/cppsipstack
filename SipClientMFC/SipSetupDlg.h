#pragma once


// CSipSetupDlg 대화 상자입니다.

class CSipSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSipSetupDlg)

public:
	CSipSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSipSetupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SIPSETUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

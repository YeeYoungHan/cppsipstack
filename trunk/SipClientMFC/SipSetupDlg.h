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

#pragma once


// CSetupDlg ��ȭ �����Դϴ�.

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSetupDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_iPeriod;
	afx_msg void OnBnClickedOk();
};

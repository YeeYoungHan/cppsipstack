#pragma once


// CSetupDlg 대화 상자입니다.

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iPeriod;
	afx_msg void OnBnClickedOk();
};

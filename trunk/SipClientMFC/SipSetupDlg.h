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
};

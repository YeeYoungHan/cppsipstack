// SipSetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SipClientMFC.h"
#include "SipSetupDlg.h"


// CSipSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSipSetupDlg, CDialog)

CSipSetupDlg::CSipSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipSetupDlg::IDD, pParent)
{

}

CSipSetupDlg::~CSipSetupDlg()
{
}

void CSipSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSipSetupDlg, CDialog)
END_MESSAGE_MAP()


// CSipSetupDlg 메시지 처리기입니다.


// KSipServerMonitorDoc.cpp : CKSipServerMonitorDoc Ŭ������ ����
//

#include "stdafx.h"
#include "KSipServerMonitor.h"

#include "KSipServerMonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKSipServerMonitorDoc

IMPLEMENT_DYNCREATE(CKSipServerMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CKSipServerMonitorDoc, CDocument)
END_MESSAGE_MAP()


// CKSipServerMonitorDoc ����/�Ҹ�

CKSipServerMonitorDoc::CKSipServerMonitorDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CKSipServerMonitorDoc::~CKSipServerMonitorDoc()
{
}

BOOL CKSipServerMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CKSipServerMonitorDoc serialization

void CKSipServerMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CKSipServerMonitorDoc ����

#ifdef _DEBUG
void CKSipServerMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKSipServerMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKSipServerMonitorDoc ���

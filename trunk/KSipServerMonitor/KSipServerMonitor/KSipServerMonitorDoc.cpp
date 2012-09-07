
// KSipServerMonitorDoc.cpp : CKSipServerMonitorDoc 클래스의 구현
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


// CKSipServerMonitorDoc 생성/소멸

CKSipServerMonitorDoc::CKSipServerMonitorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CKSipServerMonitorDoc::~CKSipServerMonitorDoc()
{
}

BOOL CKSipServerMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CKSipServerMonitorDoc serialization

void CKSipServerMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CKSipServerMonitorDoc 진단

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


// CKSipServerMonitorDoc 명령

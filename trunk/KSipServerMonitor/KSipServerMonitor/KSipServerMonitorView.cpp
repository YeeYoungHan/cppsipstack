
// KSipServerMonitorView.cpp : CKSipServerMonitorView Ŭ������ ����
//

#include "stdafx.h"
#include "KSipServerMonitor.h"

#include "KSipServerMonitorDoc.h"
#include "KSipServerMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKSipServerMonitorView

IMPLEMENT_DYNCREATE(CKSipServerMonitorView, CListView)

BEGIN_MESSAGE_MAP(CKSipServerMonitorView, CListView)
END_MESSAGE_MAP()

// CKSipServerMonitorView ����/�Ҹ�

CKSipServerMonitorView::CKSipServerMonitorView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CKSipServerMonitorView::~CKSipServerMonitorView()
{
}

BOOL CKSipServerMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CListView::PreCreateWindow(cs);
}

void CKSipServerMonitorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: GetListCtrl()�� ȣ���Ͽ� �ش� list ��Ʈ���� ���� �׼��������ν�
	//  ListView�� �׸����� ä�� �� �ֽ��ϴ�.
}

void CKSipServerMonitorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKSipServerMonitorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CKSipServerMonitorView ����

#ifdef _DEBUG
void CKSipServerMonitorView::AssertValid() const
{
	CListView::AssertValid();
}

void CKSipServerMonitorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CKSipServerMonitorDoc* CKSipServerMonitorView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKSipServerMonitorDoc)));
	return (CKSipServerMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CKSipServerMonitorView �޽��� ó����

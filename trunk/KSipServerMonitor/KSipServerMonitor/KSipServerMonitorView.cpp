/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "stdafx.h"
#include "KSipServerMonitor.h"

#include "KSipServerMonitorDoc.h"
#include "KSipServerMonitorView.h"

#include "Setup.h"
#include "TcpSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKSipServerMonitorView

IMPLEMENT_DYNCREATE(CKSipServerMonitorView, CListView)

BEGIN_MESSAGE_MAP(CKSipServerMonitorView, CListView)
END_MESSAGE_MAP()

// CKSipServerMonitorView ����/�Ҹ�

CKSipServerMonitorView::CKSipServerMonitorView() : m_bInit(false)
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

	if( m_bInit == false )
	{
		CListCtrl & clsListCtrl = GetListCtrl();

		ModifyStyle( LVS_ICON, LVS_REPORT );

		switch( GetDocument()->m_eType )
		{
		case E_COMM_SIP_STACK_COUNT_LIST:
			GetDocument()->SetTitle( __T("Transaction List") );
			clsListCtrl.InsertColumn(  0, _T("ICT")    , LVCFMT_LEFT, gclsSetup.GetInt( "tran",  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("NICT")   , LVCFMT_LEFT, gclsSetup.GetInt( "tran",  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("IST")    , LVCFMT_LEFT, gclsSetup.GetInt( "tran",  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("NIST")   , LVCFMT_LEFT, gclsSetup.GetInt( "tran",  3, 100 ) );
			clsListCtrl.InsertColumn(  4, _T("Delete") , LVCFMT_LEFT, gclsSetup.GetInt( "tran",  4, 100 ) );
			break;
		}

		gclsSocket.AddCommand( GetDocument()->m_eType, &clsListCtrl );
		m_bInit = true;
	}
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

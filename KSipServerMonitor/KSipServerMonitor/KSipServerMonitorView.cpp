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

// CKSipServerMonitorView 생성/소멸

CKSipServerMonitorView::CKSipServerMonitorView() : m_bInit(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CKSipServerMonitorView::~CKSipServerMonitorView()
{
}

BOOL CKSipServerMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

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
		case E_COMM_CALL_LIST:
			GetDocument()->SetTitle( _T("Call List") );
			clsListCtrl.InsertColumn(  0, _T("Call-Id"), LVCFMT_LEFT, gclsSetup.GetInt( "call",  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("Call-Id"), LVCFMT_LEFT, gclsSetup.GetInt( "call",  1, 100 ) );
			break;
		case E_COMM_SIP_SERVER_LIST:
			GetDocument()->SetTitle( _T("SipServer List") );
			clsListCtrl.InsertColumn(  0, _T("key")    , LVCFMT_LEFT, gclsSetup.GetInt( "sipserver",  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("name")   , LVCFMT_LEFT, gclsSetup.GetInt( "sipserver",  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("flag")   , LVCFMT_LEFT, gclsSetup.GetInt( "sipserver",  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("ip")     , LVCFMT_LEFT, gclsSetup.GetInt( "sipserver",  3, 100 ) );
			clsListCtrl.InsertColumn(  4, _T("domain") , LVCFMT_LEFT, gclsSetup.GetInt( "sipserver",  4, 100 ) );
			clsListCtrl.InsertColumn(  5, _T("userid") , LVCFMT_LEFT, gclsSetup.GetInt( "sipserver",  5, 100 ) );
			clsListCtrl.InsertColumn(  6, _T("timeout"), LVCFMT_LEFT, gclsSetup.GetInt( "sipserver",  6, 100 ) );
			clsListCtrl.InsertColumn(  7, _T("login")  , LVCFMT_LEFT, gclsSetup.GetInt( "sipserver",  7, 100 ) );
			break;
		case E_COMM_USER_LIST:
			GetDocument()->SetTitle( _T("User List") );
			clsListCtrl.InsertColumn(  0, _T("id")        , LVCFMT_LEFT, gclsSetup.GetInt( "user",  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("ip")        , LVCFMT_LEFT, gclsSetup.GetInt( "user",  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("login time"), LVCFMT_LEFT, gclsSetup.GetInt( "user",  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("timeout")   , LVCFMT_LEFT, gclsSetup.GetInt( "user",  3, 100 ) );
			break;
		case E_COMM_DIALOG_LIST:
			GetDocument()->SetTitle( _T("Dialog List") );
			clsListCtrl.InsertColumn(  0, _T("Call-Id")     , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("From")        , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("To")          , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("ContactIp")   , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  3, 100 ) );
			clsListCtrl.InsertColumn(  4, _T("RtpLocalIp")  , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  4, 100 ) );
			clsListCtrl.InsertColumn(  5, _T("RtpRemoteIp") , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  5, 100 ) );
			clsListCtrl.InsertColumn(  6, _T("InviteTime")  , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  6, 100 ) );
			clsListCtrl.InsertColumn(  7, _T("StartTime")   , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  7, 100 ) );
			clsListCtrl.InsertColumn(  8, _T("EndTime")     , LVCFMT_LEFT, gclsSetup.GetInt( "dialog",  8, 100 ) );
			break;
		case E_COMM_SIP_STACK_COUNT_LIST:
			GetDocument()->SetTitle( _T("Transaction List") );
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


// CKSipServerMonitorView 진단

#ifdef _DEBUG
void CKSipServerMonitorView::AssertValid() const
{
	CListView::AssertValid();
}

void CKSipServerMonitorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CKSipServerMonitorDoc* CKSipServerMonitorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKSipServerMonitorDoc)));
	return (CKSipServerMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CKSipServerMonitorView 메시지 처리기

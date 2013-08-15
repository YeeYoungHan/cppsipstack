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
#include "SipLoadBalancerMonitor.h"

#include "SipLoadBalancerMonitorDoc.h"
#include "SipLoadBalancerMonitorView.h"

#include "Setup.h"
#include "TcpSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKSipServerMonitorView

IMPLEMENT_DYNCREATE(CKSipServerMonitorView, CListView)

BEGIN_MESSAGE_MAP(CKSipServerMonitorView, CListView)
	ON_WM_DESTROY()
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
			clsListCtrl.InsertColumn(  0, _T("Call-Id")  , LVCFMT_LEFT, gclsSetup.GetInt( ST_CALL_LIST,  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("Call-Id")  , LVCFMT_LEFT, gclsSetup.GetInt( ST_CALL_LIST,  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("Invite")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_CALL_LIST,  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("Peer-Port"), LVCFMT_LEFT, gclsSetup.GetInt( ST_CALL_LIST,  3, 100 ) );
			break;
		case E_COMM_SIP_SERVER_LIST:
			GetDocument()->SetTitle( _T("SipServer List") );
			clsListCtrl.InsertColumn(  0, _T("key")    , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("name")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("flag")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("ip")     , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  3, 100 ) );
			clsListCtrl.InsertColumn(  4, _T("domain") , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  4, 100 ) );
			clsListCtrl.InsertColumn(  5, _T("userid") , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  5, 100 ) );
			clsListCtrl.InsertColumn(  6, _T("timeout"), LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  6, 100 ) );
			clsListCtrl.InsertColumn(  7, _T("login")  , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  7, 100 ) );
			break;
		case E_COMM_USER_LIST:
			GetDocument()->SetTitle( _T("User List") );
			clsListCtrl.InsertColumn(  0, _T("id")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_USER_LIST,  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("ip")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_USER_LIST,  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("login time"), LVCFMT_LEFT, gclsSetup.GetInt( ST_USER_LIST,  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("timeout")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_USER_LIST,  3, 100 ) );
			break;
		case E_COMM_RTP_LIST:
			GetDocument()->SetTitle( _T("Rtp List") );
			clsListCtrl.InsertColumn(  0, _T("Port")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_RTP_LIST,  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("StartPort")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_RTP_LIST,  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("Rtp")         , LVCFMT_LEFT, gclsSetup.GetInt( ST_RTP_LIST,  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("Rtcp")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_RTP_LIST,  3, 100 ) );
			clsListCtrl.InsertColumn(  4, _T("Rtp")         , LVCFMT_LEFT, gclsSetup.GetInt( ST_RTP_LIST,  4, 100 ) );
			clsListCtrl.InsertColumn(  5, _T("Rtcp")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_RTP_LIST,  5, 100 ) );
			clsListCtrl.InsertColumn(  6, _T("Stop")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_RTP_LIST,  6, 100 ) );
			break;
		case E_COMM_DIALOG_LIST:
			GetDocument()->SetTitle( _T("Dialog List") );
			clsListCtrl.InsertColumn(  0, _T("Call-Id")     , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("From")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("To")          , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("ContactIp")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  3, 100 ) );
			clsListCtrl.InsertColumn(  4, _T("RtpLocalIp")  , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  4, 100 ) );
			clsListCtrl.InsertColumn(  5, _T("RtpRemoteIp") , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  5, 100 ) );
			clsListCtrl.InsertColumn(  6, _T("InviteTime")  , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  6, 100 ) );
			clsListCtrl.InsertColumn(  7, _T("StartTime")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  7, 100 ) );
			clsListCtrl.InsertColumn(  8, _T("EndTime")     , LVCFMT_LEFT, gclsSetup.GetInt( ST_DIALOG_LIST,  8, 100 ) );
			break;
		case E_COMM_SIP_STACK_COUNT_LIST:
			GetDocument()->SetTitle( _T("Transaction List") );
			clsListCtrl.InsertColumn(  0, _T("ICT")    , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_STACK_COUNT,  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("NICT")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_STACK_COUNT,  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("IST")    , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_STACK_COUNT,  2, 100 ) );
			clsListCtrl.InsertColumn(  3, _T("NIST")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_STACK_COUNT,  3, 100 ) );
			clsListCtrl.InsertColumn(  4, _T("Delete") , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_STACK_COUNT,  4, 100 ) );
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

void CKSipServerMonitorView::OnDestroy()
{
	CListCtrl & clsListCtrl = GetListCtrl();

	CHeaderCtrl * pclsHeaderCtrl = clsListCtrl.GetHeaderCtrl();
	int i, iCount = pclsHeaderCtrl->GetItemCount();
	std::string	strName;

	switch( GetDocument()->m_eType )
	{
	case E_COMM_CALL_LIST:
		strName = ST_CALL_LIST;
		break;
	case E_COMM_SIP_SERVER_LIST:
		strName = ST_SIP_SERVER_LIST;
		break;
	case E_COMM_USER_LIST:
		strName = ST_USER_LIST;
		break;
	case E_COMM_RTP_LIST:
		strName = ST_RTP_LIST;
		break;
	case E_COMM_DIALOG_LIST:
		strName = ST_DIALOG_LIST;
		break;
	case E_COMM_SIP_STACK_COUNT_LIST:
		strName = ST_SIP_STACK_COUNT;
		break;
	}

	for( i = 0; i < iCount; ++i )
	{
		gclsSetup.PutInt( strName.c_str(), i, clsListCtrl.GetColumnWidth( i ) );
	}

	CListView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

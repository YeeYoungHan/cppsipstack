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
		case E_COMM_SIP_SERVER_LIST:
			GetDocument()->SetTitle( _T("SipServer List") );
			clsListCtrl.InsertColumn(  0, _T("ip")    , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("use")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("del")   , LVCFMT_LEFT, gclsSetup.GetInt( ST_SIP_SERVER_LIST,  2, 100 ) );
			break;
		case E_COMM_USER_LIST:
			GetDocument()->SetTitle( _T("User List") );
			clsListCtrl.InsertColumn(  0, _T("id")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_USER_LIST,  0, 100 ) );
			clsListCtrl.InsertColumn(  1, _T("ip")        , LVCFMT_LEFT, gclsSetup.GetInt( ST_USER_LIST,  1, 100 ) );
			clsListCtrl.InsertColumn(  2, _T("sip server"), LVCFMT_LEFT, gclsSetup.GetInt( ST_USER_LIST,  2, 100 ) );
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

void CKSipServerMonitorView::OnDestroy()
{
	CListCtrl & clsListCtrl = GetListCtrl();

	CHeaderCtrl * pclsHeaderCtrl = clsListCtrl.GetHeaderCtrl();
	int i, iCount = pclsHeaderCtrl->GetItemCount();
	std::string	strName;

	switch( GetDocument()->m_eType )
	{
	case E_COMM_SIP_SERVER_LIST:
		strName = ST_SIP_SERVER_LIST;
		break;
	case E_COMM_USER_LIST:
		strName = ST_USER_LIST;
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

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
#include "ServerMonitor.h"

#include "ServerMonitorDoc.h"
#include "ServerMonitorView.h"

#include "Setup.h"
#include "TcpSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerMonitorView

IMPLEMENT_DYNCREATE(CServerMonitorView, CListView)

BEGIN_MESSAGE_MAP(CServerMonitorView, CListView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CServerMonitorView 생성/소멸

CServerMonitorView::CServerMonitorView() : m_bInit(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CServerMonitorView::~CServerMonitorView()
{
}

BOOL CServerMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CListView::PreCreateWindow(cs);
}

void CServerMonitorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	if( m_bInit == false )
	{
		CListCtrl & clsListCtrl = GetListCtrl();

		ModifyStyle( LVS_ICON, LVS_REPORT );

		std::string strCommand = GetDocument()->m_strCommand;
		CMonitorEntry clsEntry;
		
		gclsMonitorSetup.Select( strCommand.c_str(), clsEntry );

		GetDocument()->SetTitle( clsEntry.m_strTitle.c_str() );

		MONITOR_ATTRIBUTE_LIST::iterator	itAttribute;
		int iIndex = 0;

		for( itAttribute = clsEntry.m_lstAttribute.begin(); itAttribute != clsEntry.m_lstAttribute.end(); ++itAttribute )
		{
			clsListCtrl.InsertColumn( iIndex++, itAttribute->m_strName.c_str(), LVCFMT_LEFT, gclsSetup.GetInt( strCommand.c_str(), iIndex, 100 ) );
		}

		gclsSocket.AddCommand( strCommand.c_str(), &clsListCtrl, clsEntry );
		m_bInit = true;
	}
}

void CServerMonitorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CServerMonitorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CServerMonitorView 진단

#ifdef _DEBUG
void CServerMonitorView::AssertValid() const
{
	CListView::AssertValid();
}

void CServerMonitorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CServerMonitorDoc* CServerMonitorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServerMonitorDoc)));
	return (CServerMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CServerMonitorView 메시지 처리기

void CServerMonitorView::OnDestroy()
{
	CListCtrl & clsListCtrl = GetListCtrl();
	std::string strCommand = GetDocument()->m_strCommand;
	CHeaderCtrl * pclsHeaderCtrl = clsListCtrl.GetHeaderCtrl();
	int i, iCount = pclsHeaderCtrl->GetItemCount();

	gclsSocket.DeleteCommand( strCommand.c_str(), &clsListCtrl );

	for( i = 0; i < iCount; ++i )
	{
		gclsSetup.PutInt( strCommand.c_str(), i, clsListCtrl.GetColumnWidth( i ) );
	}

	CListView::OnDestroy();
}

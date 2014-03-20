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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerMonitorDoc

IMPLEMENT_DYNCREATE(CServerMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CServerMonitorDoc, CDocument)
END_MESSAGE_MAP()


// CServerMonitorDoc 생성/소멸

CServerMonitorDoc::CServerMonitorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CServerMonitorDoc::~CServerMonitorDoc()
{
}

BOOL CServerMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_strCommand = gclsMonitorSetup.GetNextCommand( );

	return TRUE;
}




// CServerMonitorDoc serialization

void CServerMonitorDoc::Serialize(CArchive& ar)
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


// CServerMonitorDoc 진단

#ifdef _DEBUG
void CServerMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CServerMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CServerMonitorDoc 명령

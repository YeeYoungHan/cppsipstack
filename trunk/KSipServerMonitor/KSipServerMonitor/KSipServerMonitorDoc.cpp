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

	static ECommType eType = E_COMM_CALL_LIST;

	m_eType = eType;

	switch( eType )
	{
	case E_COMM_CALL_LIST:
		eType = E_COMM_SIP_SERVER_LIST;
		break;
	case E_COMM_SIP_SERVER_LIST:
		eType = E_COMM_USER_LIST;
		break;
	case E_COMM_USER_LIST:
		eType = E_COMM_DIALOG_LIST;
		break;
	case E_COMM_DIALOG_LIST:
		eType = E_COMM_SIP_STACK_COUNT_LIST;
		break;
	case E_COMM_SIP_STACK_COUNT_LIST:
		eType = E_COMM_CALL_LIST;
		break;
	}

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

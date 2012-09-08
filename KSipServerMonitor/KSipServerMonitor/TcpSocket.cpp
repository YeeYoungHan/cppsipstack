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
#include "TcpSocket.h"
#include "SipStackDefine.h"

#include <string>

CTcpSocket gclsSocket;

// CTcpSocket

CTcpSocket::CTcpSocket() : m_iSendCommand(0)
{
}

CTcpSocket::~CTcpSocket()
{
}

// CTcpSocket 멤버 함수

void CTcpSocket::OnReceive( int nErrorCode )
{
	if( nErrorCode != 0 )
	{
		// QQQ: 프로그램을 종료한다.
		return;
	}

	int		iPacketLen, n, iRecvLen = 0;
	char	szPacket[8192];
	std::string	strPacket;

	n = Receive( &iPacketLen, sizeof(iPacketLen) );
	if( n != sizeof(iPacketLen) )
	{
		// QQQ: 프로그램을 종료한다.
		return;
	}

	iPacketLen = ntohl( iPacketLen );
	if( iPacketLen <= 0 )
	{
		// QQQ
		return;
	}

	while( iRecvLen < iPacketLen )
	{
		n = Receive( szPacket, iPacketLen - iRecvLen );
		if( n <= 0 )
		{
			// QQQ
			return;
		}

		strPacket.append( szPacket, n );
		iRecvLen += n;
	}

	CListCtrl * pclsListCtrl = NULL;

	m_clsMutex.Lock();
	int iIndex = m_clsCommandList.size() - m_iSendCommand;
	if( iIndex >= 0 )
	{
		--m_iSendCommand;

		pclsListCtrl = m_clsCommandList[iIndex].m_pclsListCtrl;
	}
	m_clsMutex.Unlock();

	if( pclsListCtrl )
	{
		ParseRecvData( strPacket.c_str(), pclsListCtrl );
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

bool CTcpSocket::AddCommand( ECommType cCommType, CListCtrl * pclsListCtrl )
{
	CMonitorCommand clsCommand;

	switch( cCommType )
	{
	case E_COMM_SIP_STACK_COUNT_LIST:
		clsCommand.m_strCommand = MC_SIP_STACK_COUNT_LIST;
		break;
	}

	clsCommand.m_pclsListCtrl = pclsListCtrl;

	m_clsMutex.Lock();
	m_clsCommandList.push_back( clsCommand );
	m_clsMutex.Unlock();

	return true;
}

bool CTcpSocket::Execute()
{
	if( m_iSendCommand > 0 ) return false;

	MONITOR_COMMAND_LIST::iterator	itList;
	int		iPacketLen, n;

	m_clsMutex.Lock();
	for( itList = m_clsCommandList.begin(); itList != m_clsCommandList.end(); ++itList )
	{
		iPacketLen = itList->m_strCommand.length();
		iPacketLen = htonl( iPacketLen );

		n = Send( &iPacketLen, sizeof(iPacketLen) );
		if( n != sizeof(iPacketLen) )
		{
			// QQQ
			break;
		}

		n = Send( itList->m_strCommand.c_str(), itList->m_strCommand.length() );
		if( n != itList->m_strCommand.length() )
		{
			// QQQ
			break;
		}

		++m_iSendCommand;
	}
	m_clsMutex.Unlock();

	return true;
}

void CTcpSocket::ParseRecvData( const char * pszBuf, CListCtrl * pclsListCtrl )
{
	int	iPos = 0, iRow = 0, iColumn = 0;
	std::string	strText;

	pclsListCtrl->DeleteAllItems();

	USES_CONVERSION;

	for( int i = 0; pszBuf[i]; ++i )
	{
		if( !strncmp( pszBuf + i, MR_ROW_SEP, 1 ) )
		{
			strText.append( pszBuf + iPos, i - iPos );
			iPos = i + 1;

			if( iColumn == 0 )
			{
				pclsListCtrl->InsertItem( iRow, A2T(strText.c_str()) );
			}
			else
			{
				pclsListCtrl->SetItemText( iRow, iColumn, A2T(strText.c_str()) );
			}

			iColumn = 0;
			++iRow;
			strText.clear();
		}
		else if( !strncmp( pszBuf + i, MR_COL_SEP, 1 ) )
		{
			strText.append( pszBuf + iPos, i - iPos );
			iPos = i + 1;

			if( iColumn == 0 )
			{
				pclsListCtrl->InsertItem( iRow, A2T(strText.c_str()) );
			}
			else
			{
				pclsListCtrl->SetItemText( iRow, iColumn, A2T(strText.c_str()) );
			}

			++iColumn;
			strText.clear();
		}
	}

	CString			strBuf;
	SYSTEMTIME	sttTime;

	strBuf.Format( _T("count=%d"), pclsListCtrl->GetItemCount() );
	pclsListCtrl->InsertItem( iRow, strBuf );

	GetLocalTime( &sttTime );

	strBuf.Format( _T("%02d:%02d:%02d.%03d"), sttTime.wHour, sttTime.wMinute, sttTime.wSecond, sttTime.wMilliseconds );
	pclsListCtrl->SetItemText( iRow, 1, strBuf );
}

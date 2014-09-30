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
#include "TcpSocket.h"
#include "SipStackDefine.h"

#include <string>

CTcpSocket gclsSocket;

CTcpSocket::CTcpSocket() : m_hSocket(INVALID_SOCKET), m_iPort(0)
{
	memset( m_szSendPacket, 0, sizeof(m_szSendPacket) );
}

CTcpSocket::~CTcpSocket()
{
}

/**
 * @ingroup ServerMonitor
 * @brief ����͸� ���� ��Ʈ�� �����Ѵ�.
 * @param pszIp ���� IP �ּ�
 * @param iPort ���� ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocket::Connect( const char * pszIp, int iPort )
{
	m_clsMutex.Lock();
	m_hSocket = TcpConnect( pszIp, iPort, TCP_TIMEOUT );
	m_clsMutex.Unlock();

	if( m_hSocket == INVALID_SOCKET ) return false;

	m_strIp = pszIp;
	m_iPort = iPort;

	return true;
}

/**
 * @ingroup ServerMonitor
 * @brief ����͸� ���� ��Ʈ�� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocket::Connect( )
{
	if( m_hSocket != INVALID_SOCKET ) return false;

	m_clsMutex.Lock();
	m_hSocket = TcpConnect( m_strIp.c_str(), m_iPort, TCP_TIMEOUT );
	m_clsMutex.Unlock();

	if( m_hSocket == INVALID_SOCKET ) return false;

	return true;
}

/**
 * @ingroup ServerMonitor
 * @brief ���� ������ �����Ѵ�.
 */
void CTcpSocket::Close( )
{
	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;

		m_clsMutex.Lock();
		m_clsSendCommandList.clear();
		m_clsMutex.Unlock();
	}
}

/**
 * @ingroup ServerMonitor
 * @brief ���� �̺�Ʈ�� ����Ѵ�.
 * @returns ���� �̺�Ʈ�� �߻��ϸ� ����� �����ϰ� �׷��� ������ 0 �Ǵ� -1 �� �����Ѵ�.
 */
int CTcpSocket::Poll( )
{
	struct pollfd arrPoll[1];

	TcpSetPollIn( arrPoll[0], m_hSocket );

	return poll( arrPoll, 1, 100 );
}

/**
 * @ingroup ServerMonitor
 * @brief ����͸� ����� �����ϰ� ȭ�鿡 �����ش�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocket::Receive( )
{
	int		iPacketLen, n, iRecvLen = 0, iLen;
	char	szPacket[8192];
	std::string	strPacket;

	n = TcpRecv( m_hSocket, (char *)&iPacketLen, sizeof(iPacketLen), TCP_TIMEOUT );
	if( n != sizeof(iPacketLen) )
	{
		TRACE( "Recv packet length error\n" );
		return false;
	}

	iPacketLen = ntohl( iPacketLen );
	if( iPacketLen > 0 )
	{
		while( iRecvLen < iPacketLen )
		{
			if( ( iPacketLen - iRecvLen ) >= sizeof(szPacket) )
			{
				iLen = sizeof(szPacket);
			}
			else
			{
				iLen = iPacketLen - iRecvLen;
			}

			// -1 �� ���ϵȴ�. Network ���ϰ� �߻��Ͽ��� timeout �Ǵ� �� ����.
			n = TcpRecv( m_hSocket, szPacket, iLen, TCP_TIMEOUT );
			if( n <= 0 )
			{
				TRACE( "Recv packet body error\n" );
				return false;
			}

			strPacket.append( szPacket, n );
			iRecvLen += n;
		}
	}

	CMonitorCommand clsCommand;

	m_clsMutex.Lock();
	if( m_clsSendCommandList.empty() == false )
	{
		clsCommand = m_clsSendCommandList.front();
		m_clsSendCommandList.pop_front();

		TRACE( "Recv command(%s)\n", clsCommand.m_strCommand.c_str() );
	}
	m_clsMutex.Unlock();

	if( clsCommand.m_pclsListCtrl )
	{
		if( clsCommand.m_clsEntry.m_bUpdateRow == false )
		{
			clsCommand.m_pclsListCtrl->DeleteAllItems();
		}

		if( iPacketLen > 0 )
		{
			ParseRecvData( strPacket.c_str(), clsCommand );
		}

		CString			strBuf;
		SYSTEMTIME	sttTime;
		int					iRow = clsCommand.m_pclsListCtrl->GetItemCount();
		bool				bInsertItem = false;

		strBuf.Format( _T("count=%d"), iRow );

		if( clsCommand.m_clsEntry.m_bUpdateRow )
		{
			if( iRow > 0 )
			{
				CString strText = clsCommand.m_pclsListCtrl->GetItemText( iRow - 1, 0 );
				if( !strncmp( strText, "count=", 6 ) )
				{
					iRow = iRow - 1;
					bInsertItem = true;
				}
			}
		}

		if( bInsertItem == false )
		{
			clsCommand.m_pclsListCtrl->InsertItem( iRow, strBuf );
		}

		GetLocalTime( &sttTime );

		strBuf.Format( _T("%02d:%02d:%02d.%03d"), sttTime.wHour, sttTime.wMinute, sttTime.wSecond, sttTime.wMilliseconds );
		clsCommand.m_pclsListCtrl->SetItemText( iRow, 1, strBuf );
	}

	return true;
}

/**
 * @ingroup ServerMonitor
 * @brief ����� �߰��Ѵ�.
 * @param pszCommand ���
 * @param pclsListCtrl ��� ����� ������ CListCtrl
 * @param clsEntry			����͸� ��� ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocket::AddCommand( const char * pszCommand, CListCtrl * pclsListCtrl, CMonitorEntry & clsEntry )
{
	if( m_hSocket == INVALID_SOCKET ) return false;

	CMonitorCommand clsCommand;

	clsCommand.m_strCommand = pszCommand;
	clsCommand.m_pclsListCtrl = pclsListCtrl;
	clsCommand.m_clsEntry = clsEntry;

	m_clsMutex.Lock();
	m_clsCommandList.push_back( clsCommand );
	m_clsMutex.Unlock();

	return true;
}

/**
 * @ingroup ServerMonitor
 * @brief ����� �����Ѵ�.
 * @param pszCommand ���
 * @param pclsListCtrl ��� ����� ������ CListCtrl
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocket::DeleteCommand( const char * pszCommand, CListCtrl * pclsListCtrl )
{
	MONITOR_COMMAND_LIST::iterator	itList;
	bool bRes = false;

	m_clsMutex.Lock();
	for( itList = m_clsSendCommandList.begin(); itList != m_clsSendCommandList.end(); ++itList )
	{
		if( itList->m_pclsListCtrl == pclsListCtrl )
		{
			m_clsSendCommandList.erase( itList );
			break;
		}
	}

	for( itList = m_clsCommandList.begin(); itList != m_clsCommandList.end(); ++itList )
	{
		if( itList->m_pclsListCtrl == pclsListCtrl )
		{
			m_clsCommandList.erase( itList );
			bRes = true;
			break;
		}
	}
	m_clsMutex.Unlock();

	return bRes;
}

/**
 * @ingroup ServerMonitor
 * @brief ����͸� ����� ��û�Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocket::Execute()
{
	int iSendCount;

	m_clsMutex.Lock();
	iSendCount = m_clsSendCommandList.size();
	m_clsMutex.Unlock();

	if( iSendCount > 0 ) return false;

	MONITOR_COMMAND_LIST::iterator	itList;
	int		iPacketLen, n;
	bool	bError = false;

	m_clsMutex.Lock();
	for( itList = m_clsCommandList.begin(); itList != m_clsCommandList.end(); ++itList )
	{
		iPacketLen = itList->m_strCommand.length();
		int iTemp = htonl( iPacketLen );

		memcpy( m_szSendPacket, &iTemp, sizeof(iTemp) );
		memcpy( m_szSendPacket + sizeof(iTemp), itList->m_strCommand.c_str(), itList->m_strCommand.length() );

		iPacketLen += sizeof(iTemp);

		n = TcpSend( m_hSocket, m_szSendPacket, iPacketLen );
		if( n != iPacketLen )
		{
			TRACE( "Send command(%s) error\n", itList->m_strCommand.c_str() );
			bError = true;
			break;
		}

		TRACE( "Send command(%s)\n", itList->m_strCommand.c_str() );

		m_clsSendCommandList.push_back( *itList );
	}
	m_clsMutex.Unlock();

	if( bError )
	{
		Close();
		return false;
	}

	return true;
}

/**
 * @ingroup ServerMonitor
 * @brief ����� �����Ѵ�.
 * @param pszCommand ��� ���ڿ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocket::SendCommand( const char * pszCommand )
{
	std::string strCommand = pszCommand;
	int		iPacketLen, n;
	bool	bError = false;

	if( strCommand.length() > ( sizeof(m_szSendPacket) - 4 ) ) 
	{
		TRACE( "Send command(%s) length error\n", strCommand.c_str() );
		return false;
	}

	m_clsMutex.Lock();
	iPacketLen = strCommand.length();
	int iTemp = htonl( iPacketLen );

	memcpy( m_szSendPacket, &iTemp, sizeof(iTemp) );
	memcpy( m_szSendPacket + sizeof(iTemp), pszCommand, strCommand.length() );

	iPacketLen += sizeof(iTemp);

	n = TcpSend( m_hSocket, m_szSendPacket, iPacketLen );
	if( n != iPacketLen )
	{
		TRACE( "Send command(%s) error\n", strCommand.c_str() );
		bError = true;
	}
	else
	{
		TRACE( "Send command(%s)\n", strCommand.c_str() );
	}
	m_clsMutex.Unlock();

	if( bError ) return false;

	return true;
}

static void CommaSepString( std::string & strText )
{
	int iLen = (int)strText.length();
	int iCount = 0;

	for( int i = iLen - 1; i > 0; --i )
	{
		if( i % 3 == 0 )
		{
			strText.insert( iLen - i + iCount, "," );
			++iCount;
		}
	}
}

/**
 * @ingroup ServerMonitor
 * @brief ����͸� ����� ����Ʈ �信 �����ش�.
 * @param pszBuf			����͸� ��� ���ڿ�
 * @param clsCommand	���� ��� ���� ��ü
 * @param iPos				����͸� ��� ���ڿ��� ���� ��ġ
 * @param i						����͸� ��� ���ڿ��� ���� ��ġ
 * @param iRow				����Ʈ ���� row
 * @param iColumn			����Ʈ ���� column
 */
void CTcpSocket::SetItemText( const char * pszBuf, CMonitorCommand & clsCommand, int & iPos, int & i, int iRow, int iColumn )
{
	std::string strText;

	strText.append( pszBuf + iPos, i - iPos );
	iPos = i + 1;

	EMonitorAttributeType eType = clsCommand.m_clsEntry.GetType( iColumn );
	if( eType == E_MAT_COMMA_SEP )
	{
		CommaSepString( strText );
	}

	if( iColumn == 0 )
	{
		if( clsCommand.m_clsEntry.m_bUpdateRow )
		{
			int iRowCount = clsCommand.m_pclsListCtrl->GetItemCount();

			if( iRowCount == iRow )
			{
				clsCommand.m_pclsListCtrl->InsertItem( iRow, strText.c_str() );
			}
			else
			{
				clsCommand.m_pclsListCtrl->SetItemText( iRow, iColumn, strText.c_str() );
			}
		}
		else
		{
			clsCommand.m_pclsListCtrl->InsertItem( iRow, strText.c_str() );
		}
	}
	else
	{
		clsCommand.m_pclsListCtrl->SetItemText( iRow, iColumn, strText.c_str() );
	}
}

/**
 * @ingroup ServerMonitor
 * @brief ���ŵ� ����͸� ����� �Ľ��Ͽ��� CListCtrl �� �����ش�.
 * @param pszBuf ���ŵ� ����͸� ���
 * @param clsCommand ����͸� ��� ���� ��ü
 */
void CTcpSocket::ParseRecvData( const char * pszBuf, CMonitorCommand & clsCommand )
{
	int	iPos = 0, iRow = 0, iColumn = 0;

	USES_CONVERSION;

	for( int i = 0; pszBuf[i]; ++i )
	{
		if( !strncmp( pszBuf + i, MR_ROW_SEP, 1 ) )
		{
			SetItemText( pszBuf, clsCommand, iPos, i, iRow, iColumn );

			iColumn = 0;
			++iRow;
		}
		else if( !strncmp( pszBuf + i, MR_COL_SEP, 1 ) )
		{
			SetItemText( pszBuf, clsCommand, iPos, i, iRow, iColumn );

			++iColumn;
		}
	}
}

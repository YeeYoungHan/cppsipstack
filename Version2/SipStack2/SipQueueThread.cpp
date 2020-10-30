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

#include "SipStackThread.h"
#include "SipQueue.h"
#include "TimeUtility.h"
#include "ServerUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipStack
 * @brief SIP 큐에서 SIP 메시지를 가져와서 파싱 및 callback 처리하는 쓰레드
 * @param lpParameter SIP stack 객체
 * @returns 0 을 리턴한다.
 */
THREAD_API SipQueueThread( LPVOID lpParameter )
{
	CSipStack * pclsSipStack = (CSipStack *)lpParameter;
	CSipQueueEntry * pclsEntry = NULL;
	int		iThreadId;

	pclsSipStack->IncreateTcpThreadCount( iThreadId );

	while( pclsSipStack->m_bStopEvent == false )
	{
		if( gclsSipQueue.Select( &pclsEntry ) )
		{
			pclsSipStack->RecvSipMessage( iThreadId, pclsEntry->m_strBuf.c_str(), (int)pclsEntry->m_strBuf.length()
				, pclsEntry->m_strIp.c_str(), pclsEntry->m_sPort, pclsEntry->m_eTransport );

			delete pclsEntry;
			pclsEntry = NULL;
		}
	}

	pclsSipStack->ThreadEnd( -1 );
	pclsSipStack->DecreateTcpThreadCount();

	return 0;
}

/**
 * @ingroup SipStack
 * @brief SIP 큐에서 SIP 메시지를 가져와서 파싱 및 callback 처리하는 쓰레드를 시작한다.
 * @param pclsSipStack SIP stack 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartSipQueueThread( CSipStack * pclsSipStack )
{
	return StartThread( "SipQueueThread", SipQueueThread, pclsSipStack );
}

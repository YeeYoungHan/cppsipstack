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

#include "SipRegisterThread.h"
#include "ServerUtility.h"
#include <time.h>
#include "MemoryDebug.h"

/**
 * @ingroup SipUserAgent
 * @brief SIP 서버에 주기적으로 REGISTER 메시지를 전송하는 쓰레드
 * @param lpParameter CSipUserAgent 객체의 포인터
 * @returns 0 을 리턴한다.
 */
THREAD_API SipRegisterThread( LPVOID lpParameter )
{
	CSipUserAgent * pclsSipUserAgent = (CSipUserAgent *)lpParameter;
	SIP_SERVER_INFO_LIST::iterator itList;
	time_t	iTime;

	while( pclsSipUserAgent->m_bStopEvent == false )
	{
		time( &iTime );

		pclsSipUserAgent->m_clsRegisterMutex.acquire();
		for( itList = pclsSipUserAgent->m_clsRegisterList.begin(); itList != pclsSipUserAgent->m_clsRegisterList.end(); ++itList )
		{
			if( itList->m_bLogin == false )
			{
				if( itList->m_iSendTime == 0 )
				{
					if( itList->m_iNextSendTime != 0 )
					{
						if( itList->m_iNextSendTime > iTime ) continue;
					}

					CSipMessage * pclsRequest = itList->CreateRegister( &pclsSipUserAgent->m_clsSipStack, NULL );
					if( pclsRequest )
					{
						if( pclsSipUserAgent->m_clsSipStack.SendSipMessage( pclsRequest ) )
						{
							itList->m_iSendTime = iTime;
							itList->m_iResponseTime = 0;
						}
					}
				}
			}
			else
			{
				if( ( ( iTime - itList->m_iLoginTime ) > ( itList->m_iLoginTimeout / 2 ) && itList->m_iResponseTime != 0 ) || itList->m_iLoginTimeout == 0 )
				{
					CSipMessage * pclsRequest = itList->CreateRegister( &pclsSipUserAgent->m_clsSipStack, NULL );
					if( pclsRequest )
					{
						if( pclsSipUserAgent->m_clsSipStack.SendSipMessage( pclsRequest ) )
						{
							itList->m_iSendTime = iTime;
							itList->m_iResponseTime = 0;
						}
					}
				}
				else if( itList->m_iNatTimeout > 0 )
				{
					if( ( iTime - itList->m_iSendTime ) >= itList->m_iNatTimeout )
					{
						pclsSipUserAgent->m_clsSipStack.Send( "\r\n", itList->m_strIp.c_str(), itList->m_iPort, itList->m_eTransport );
						itList->m_iSendTime = iTime;
					}
				}
			}
		}
		pclsSipUserAgent->m_clsRegisterMutex.release();

		sleep(1);
	}

	pclsSipUserAgent->m_bStopEvent = false;

	return 0;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 서버에 주기적으로 REGISTER 메시지를 전송하는 쓰레드를 시작한다.
 * @param pclsSipUserAgent CSipUserAgent 객체의 포인터
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartSipRegisterThread( CSipUserAgent * pclsSipUserAgent )
{
	return StartThread( "SipRegisterThread", SipRegisterThread, pclsSipUserAgent );
}

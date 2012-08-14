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
#include <time.h>

/**
 * @ingroup SipUserAgent
 * @brief SIP 서버에 주기적으로 REGISTER 메시지를 전송하는 쓰레드
 * @param lpParameter CSipUserAgent 객체의 포인터
 * @returns 0 을 리턴한다.
 */
#ifdef WIN32
DWORD WINAPI SipRegisterThread( LPVOID lpParameter )
#else
void * SipRegisterThread( void * lpParameter )
#endif
{
	CSipUserAgent * pclsSipUserAgent = (CSipUserAgent *)lpParameter;
	SIP_SERVER_INFO_LIST::iterator itList;
	time_t	iTime;

	while( gclsSipStack.m_bStopEvent == false )
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

					CSipMessage * pclsRequest = itList->CreateRegister( NULL );
					if( pclsRequest )
					{
						gclsSipStack.SendSipMessage( pclsRequest );
					}

					itList->m_iSendTime = iTime;
				}
			}
			else
			{
				if( ( iTime - itList->m_iLoginTime ) > ( itList->m_iLoginTimeout / 2 ) )
				{
					CSipMessage * pclsRequest = itList->CreateRegister( NULL );
					if( pclsRequest )
					{
						gclsSipStack.SendSipMessage( pclsRequest );
					}
				}
			}
		}
		pclsSipUserAgent->m_clsRegisterMutex.release();

		if( pclsSipUserAgent->m_pclsCallBack )
		{
			pclsSipUserAgent->m_pclsCallBack->EventTimer();
		}

		sleep(1);
	}

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

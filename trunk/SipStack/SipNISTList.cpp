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

#include "SipNISTList.h"
#include "SipStack.h"
#include "SipDeleteQueue.h"
#include "TimeUtility.h"

/**
 * @ingroup SipStack
 * @brief 생성자
 */
CSipNISTList::CSipNISTList()
{
}

/**
 * @ingroup SipStack
 * @brief 소멸자
 */
CSipNISTList::~CSipNISTList()
{
}

/**
 * @ingroup SipStack
 * @brief Non-Invite Server Transaction List 에 SIP 메시지를 추가한다.
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipNISTList::Insert( CSipMessage * pclsMessage )
{
	std::string strKey;
	NON_INVITE_TRANSACTION_MAP::iterator	itMap;
	bool bRes = false;

	if( pclsMessage == NULL ) return false;
	if( GetKey( pclsMessage, strKey ) == false ) return false;

	if( pclsMessage->IsRequest() )
	{
		m_clsMutex.acquire();
		itMap = m_clsMap.find( strKey );

		if( itMap == m_clsMap.end() )
		{
			CSipNonInviteTransaction * psttTransaction = new CSipNonInviteTransaction();
			if( psttTransaction )
			{
				psttTransaction->m_pclsRequest = pclsMessage;
				gettimeofday( &psttTransaction->m_sttStartTime, NULL );

				m_clsMap.insert( NON_INVITE_TRANSACTION_MAP::value_type( strKey, psttTransaction ) );
				bRes = true;
			}
		}
		else
		{
			if( itMap->second->m_pclsResponse )
			{
				m_pclsSipStack->Send( itMap->second->m_pclsResponse, false );
			}
		}
		m_clsMutex.release();
	}
	else
	{
		m_clsMutex.acquire();
		itMap = m_clsMap.find( strKey );
		if( itMap != m_clsMap.end() )
		{
			if( itMap->second->m_pclsResponse == NULL )
			{
				itMap->second->m_pclsResponse = pclsMessage;
				bRes = true;
			}
			else if( itMap->second->m_pclsResponse->m_iStatusCode != pclsMessage->m_iStatusCode )
			{
				gclsSipDeleteQueue.Insert( itMap->second->m_pclsResponse );
				itMap->second->m_pclsResponse = pclsMessage;
				bRes = true;
			}

			if( bRes )
			{
				if( pclsMessage->m_iStatusCode >= 200 )
				{
					gettimeofday( &itMap->second->m_sttStopTime, NULL );
				}

				pclsMessage->MakePacket();
			}
		}
		m_clsMutex.release();
	}

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief Non-Invite Server Transaction List 에서 재전송할 항목에서 삭제할 항목을 삭제한다.
 * @param psttTime 실행한 시간
 */
void CSipNISTList::Execute( struct timeval * psttTime )
{
	if( m_clsMap.size() == 0 ) return;

	NON_INVITE_TRANSACTION_MAP::iterator	itMap, itNext;
	OSIP_MESSAGE_LIST	clsResponseList;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( itMap->second->m_sttStopTime.tv_sec > 0 )
		{
			if( DiffTimeval( &itMap->second->m_sttStopTime, psttTime ) >= 32000 )
			{
				itNext = itMap;
				++itNext;

				delete itMap->second;
				m_clsMap.erase( itMap );

				if( itNext == m_clsMap.end() ) break;
				itMap = itNext;
				goto LOOP_START;
			}
		}
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Non-Invite Server Transaction List 의 모든 항목을 삭제한다.
 */
void CSipNISTList::DeleteAll( )
{
	NON_INVITE_TRANSACTION_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		delete itMap->second;
	}

	m_clsMap.clear();
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Non-Invite Server Transaction List 의 크기를 리턴한다.
 * @returns Non-Invite Server Transaction List 의 크기를 리턴한다.
 */
int CSipNISTList::GetSize( )
{
	return (int)m_clsMap.size();
}

/**
 * @ingroup SipStack
 * @brief NIST 자료구조에 저장된 SIP Call-ID 들을 문자열에 저장한다.
 * @param strBuf SIP Call-ID 들을 저장할 문자열
 */
void CSipNISTList::GetString( std::string & strBuf )
{
	NON_INVITE_TRANSACTION_MAP::iterator	itMap;

	strBuf.clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.append( itMap->first );
		strBuf.append( "\n" );
	}
	m_clsMutex.release();
}

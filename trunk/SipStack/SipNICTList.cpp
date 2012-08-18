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

#include "SipNICTList.h"
#include "SipStack.h"
#include "SipDeleteQueue.h"

/**
 * @ingroup SipStack
 * @brief 생성자
 */
CSipNICTList::CSipNICTList()
{
}

/**
 * @ingroup SipStack
 * @brief 소멸자
 */
CSipNICTList::~CSipNICTList()
{
}

/**
 * @ingroup SipStack
 * @brief Non-Invite Client Transaction List 에 SIP 메시지를 추가한다.
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipNICTList::Insert( CSipMessage * pclsMessage )
{
	std::string strKey;
	NON_INVITE_TRANSACTION_MAP::iterator	itMap;
	bool	bRes = false;

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

			if( bRes )
			{
				pclsMessage->MakePacket();
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
			if( itMap->second->m_pclsResponse )
			{
				if( itMap->second->m_pclsResponse->m_iStatusCode != pclsMessage->m_iStatusCode )
				{
					gclsSipDeleteQueue.Insert( itMap->second->m_pclsResponse );
					itMap->second->m_pclsResponse = pclsMessage;
					bRes = true;
				}
			}
			else
			{
				itMap->second->m_pclsResponse = pclsMessage;
				bRes = true;
			}

			if( bRes && pclsMessage->m_iStatusCode >= 200 )
			{
				gettimeofday( &itMap->second->m_sttStopTime, NULL );
			}
		}
		m_clsMutex.release();
	}

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief Non-Invite Client Transaction List 에서 재전송할 항목을 재전송하고 timeout 된 항목은 timeout 처리한다.
 * @param psttTime 실행한 시간
 */
void CSipNICTList::Execute( struct timeval * psttTime )
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
			if( DiffTimeval( &itMap->second->m_sttStopTime, psttTime ) >= 5000 )
			{
DELETE_TRANSACTION:
				itNext = itMap;
				++itNext;

				delete itMap->second;
				m_clsMap.erase( itMap );

				if( itNext == m_clsMap.end() ) break;
				itMap = itNext;
				goto LOOP_START;
			}
		}
		else
		{
			if( DiffTimeval( &itMap->second->m_sttStartTime, psttTime ) >= m_arrICTReSendTime[itMap->second->m_iReSendCount] )
			{
				++itMap->second->m_iReSendCount;
				if( itMap->second->m_iReSendCount == MAX_ICT_RESEND_COUNT )
				{
					if( itMap->second->m_pclsResponse == NULL )
					{
						CSipMessage * pclsResponse = itMap->second->m_pclsRequest->CreateResponse( SIP_REQUEST_TIME_OUT );
						if( pclsResponse )
						{
							clsResponseList.push_back( pclsResponse );
						}
					}

					goto DELETE_TRANSACTION;
				}
				else
				{
					m_pclsSipStack->Send( itMap->second->m_pclsRequest, false );
				}
			}
		}
	}
	m_clsMutex.release();

	for( OSIP_MESSAGE_LIST::iterator itList = clsResponseList.begin(); itList != clsResponseList.end(); ++itList )
	{
		m_pclsSipStack->RecvResponse( m_pclsSipStack->m_clsSetup.m_iUdpThreadCount, *itList );
		delete *itList;
	}

	clsResponseList.clear();
}

/**
 * @ingroup SipStack
 * @brief Non-Invite Client Transaction List 의 모든 항목을 삭제한다.
 */
void CSipNICTList::DeleteAll( )
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
 * @brief Non-Invite Client Transaction List 의 크기를 리턴한다.
 * @returns Non-Invite Client Transaction List 의 크기를 리턴한다.
 */
int CSipNICTList::GetSize( )
{
	return m_clsMap.size();
}

/**
 * @ingroup SipStack
 * @brief NICT 자료구조에 저장된 SIP Call-ID 들을 문자열에 저장한다.
 * @param strBuf SIP Call-ID 들을 저장할 문자열
 */
void CSipNICTList::GetString( std::string & strBuf )
{
	// QQQ
}

/**
 * @ingroup SipStack
 * @brief SIP 메시지에 해당하는 CANCEL 메시지를 검색해서 삭제한다.
 * @param pclsMessage		SIP 메시지
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipNICTList::DeleteCancel( CSipMessage * pclsMessage )
{
	if( m_clsMap.size() == 0 ) return false;

	std::string strKey;
	NON_INVITE_TRANSACTION_MAP::iterator	itMap;
	bool	bRes = false;

	if( pclsMessage == NULL ) return false;
	if( pclsMessage->IsRequest() )	return false;
	if( GetKey( pclsMessage, "CANCEL", strKey ) == false ) return false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		delete itMap->second;
		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

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

#include "SipISTList.h"
#include "SipStack.h"
#include "SipDeleteQueue.h"

/**
 * @ingroup SipStack
 * @brief 생성자
 */
CSipISTList::CSipISTList()
{
}

/**
 * @ingroup SipStack
 * @brief 소멸자
 */
CSipISTList::~CSipISTList()
{
}

/**
 * @ingroup SipStack
 * @brief Invite Server Transaction List 에 SIP 메시지를 추가한다.
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipISTList::Insert( CSipMessage * pclsMessage )
{
	std::string strKey;
	INVITE_TRANSACTION_MAP::iterator	itMap;
	bool bRes = false;

	if( pclsMessage == NULL ) return false;
	if( GetKey( pclsMessage, strKey ) == false ) return false;

	if( pclsMessage->IsRequest() )
	{
		m_clsMutex.acquire();
		itMap = m_clsMap.find( strKey );

		if( itMap == m_clsMap.end() )
		{
			if( pclsMessage->IsMethod( "ACK" ) )
			{
				for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
				{
					if( itMap->second->m_pclsRequest->IsEqualCallId( pclsMessage ) )
					{
						if( itMap->second->m_pclsAck == NULL )
						{
							if( itMap->second->m_pclsResponse && itMap->second->m_pclsResponse->m_iStatusCode == SIP_UNAUTHORIZED )
							{
								// INVITE 에 대한 응답 메시지가 401 인 ACK 메시지를 수신하면 Transaction 을 바로 삭제한다.
								delete itMap->second;
								m_clsMap.erase( itMap );
							}
							else
							{
								itMap->second->m_pclsAck = pclsMessage;
								gettimeofday( &itMap->second->m_sttStopTime, NULL );
								bRes = true;
							}

							break;
						}
					}
				}
			}
			else
			{
				CSipInviteTransaction * psttTransaction = new CSipInviteTransaction();
				if( psttTransaction )
				{
					psttTransaction->m_pclsRequest = pclsMessage;
					gettimeofday( &psttTransaction->m_sttStartTime, NULL );

					m_clsMap.insert( INVITE_TRANSACTION_MAP::value_type( strKey, psttTransaction ) );
					itMap = m_clsMap.find( strKey );
					bRes = true;

					CSipMessage * psttResponse = itMap->second->m_pclsRequest->CreateResponse( SIP_TRYING );
					if( psttResponse )
					{
						itMap->second->m_pclsResponse = psttResponse;
						m_pclsSipStack->Send( psttResponse );
					}
				}
			}
		}
		else
		{
			if( pclsMessage->IsMethod( "ACK" ) )
			{
				if( itMap->second->m_pclsResponse && itMap->second->m_pclsResponse->m_iStatusCode == SIP_UNAUTHORIZED )
				{
					// INVITE 에 대한 응답 메시지가 401 인 ACK 메시지를 수신하면 Transaction 을 바로 삭제한다.
					delete itMap->second;
					m_clsMap.erase( itMap );	
				}
				else if( itMap->second->m_pclsAck == NULL )
				{
					itMap->second->m_pclsAck = pclsMessage;
					gettimeofday( &itMap->second->m_sttStopTime, NULL );
					bRes = true;
				}
			}
			else if( itMap->second->m_pclsResponse )
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
				bRes = true;
				itMap->second->m_pclsResponse = pclsMessage;
				itMap->second->m_iStatusCode = pclsMessage->m_iStatusCode;
			}
			else if( itMap->second->m_iStatusCode != pclsMessage->m_iStatusCode )
			{
				bRes = true;

				gclsSipDeleteQueue.Insert( itMap->second->m_pclsResponse );
				itMap->second->m_pclsResponse = pclsMessage;
				itMap->second->m_iStatusCode = pclsMessage->m_iStatusCode;
			}

			if( bRes )
			{
				if( pclsMessage->m_iStatusCode >= 200 )
				{
					gettimeofday( &itMap->second->m_sttStartTime, NULL );
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
 * @brief Invite Server Transaction List 에서 삭제할 항목은 삭제하고 ACK 를 수신하지 못 한 항목은 응답 메시지를 재전송해 준다.
 * @param psttTime 실행한 시간
 */
void CSipISTList::Execute( struct timeval * psttTime )
{
	if( m_clsMap.size() == 0 ) return;

	INVITE_TRANSACTION_MAP::iterator	itMap, itNext;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( itMap->second->m_sttStopTime.tv_sec > 0 )
		{
			if( DiffTimeval( &itMap->second->m_sttStopTime, psttTime ) >= 5000 )
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
		else if( itMap->second->m_iStatusCode >= 200 && itMap->second->m_pclsAck == NULL )
		{
			if( DiffTimeval( &itMap->second->m_sttStartTime, psttTime ) >= m_arrICTReSendTime[itMap->second->m_iReSendCount] )
			{
				++itMap->second->m_iReSendCount;
				if( itMap->second->m_iReSendCount == MAX_ICT_RESEND_COUNT )
				{
					memcpy( &itMap->second->m_sttStopTime, psttTime, sizeof(itMap->second->m_sttStopTime) );
				}
				else
				{
					m_pclsSipStack->Send( itMap->second->m_pclsResponse, false );
				}
			}
		}
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Invite Server Transaction List 의 모든 항목을 삭제한다.
 */
void CSipISTList::DeleteAll( )
{
	INVITE_TRANSACTION_MAP::iterator	itMap;

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
 * @brief Invite Server Transaction List 의 크기를 리턴한다.
 * @returns Invite Server Transaction List 의 크기를 리턴한다.
 */
int CSipISTList::GetSize( )
{
	return (int)m_clsMap.size();
}

/**
 * @ingroup SipStack
 * @brief IST 자료구조에 저장된 SIP Call-ID 들을 문자열에 저장한다.
 * @param strBuf SIP Call-ID 들을 저장할 문자열
 */
void CSipISTList::GetString( std::string & strBuf )
{
	// QQQ
}

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

#include "SipICTList.h"
#include "SipStack.h"
#include "SipDeleteQueue.h"
#include "TimeUtility.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipStack
 * @brief 생성자
 */
CSipICTList::CSipICTList() : m_iTimerD(32000)
{
}

/**
 * @ingroup SipStack
 * @brief 소멸자
 */
CSipICTList::~CSipICTList()
{
	DeleteAll();
}

/**
 * @ingroup SipStack
 * @brief Invite Client Transaction List 에 SIP 메시지를 추가한다.
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipICTList::Insert( CSipMessage * pclsMessage )
{
	std::string strKey;
	INVITE_TRANSACTION_MAP::iterator	itMap;
	bool	bRes = false;

	if( pclsMessage == NULL ) return false;
	if( GetKey( pclsMessage, strKey ) == false ) return false;

	if( pclsMessage->IsRequest() )
	{
		pclsMessage->MakePacket();

		m_clsMutex.acquire();
		itMap = m_clsMap.find( strKey );
		if( itMap == m_clsMap.end() )
		{
			if( pclsMessage->IsMethod( SIP_METHOD_ACK ) )
			{
				INVITE_CALL_ID_MAP::iterator itCIM;
				std::string strCallId;

				pclsMessage->GetCallId( strCallId );

				itCIM = m_clsCallIdMap.find( strCallId );
				if( itCIM != m_clsCallIdMap.end() )
				{
					itMap = m_clsMap.find( itCIM->second );
					if( itMap != m_clsMap.end() )
					{
						if( itMap->second->m_pclsAck == NULL )
						{
							if( itMap->second->m_pclsResponse && itMap->second->m_pclsResponse->m_iStatusCode == SIP_UNAUTHORIZED )
							{
								// INVITE 에 대한 응답 메시지가 401 인 ACK 메시지를 수신하면 Transaction 을 바로 삭제한다.
								delete itMap->second;
								m_clsMap.erase( itMap );
								m_clsCallIdMap.erase( itCIM );
							}
							else
							{
								itMap->second->m_pclsAck = pclsMessage;
								gettimeofday( &itMap->second->m_sttStopTime, NULL );
							}
							
							bRes = true;
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
					bRes = true;

					INVITE_CALL_ID_MAP::iterator itCIM;
					std::string strCallId;

					pclsMessage->GetCallId( strCallId );

					itCIM = m_clsCallIdMap.find( strCallId );
					if( itCIM == m_clsCallIdMap.end() )
					{
						m_clsCallIdMap.insert( INVITE_CALL_ID_MAP::value_type( strCallId, strKey ) );
					}
				}
			}
		}
		else if( pclsMessage->IsMethod( SIP_METHOD_ACK ) )
		{
			itMap->second->m_pclsAck = pclsMessage;
			gettimeofday( &itMap->second->m_sttStopTime, NULL );
			bRes = true;
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
				if( itMap->second->m_iStatusCode != pclsMessage->m_iStatusCode )
				{
					gclsSipDeleteQueue.Insert( itMap->second->m_pclsResponse );
					itMap->second->m_pclsResponse = pclsMessage;
					itMap->second->m_iStatusCode = pclsMessage->m_iStatusCode;
					bRes = true;
				}
			}
			else
			{
				itMap->second->m_pclsResponse = pclsMessage;
				itMap->second->m_iStatusCode = pclsMessage->m_iStatusCode;

				// TCP,TLS connect error 이면 transaction 삭제를 위해서 stop time 을 설정한다.
				if( pclsMessage->m_iStatusCode == SIP_CONNECT_ERROR )
				{
					gettimeofday( &itMap->second->m_sttStopTime, NULL );
				}

				bRes = true;
			}

			// 180/183 응답 메시지를 수신한 후, 200 OK 를 수신하지 못 한 경우 ICT 에서 삭제하기 위한 기능
			// 200 OK 를 수신하고 ACK 를 전송하지 않은 경우 ICT 에서 삭제하기 위한 기능
			if( itMap->second->m_sttRingTime.tv_sec == 0 )
			{
				gettimeofday( &itMap->second->m_sttRingTime, NULL );
			}

			if( itMap->second->m_pclsAck )
			{
				m_pclsSipStack->Send( itMap->second->m_pclsAck, false );
			}
		}
		m_clsMutex.release();
	}

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief Invite Client Transaction List 에서 재전송할 항목은 재전송하고 삭제할 항목은 삭제하고 Timeout 된 항목은 Timeout 처리한다.
 * @param psttTime 실행한 시간
 */
void CSipICTList::Execute( struct timeval * psttTime )
{
	INVITE_TRANSACTION_MAP::iterator	itMap, itNext;
	INVITE_CALL_ID_MAP::iterator	itCIM;
	SIP_MESSAGE_LIST	clsResponseList;
	std::string strCallId;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( itMap->second->m_sttStopTime.tv_sec > 0 )
		{
			if( DiffTimeval( &itMap->second->m_sttStopTime, psttTime ) >= m_iTimerD )
			{
DELETE_TRANSACTION:
				itNext = itMap;
				++itNext;

				itMap->second->m_pclsRequest->GetCallId( strCallId );

				delete itMap->second;
				m_clsMap.erase( itMap );

				itCIM = m_clsCallIdMap.find( strCallId );
				if( itCIM != m_clsCallIdMap.end() )
				{
					m_clsCallIdMap.erase( itCIM );
				}

				if( itNext == m_clsMap.end() ) break;
				itMap = itNext;
				goto LOOP_START;
			}
		}
		else if( itMap->second->m_iStatusCode == 0 )
		{
			if( DiffTimeval( &itMap->second->m_sttStartTime, psttTime ) >= m_arrICTReSendTime[itMap->second->m_iReSendCount] )
			{
				++itMap->second->m_iReSendCount;
				if( itMap->second->m_iReSendCount == MAX_ICT_RESEND_COUNT )
				{
					if( itMap->second->m_pclsResponse == NULL )
					{
						CSipMessage * psttResponse = itMap->second->m_pclsRequest->CreateResponse( SIP_REQUEST_TIME_OUT );
						if( psttResponse )
						{
							clsResponseList.push_back( psttResponse );
						}
					}

					goto DELETE_TRANSACTION;
				}
				else if( itMap->second->m_pclsRequest->m_eTransport == E_SIP_UDP )
				{
					m_pclsSipStack->Send( itMap->second->m_pclsRequest, false );
				}
			}
		}
		else if( itMap->second->m_sttRingTime.tv_sec > 0 )
		{
			if( DiffTimeval( &itMap->second->m_sttRingTime, psttTime ) >= SIP_RING_TIMEOUT )
			{
				CSipMessage * psttResponse = itMap->second->m_pclsRequest->CreateResponse( SIP_REQUEST_TIME_OUT );
				if( psttResponse )
				{
					clsResponseList.push_back( psttResponse );
				}

				goto DELETE_TRANSACTION;
			}
		}
	}
	m_clsMutex.release();

	for( SIP_MESSAGE_LIST::iterator itList = clsResponseList.begin(); itList != clsResponseList.end(); ++itList )
	{
		m_pclsSipStack->RecvResponse( m_pclsSipStack->m_clsSetup.m_iUdpThreadCount, *itList );
		delete *itList;
	}

	clsResponseList.clear();
}

/**
 * @ingroup SipStack
 * @brief Invite Client Transaction List 의 모든 항목을 삭제한다.
 */
void CSipICTList::DeleteAll( )
{
	INVITE_TRANSACTION_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		delete itMap->second;
	}

	m_clsMap.clear();
	m_clsCallIdMap.clear();
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Invite Client Transaction List 의 크기를 리턴한다.
 * @returns Invite Client Transaction List 의 크기를 리턴한다.
 */
int CSipICTList::GetSize( )
{
	int iSize;

	m_clsMutex.acquire();
	iSize = (int)m_clsMap.size();
	m_clsMutex.release();

	return iSize;
}

/**
 * @ingroup SipStack
 * @brief ICT 에 저장된 KEY 들을 문자열에 저장한다.
 * @param strBuf KEY 들을 저장할 변수
 */
void CSipICTList::GetString( CMonitorString & strBuf )
{
	INVITE_TRANSACTION_MAP::iterator	itMap;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.AddRow( itMap->first );
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Timer D 값을 수정한다.
 * @param iMiliSecond Timer D 값 ( milisecond 단위 )
 */
void CSipICTList::SetTimerD( int iMiliSecond )
{
	m_iTimerD = iMiliSecond;
}

/**
 * @ingroup SipStack
 * @brief transcation map 을 가져온다.
 * @param clsMap [out] transcation map 을 저장할 변수
 */
void CSipICTList::GetTransactionMap( INVITE_TRANSACTION_MAP & clsMap )
{
	m_clsMutex.acquire();
	clsMap = m_clsMap;
	m_clsMutex.release();
}

/**
 * @ingroup SipStack3
 * @brief SIP Call-ID 자료구조에 저장된 개수를 리턴한다.
 * @returns SIP Call-ID 자료구조에 저장된 개수를 리턴한다.
 */
int CSipICTList::GetCallIdCount( )
{
	int iCount = 0;

	m_clsMutex.acquire();
	iCount = (int)m_clsCallIdMap.size();
	m_clsMutex.release();

	return iCount;
}

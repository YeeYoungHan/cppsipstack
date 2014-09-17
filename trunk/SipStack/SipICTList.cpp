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
 * @brief ������
 */
CSipICTList::CSipICTList() : m_iTimerD(32000)
{
}

/**
 * @ingroup SipStack
 * @brief �Ҹ���
 */
CSipICTList::~CSipICTList()
{
	DeleteAll();
}

/**
 * @ingroup SipStack
 * @brief Invite Client Transaction List �� SIP �޽����� �߰��Ѵ�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
				for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
				{
					if( itMap->second->m_pclsRequest->IsEqualCallId( pclsMessage ) )
					{
						if( itMap->second->m_pclsAck == NULL )
						{
							if( itMap->second->m_pclsResponse && itMap->second->m_pclsResponse->m_iStatusCode == SIP_UNAUTHORIZED )
							{
								// INVITE �� ���� ���� �޽����� 401 �� ACK �޽����� �����ϸ� Transaction �� �ٷ� �����Ѵ�.
								delete itMap->second;
								m_clsMap.erase( itMap );
							}
							else
							{
								itMap->second->m_pclsAck = pclsMessage;
								gettimeofday( &itMap->second->m_sttStopTime, NULL );
							}
							
							bRes = true;
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
					bRes = true;
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

				// TCP,TLS connect error �̸� transaction ������ ���ؼ� stop time �� �����Ѵ�.
				if( pclsMessage->m_iStatusCode == SIP_CONNECT_ERROR )
				{
					gettimeofday( &itMap->second->m_sttStopTime, NULL );
				}

				bRes = true;
			}

			// 180/183 ���� �޽����� ������ ��, 200 OK �� �������� �� �� ��� ICT ���� �����ϱ� ���� ���
			// 200 OK �� �����ϰ� ACK �� �������� ���� ��� ICT ���� �����ϱ� ���� ���
			gettimeofday( &itMap->second->m_sttRingTime, NULL );

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
 * @brief Invite Client Transaction List ���� �������� �׸��� �������ϰ� ������ �׸��� �����ϰ� Timeout �� �׸��� Timeout ó���Ѵ�.
 * @param psttTime ������ �ð�
 */
void CSipICTList::Execute( struct timeval * psttTime )
{
	INVITE_TRANSACTION_MAP::iterator	itMap, itNext;
	SIP_MESSAGE_LIST	clsResponseList;

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

				delete itMap->second;
				m_clsMap.erase( itMap );

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
 * @brief Invite Client Transaction List �� ��� �׸��� �����Ѵ�.
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
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Invite Client Transaction List �� ũ�⸦ �����Ѵ�.
 * @returns Invite Client Transaction List �� ũ�⸦ �����Ѵ�.
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
 * @brief ICT �� ����� SIP Call-ID ���� ���ڿ��� �����Ѵ�.
 * @param strBuf SIP Call-ID ���� ������ ����
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
 * @brief Timer D ���� �����Ѵ�.
 * @param iMiliSecond Timer D �� ( milisecond ���� )
 */
void CSipICTList::SetTimerD( int iMiliSecond )
{
	m_iTimerD = iMiliSecond;
}

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
#include "TimeUtility.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipStack
 * @brief ������
 */
CSipISTList::CSipISTList()
{
}

/**
 * @ingroup SipStack
 * @brief �Ҹ���
 */
CSipISTList::~CSipISTList()
{
}

/**
 * @ingroup SipStack
 * @brief Invite Server Transaction List �� SIP �޽����� �߰��Ѵ�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
			if( pclsMessage->IsMethod( SIP_METHOD_ACK ) )
			{
				for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
				{
					if( itMap->second->m_pclsRequest->IsEqualCallIdSeq( pclsMessage ) )
					{
						// 2014��4��3�� ���� ���������� SIP Call-ID �� ���Ͽ����� �Ʒ��� ACK �� NULL ���� �����ϴ� ��� ������ ���������� �����Ͽ���.
						// ���� �Ʒ��� ����� �����ٸ� 2���� ReINVITE �� ���� ACK �� �����Ͽ��� ���� ù��° ReINVITE ���� ACK �� 2�� �����Ͽ���
						// �ι�° ReINVITE �� ���� ACK �� ���ŵ��� ���� ������ �������� �� �ֽ��ϴ�.
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
			if( pclsMessage->IsMethod( SIP_METHOD_ACK ) )
			{
				if( itMap->second->m_pclsResponse && itMap->second->m_pclsResponse->m_iStatusCode == SIP_UNAUTHORIZED )
				{
					// INVITE �� ���� ���� �޽����� 401 �� ACK �޽����� �����ϸ� Transaction �� �ٷ� �����Ѵ�.
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
		pclsMessage->MakePacket();

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
			else if( pclsMessage->m_iStatusCode > itMap->second->m_iStatusCode )
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
			}
		}
		m_clsMutex.release();
	}

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief Invite Server Transaction List ���� ������ �׸��� �����ϰ� ACK �� �������� �� �� �׸��� ���� �޽����� �������� �ش�.
 * @param psttTime ������ �ð�
 */
void CSipISTList::Execute( struct timeval * psttTime )
{
	INVITE_TRANSACTION_MAP::iterator	itMap, itNext;
	SIP_MESSAGE_LIST	clsResponseList;

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

					// INVITE ���� �޽����� 200 OK �� �����ϰ� �̿� ���� ACK �� �������� �� �� ��� timeout ó���Ѵ�.
					if( itMap->second->m_iStatusCode == 200 )
					{
						clsResponseList.push_back( itMap->second->m_pclsResponse );
					}
				}
				else if( itMap->second->m_pclsResponse->m_eTransport == E_SIP_UDP )
				{
					m_pclsSipStack->Send( itMap->second->m_pclsResponse, false );
				}
			}
		}
	}
	m_clsMutex.release();

	for( SIP_MESSAGE_LIST::iterator itList = clsResponseList.begin(); itList != clsResponseList.end(); ++itList )
	{
		m_pclsSipStack->SendTimeout( m_pclsSipStack->m_clsSetup.m_iUdpThreadCount, *itList );
	}

	clsResponseList.clear();
}

/**
 * @ingroup SipStack
 * @brief Invite Server Transaction List �� ��� �׸��� �����Ѵ�.
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
 * @brief Invite Server Transaction List �� ũ�⸦ �����Ѵ�.
 * @returns Invite Server Transaction List �� ũ�⸦ �����Ѵ�.
 */
int CSipISTList::GetSize( )
{
	int iSize;

	m_clsMutex.acquire();
	iSize = (int)m_clsMap.size();
	m_clsMutex.release();

	return iSize;
}

/**
 * @ingroup SipStack
 * @brief IST �ڷᱸ���� ����� SIP Call-ID ���� ���ڿ��� �����Ѵ�.
 * @param strBuf SIP Call-ID ���� ������ ���ڿ�
 */
void CSipISTList::GetString( CMonitorString & strBuf )
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

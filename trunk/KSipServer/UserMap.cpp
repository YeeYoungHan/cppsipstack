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

#include "SipParserDefine.h"
#include "UserMap.h"
#include "Log.h"
#include "SipServerSetup.h"
#include "SipServer.h"
#include "TimeString.h"
#include "MemoryDebug.h"

CUserMap gclsUserMap;

CUserInfo::CUserInfo() : m_iPort(0), m_iLoginTime(0), m_iLoginTimeout(3600), m_iOptionsSeq(0), m_iSendOptionsTime(0)
{
}

void CUserInfo::GetCallRoute( CSipCallRoute & clsRoute )
{
	clsRoute.m_strDestIp = m_strIp;
	clsRoute.m_iDestPort = m_iPort;
	clsRoute.m_eTransport = m_eTransport;
}

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

/**
 * @ingroup KSipServer
 * @brief �α��ε� Ŭ���̾�Ʈ ������ �����Ѵ�.
 * @param pclsMessage SIP REGISTER �޽���
 * @param pclsContact	SIP REGISTER ���� �޽����� ���Ե� Contact Url
 * @param pclsXmlUser	XML �� ����� ����� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CUserMap::Insert( CSipMessage * pclsMessage, CSipFrom * pclsContact, CXmlUser * pclsXmlUser )
{
	CUserInfo			clsInfo;
	std::string		strUserId;
	USER_MAP::iterator	itMap;

	strUserId = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
	if( strUserId.empty() ) return false;

	if( pclsMessage->GetTopViaIpPort( clsInfo.m_strIp, clsInfo.m_iPort ) == false ) return false;
	clsInfo.m_iLoginTimeout = pclsMessage->GetExpires();

	if( clsInfo.m_iLoginTimeout == 0 )
	{
		return false;
	}

	clsInfo.m_eTransport = pclsMessage->m_eTransport;
	time( &clsInfo.m_iLoginTime );

	clsInfo.m_strGroupId = pclsXmlUser->m_strGroupId;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strUserId );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( USER_MAP::value_type( strUserId, clsInfo ) );
		CLog::Print( LOG_DEBUG, "user(%s) is inserted (%s:%d:%d) group(%s)"
			, strUserId.c_str(), clsInfo.m_strIp.c_str(), clsInfo.m_iPort, clsInfo.m_eTransport, clsInfo.m_strGroupId.c_str() );
	}
	else
	{
		itMap->second = clsInfo;
		CLog::Print( LOG_DEBUG, "user(%s) is updated (%s:%d:%d) group(%s)"
			, strUserId.c_str(), clsInfo.m_strIp.c_str(), clsInfo.m_iPort, clsInfo.m_eTransport, clsInfo.m_strGroupId.c_str() );
	}
	m_clsMutex.release();

	if( pclsContact )
	{
		pclsContact->m_clsUri.m_strProtocol = SIP_PROTOCOL;
		pclsContact->m_clsUri.m_strUser = strUserId;
		pclsContact->m_clsUri.m_strHost = clsInfo.m_strIp;
		pclsContact->m_clsUri.m_iPort = clsInfo.m_iPort;
		pclsContact->m_clsUri.InsertTransport( clsInfo.m_eTransport );
	}

	return true;
}

/**
 * @ingroup KSipServer
 * @brief ����� ID �� �ش��ϴ� ������ �˻��Ѵ�.
 * @param pszUserId ����� ID
 * @param clsInfo		����� ������ ������ ����
 * @returns ����� ID �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserMap::Select( const char * pszUserId, CUserInfo & clsInfo )
{
	bool bRes = false;
	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		clsInfo = itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief ����� ���̵� �����ϴ��� �˻��Ѵ�.
 * @param pszUserId ����� ���̵�
 * @returns ����� ���̵� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserMap::Select( const char * pszUserId )
{
	bool bRes = false;
	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief �׷� ���̵� ���Ե� ����� ���̵� ����Ʈ�� �����Ѵ�.
 * @param pszGroupId	�׷� ���̵�
 * @param clsList			����� ���̵� ����Ʈ ���� ��ü
 * @returns �׷� ���̵� ���Ե� ����� ����Ʈ�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserMap::SelectGroup( const char * pszGroupId, USER_ID_LIST & clsList )
{
	USER_MAP::iterator	itMap;

	clsList.clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( !strcmp( pszGroupId, itMap->second.m_strGroupId.c_str() ) )
		{
			clsList.push_back( itMap->first );
		}
	}
	m_clsMutex.release();

	if( clsList.empty() == false ) return true;

	return false;
}

/**
 * @ingroup KSipServer
 * @brief ����� ���̵� �ڷᱸ������ �����Ѵ�.
 * @param pszUserId ����� ���̵�
 * @returns ����� ���̵� �ڷᱸ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserMap::Delete( const char * pszUserId )
{
	bool bRes = false;
	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		m_clsMap.erase( itMap );
		CLog::Print( LOG_DEBUG, "user(%s) is deleted", pszUserId );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CUserMap::SetIpPort( const char * pszUserId, const char * pszIp, int iPort )
{
	bool bRes = false;
	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.m_strIp = pszIp;
		itMap->second.m_iPort = iPort;
		CLog::Print( LOG_DEBUG, "user(%s) ip(%s) port(%d)", pszUserId, pszIp, iPort );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief ����� ����ڸ� �ڷᱸ������ �����Ѵ�.
 * @param iTimeout ����� �ð� ���Ŀ� ��� �ð� (�ʴ���)
 */
void CUserMap::DeleteTimeout( int iTimeout )
{
	USER_MAP::iterator	itMap, itNext;
	time_t iTime;

	time( &iTime );

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( iTime > ( itMap->second.m_iLoginTime + itMap->second.m_iLoginTimeout + iTimeout ) )
		{
			itNext = itMap;
			++itNext;

			CLog::Print( LOG_DEBUG, "user(%s) is deleted - timeout", itMap->first.c_str() );

			m_clsMap.erase( itMap );
			if( itNext == m_clsMap.end() ) break;
			itMap = itNext;
			goto LOOP_START;
		}
	}
	m_clsMutex.release();
}

/**
 * @ingroup KSipServer
 * @brief �α��ε� ��� ����ڿ��� OPTIONS �޽����� �����Ѵ�.
 */
void CUserMap::SendOptions(  )
{
	USER_MAP::iterator	itMap;
	USER_ID_LIST	clsList;
	USER_ID_LIST::iterator	itList;
	time_t	iTime;

	if( gclsSetup.m_iSendOptionsPeriod <= 0 ) return;

	time( &iTime );

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( itMap->second.m_iSendOptionsTime == 0 )
		{
			if( ( iTime - itMap->second.m_iLoginTime ) < gclsSetup.m_iSendOptionsPeriod )
			{
				continue;
			}
		}
		else
		{
			if( ( iTime - itMap->second.m_iSendOptionsTime ) < gclsSetup.m_iSendOptionsPeriod )
			{
				continue;
			}
		}

		itMap->second.m_iSendOptionsTime = iTime;

		++itMap->second.m_iOptionsSeq;
		if( itMap->second.m_iOptionsSeq > 10000000 ) itMap->second.m_iOptionsSeq = 1;

		clsList.push_back( itMap->first );
	}
	m_clsMutex.release();

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		CUserInfo clsUserInfo;

		if( Select( itList->c_str(), clsUserInfo ) == false )
		{
			continue;
		}

		CSipMessage * pclsMessage = new CSipMessage();
		if( pclsMessage == NULL ) break;

		pclsMessage->m_strSipMethod = SIP_METHOD_OPTIONS;
		pclsMessage->m_clsReqUri.Set( SIP_PROTOCOL, itList->c_str(), clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort );
		
		pclsMessage->m_clsFrom.m_clsUri.Set( SIP_PROTOCOL, "ksipserver", gclsSetup.m_strLocalIp.c_str() );
		pclsMessage->m_clsFrom.InsertTag();

		pclsMessage->m_clsTo.m_clsUri.Set( SIP_PROTOCOL, itList->c_str(), clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort );

		pclsMessage->m_clsCallId.Make( gclsSetup.m_strLocalIp.c_str() );

		pclsMessage->m_clsCSeq.Set( clsUserInfo.m_iOptionsSeq, SIP_METHOD_OPTIONS );
		pclsMessage->AddRoute( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort );

		gclsUserAgent.m_clsSipStack.SendSipMessage( pclsMessage );
	}
}

/**
 * @ingroup KSipServer
 * @brief �ڷᱸ�� ����͸��� ���ڿ��� �����Ѵ�. 
 * @param strBuf �ڷᱸ�� ����͸��� ���ڿ� ����
 */
void CUserMap::GetString( CMonitorString & strBuf )
{
	USER_MAP::iterator	itMap;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.AddCol( itMap->first );
		strBuf.AddCol( itMap->second.m_strIp, itMap->second.m_iPort );
		strBuf.AddCol( itMap->second.m_iLoginTime );
		strBuf.AddRow( itMap->second.m_iLoginTimeout );
	}
	m_clsMutex.release();
}

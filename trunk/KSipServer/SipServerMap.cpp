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

#include "KSipServer.h"
#include "SipServer.h"
#include "SipServerMap.h"
#include "SipServerSetup.h"
#include "Directory.h"
#include "Log.h"
#include "MemoryDebug.h"

CSipServerMap gclsSipServerMap;

CSipServerMap::CSipServerMap()
{
}

CSipServerMap::~CSipServerMap()
{
}

/**
 * @ingroup KSipServer
 * @brief XML 파일 또는 MySQL 에서 IP-PBX 정보를 읽는다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServerMap::Load( )
{
	bool	bRes = false;

	ReSetFlag( );

	if( gclsSetup.m_strSipServerXmlFolder.length() > 0 )
	{
		CLog::Print( LOG_DEBUG, "SipServerMap Load" );
		bRes = ReadDir( gclsSetup.m_strSipServerXmlFolder.c_str() );
	}

	SIP_SERVER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( itMap->second.m_iFlag == FLAG_NULL )
		{
			itMap->second.m_iFlag = FLAG_DELETE;
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief 폴더에서 IP-PBX 정보를 저장한 XML 파일을 읽어서 자료구조에 저장한다.
 * @param pszDirName 폴더 full path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServerMap::ReadDir( const char * pszDirName )
{
	FILE_LIST	clsFileList;
	FILE_LIST::iterator	itFL;
	SIP_SERVER_MAP::iterator	itMap;
	std::string		strFileName;

	if( CDirectory::FileList( pszDirName, clsFileList ) == false ) return false;

	for( itFL = clsFileList.begin(); itFL != clsFileList.end(); ++itFL )
	{
		CXmlSipServer	clsXml;

		strFileName = pszDirName;
		CDirectory::AppendName( strFileName, itFL->c_str() );

		if( clsXml.Parse( strFileName.c_str() ) )
		{
			clsXml.m_strName = itFL->c_str();
			Insert( clsXml );
		}
	}

	return true;
}

/**
 * @ingroup KSipServer
 * @brief IP-PBX 정보 저장 자료구조의 정보를 SipUserAgent 라이브러리에 전달한다.
 * @returns true 를 리턴한다.
 */
bool CSipServerMap::SetSipUserAgentRegisterInfo( )
{
	SIP_SERVER_MAP::iterator	itMap, itNext;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( itMap->second.m_iFlag == FLAG_INSERT )
		{
			gclsUserAgent.InsertRegisterInfo( itMap->second );
		}
		else if( itMap->second.m_iFlag == FLAG_UPDATE )
		{
			gclsUserAgent.UpdateRegisterInfo( itMap->second );
		}
		else if( itMap->second.m_iFlag == FLAG_DELETE )
		{
			gclsUserAgent.DeleteRegisterInfo( itMap->second );

			itNext = itMap;
			++itNext;

			m_clsMap.erase( itMap );
			if( itNext == m_clsMap.end() ) break;
			
			itMap = itNext;
			goto LOOP_START;
		}
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup KSipServer
 * @brief IP-PBX 정보 저장 자료구조에 존재하는지 검사한다.
 * @param pszIp			IP-PBX 의 IP 주소
 * @param pszUserId IP-PBX 에 로그인한 사용자 아이디
 * @returns IP-PBX 정보 저장 자료구조에 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerMap::Select( const char * pszIp, const char * pszUserId )
{
	std::string	strKey;
	bool	bRes = false;
	SIP_SERVER_MAP::iterator	itMap;

	strKey = pszIp;
	strKey.append( "_" );
	strKey.append( pszUserId );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief IP-PBX 정보 저장 자료구조를 검색하여서 수신자 전화번호에 대한 routing 정보를 가지고 있는 IP-PBX 정보를 저장한다.
 * @param pszTo						수신자 전화번호
 * @param clsXmlSipServer IP-PBX 정보 저장 객체
 * @param strTo						INVITE 메시지를 전송할 때 사용할 수신자 전화번호
 * @returns 검색에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerMap::SelectRoutePrefix( const char * pszTo, CXmlSipServer & clsXmlSipServer, std::string & strTo )
{
	SIP_SERVER_MAP::iterator		itMap;
	ROUTE_PREFIX_LIST::iterator	itList;
	bool bRes = false;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( itMap->second.m_clsRoutePrefixList.size() > 0 )
		{
			for( itList = itMap->second.m_clsRoutePrefixList.begin(); itList != itMap->second.m_clsRoutePrefixList.end(); ++itList )
			{
				if( !strncmp( itList->m_strPrefix.c_str(), pszTo, itList->m_strPrefix.length() ) )
				{
					clsXmlSipServer = itMap->second;

					if( itList->m_bDeletePrefix )
					{
						strTo = pszTo + itList->m_strPrefix.length();
					}
					else
					{
						strTo = pszTo;
					}

					bRes = true;
					break;
				}
			}
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief IP-PBX 에서 수신한 전화 요청과 매핑되는 KSipServer 아이디를 검색한다.
 * @parma pszIp	IP-PBX IP 주소
 * @param pszTo IP-PBX 에서 전송한 SIP 메시지의 TO 아이디
 * @param strTo KSipSerfver 아이디
 * @returns 검색에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerMap::SelectIncomingRoute( const char * pszIp, const char * pszTo, std::string & strTo )
{
	SIP_SERVER_MAP::iterator		itMap;
	INCOMING_ROUTE_LIST::iterator	itList;
	bool bRes = false;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( pszIp )
		{
			if( strcmp( itMap->second.m_strIp.c_str(), pszIp ) ) continue;
		}

		if( itMap->second.m_clsIncomingRouteList.size() > 0 )
		{
			for( itList = itMap->second.m_clsIncomingRouteList.begin(); itList != itMap->second.m_clsIncomingRouteList.end(); ++itList )
			{
				if( !strcmp( itList->m_strToId.c_str(), pszTo ) )
				{
					strTo = itList->m_strToId;
					bRes = true;
					break;
				}
			}
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief IP-PBX 정보 저장 자료구조의 key 문자열을 생성한다.
 * @param clsXmlSipServer IP-PBX 정보 저장 객체
 * @param strKey					key 문자열을 저장할 변수
 */
void CSipServerMap::GetKey( CXmlSipServer & clsXmlSipServer, std::string & strKey )
{
	strKey = clsXmlSipServer.m_strIp;
	strKey.append( "_" );
	strKey.append( clsXmlSipServer.m_strUserId );
}

/**
 * @brief 모든 SIP 서버 정보 상태값을 초기화시킨다.
 */
void CSipServerMap::ReSetFlag( )
{
	SIP_SERVER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		itMap->second.m_iFlag = FLAG_NULL;
	}
	m_clsMutex.release();
}

/**
 * @ingroup KSipServer
 * @brief IP-PBX 정보 저장 자료구조에 존재하지 않으면 추가하고
 *				IP-PBX 정보 저장 자료구조에 존재하면 정보 수정 여부를 검사하여서 수정된 경우 IP-PBX 정보를 수정한다.
 * @param clsXmlSipServer IP-PBX 정보 저장 객체
 * @returns true 를 리턴한다.
 */
bool CSipServerMap::Insert( CXmlSipServer & clsXmlSipServer )
{
	SIP_SERVER_MAP::iterator	itMap;
	std::string	strKey;

	GetKey( clsXmlSipServer, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		clsXmlSipServer.m_iFlag = FLAG_INSERT;
		m_clsMap.insert( SIP_SERVER_MAP::value_type( strKey, clsXmlSipServer ) );
	}
	else
	{
		itMap->second = clsXmlSipServer;

		if( strcmp( itMap->second.m_strDomain.c_str(), clsXmlSipServer.m_strDomain.c_str() ) ||
				strcmp( itMap->second.m_strPassWord.c_str(), clsXmlSipServer.m_strPassWord.c_str() ) ||
				itMap->second.m_iPort != clsXmlSipServer.m_iPort )
		{
			clsXmlSipServer.m_iFlag = FLAG_UPDATE;	
		}
		else
		{
			itMap->second.m_iFlag = FLAG_NO_CHANGE;
		}
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup KSipServer
 * @brief 자료구조 모니터링을 위한 문자열을 가져온다.
 * @param strBuf 자료구조 모니터링을 위한 문자열 저장 변수
 */
void CSipServerMap::GetString( CMonitorString & strBuf )
{
	SIP_SERVER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.AddCol( itMap->first );
		strBuf.AddCol( itMap->second.m_strName );
		strBuf.AddCol( itMap->second.m_iFlag );
		strBuf.AddCol( itMap->second.m_strIp, itMap->second.m_iPort );
		strBuf.AddCol( itMap->second.m_strDomain );
		strBuf.AddCol( itMap->second.m_strUserId );
		strBuf.AddCol( itMap->second.m_iLoginTimeout );
		strBuf.AddRow( itMap->second.m_bLogin ? "login" : "logout" );
	}
	m_clsMutex.release();
}

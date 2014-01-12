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
#include "DbMySQL.h"
#include "Log.h"
#include "MemoryDebug.h"

CSipServerMap gclsSipServerMap;

CSipServerMap::CSipServerMap()
{
}

CSipServerMap::~CSipServerMap()
{
}

#ifdef USE_MYSQL
/**
 * @ingroup KSipServer
 * @brief SipServer ���̺��� ���� �ϳ��� row �� ó���Ѵ�.
 * @param pclsData 
 * @param sttRow		�ϳ��� Row ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool SipServerFetchRow( void * pclsData, MYSQL_ROW & sttRow )
{
	CXmlSipServer	clsXml;

	if( sttRow[0] ) clsXml.m_strName = sttRow[0];
	if( sttRow[1] ) clsXml.m_strIp = sttRow[1];
	if( sttRow[2] ) clsXml.m_iPort = atoi( sttRow[2] );
	if( sttRow[3] ) clsXml.m_strDomain = sttRow[3];
	if( sttRow[4] ) clsXml.m_strUserId = sttRow[4];
	if( sttRow[5] ) clsXml.m_strPassWord = sttRow[5];
	if( sttRow[6] ) clsXml.m_iLoginTimeout = atoi( sttRow[6] );

	gclsSipServerMap.Insert( clsXml );

	return true;
}

/**
 * @ingroup KSipServer
 * @brief RoutePrefix ���̺��� ���� �ϳ��� row �� ó���Ѵ�.
 * @param pclsData	
 * @param sttRow		�ϳ��� Row ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool RoutePrefixFetchRow( void * pclsData, MYSQL_ROW & sttRow )
{
	std::string	strName;
	CRoutePrefix clsRoutePrefix;

	if( sttRow[0] ) strName = sttRow[0];
	if( sttRow[1] ) clsRoutePrefix.m_strPrefix = sttRow[1];
	if( sttRow[2] && sttRow[2][0] == 'Y' ) clsRoutePrefix.m_bDeletePrefix = true;

	gclsSipServerMap.InsertRoutePrefix( strName.c_str(), clsRoutePrefix );

	return true;
}
#endif

/**
 * @ingroup KSipServer
 * @brief XML ���� �Ǵ� MySQL ���� IP-PBX ������ �д´�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServerMap::Load( )
{
	bool	bRes = false;

	ReSetFlag( );

	if( gclsSetup.m_eType == E_DT_XML )
	{
		if( gclsSetup.m_strSipServerXmlFolder.length() > 0 )
		{
			CLog::Print( LOG_DEBUG, "SipServerMap Load" );
			bRes = ReadDir( gclsSetup.m_strSipServerXmlFolder.c_str() );
		}
	}
#ifdef USE_MYSQL
	else if( gclsSetup.m_eType == E_DT_MYSQL )
	{
		char szSQL[255];

		CLog::Print( LOG_DEBUG, "SipServerMap Load" );

		snprintf( szSQL, sizeof(szSQL), "SELECT Id, Ip, Port, Domain, UserId, PassWord, LoginTimeout FROM SipServer" );
		if( gclsReadDB.Query( szSQL, NULL, SipServerFetchRow ) )
		{
			bRes = true;

			snprintf( szSQL, sizeof(szSQL), "SELECT Id, Prefix, DeletePrefix FROM RoutePrefix" );
			if( gclsReadDB.Query( szSQL, NULL, RoutePrefixFetchRow ) )
			{
				
			}
		}
	}
#endif

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
 * @brief �������� IP-PBX ������ ������ XML ������ �о �ڷᱸ���� �����Ѵ�.
 * @param pszDirName ���� full path
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief IP-PBX ���� ���� �ڷᱸ���� ������ SipUserAgent ���̺귯���� �����Ѵ�.
 * @returns true �� �����Ѵ�.
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
 * @brief IP-PBX ���� ���� �ڷᱸ���� �����ϴ��� �˻��Ѵ�.
 * @param pszIp			IP-PBX �� IP �ּ�
 * @param pszUserId IP-PBX �� �α����� ����� ���̵�
 * @returns IP-PBX ���� ���� �ڷᱸ���� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief IP-PBX ���� ���� �ڷᱸ���� �˻��Ͽ��� ������ ��ȭ��ȣ�� ���� routing ������ ������ �ִ� IP-PBX ������ �����Ѵ�.
 * @param pszTo						������ ��ȭ��ȣ
 * @param clsXmlSipServer IP-PBX ���� ���� ��ü
 * @param strTo						INVITE �޽����� ������ �� ����� ������ ��ȭ��ȣ
 * @returns true �� �����Ѵ�.
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
 * @brief IP-PBX ���� ���� �ڷᱸ���� key ���ڿ��� �����Ѵ�.
 * @param clsXmlSipServer IP-PBX ���� ���� ��ü
 * @param strKey					key ���ڿ��� ������ ����
 */
void CSipServerMap::GetKey( CXmlSipServer & clsXmlSipServer, std::string & strKey )
{
	strKey = clsXmlSipServer.m_strIp;
	strKey.append( "_" );
	strKey.append( clsXmlSipServer.m_strUserId );
}

/**
 * @brief ��� SIP ���� ���� ���°��� �ʱ�ȭ��Ų��.
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
 * @brief IP-PBX ���� ���� �ڷᱸ���� �������� ������ �߰��ϰ�
 *				IP-PBX ���� ���� �ڷᱸ���� �����ϸ� ���� ���� ���θ� �˻��Ͽ��� ������ ��� IP-PBX ������ �����Ѵ�.
 * @param clsXmlSipServer IP-PBX ���� ���� ��ü
 * @returns true �� �����Ѵ�.
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
		if( strcmp( itMap->second.m_strDomain.c_str(), clsXmlSipServer.m_strDomain.c_str() ) ||
				strcmp( itMap->second.m_strPassWord.c_str(), clsXmlSipServer.m_strPassWord.c_str() ) ||
				itMap->second.m_iPort != clsXmlSipServer.m_iPort )
		{
			clsXmlSipServer.m_iFlag = FLAG_UPDATE;
			itMap->second = clsXmlSipServer;
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
 * @brief IP-PBX ����� ������ �����Ѵ�.
 * @param pszName					IP-PBX �̸�
 * @param clsRoutePrefix	����� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServerMap::InsertRoutePrefix( const char * pszName, CRoutePrefix & clsRoutePrefix )
{
	SIP_SERVER_MAP::iterator	itMap;
	bool bRes = false;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( !strcmp( itMap->second.m_strName.c_str(), pszName ) )
		{
			itMap->second.m_clsRoutePrefixList.push_back( clsRoutePrefix );
			break;
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief �ڷᱸ�� ����͸��� ���� ���ڿ��� �����´�.
 * @param strBuf �ڷᱸ�� ����͸��� ���� ���ڿ� ���� ����
 */
void CSipServerMap::GetString( std::string & strBuf )
{
	SIP_SERVER_MAP::iterator	itMap;
	char	szTemp[51];

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.append( itMap->first );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strName );
		strBuf.append( MR_COL_SEP );

		snprintf( szTemp, sizeof(szTemp), "%d", itMap->second.m_iFlag );
		strBuf.append( szTemp );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strIp );
		snprintf( szTemp, sizeof(szTemp), ":%d", itMap->second.m_iPort );
		strBuf.append( szTemp );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strDomain );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strUserId );
		strBuf.append( MR_COL_SEP );

		snprintf( szTemp, sizeof(szTemp), "%d", itMap->second.m_iLoginTimeout );
		strBuf.append( szTemp );
		strBuf.append( MR_COL_SEP );

		if( itMap->second.m_bLogin )
		{
			strBuf.append( "login" );
		}
		else
		{
			strBuf.append( "logout" );
		}
		strBuf.append( MR_ROW_SEP );
	}
	m_clsMutex.release();
}

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

#include "StdAfx.h"
#include "Setup.h"

CSetup gclsSetup;

CSetup::CSetup() : m_iSipServerPort(5060), m_bUseTwoMedia(false)
{
}

CSetup::~CSetup()
{
}

/**
 * @brief 설정 파일을 읽어서 멤버변수에 저장한다.
 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSetup::Get()
{
	if( GetFile() == false ) return false;

	GetString( ST_SIP_SERVER_IP, m_strSipServerIp );
	m_iSipServerPort = GetInt( ST_SIP_SERVER_PORT, 5060 );
	GetString( ST_SIP_DOMAIN, m_strSipDomain );
	GetString( ST_CALLER_ID, m_strCallerId );
	GetString( ST_CALLER_PW, m_strCallerPassWord );
	GetString( ST_CALLEE_ID, m_strCalleeId );
	GetString( ST_CALLEE_PW, m_strCalleePassWord );
	GetString( ST_CALLEE2_ID, m_strCalleeId2 );
	GetString( ST_CALLEE2_PW, m_strCalleePassWord2 );

#ifdef USE_MEDIA_LIST
	m_bUseTwoMedia = GetBool( ST_USE_TWO_MEDIA, false );
#endif

	m_clsMap.clear();

	return true;
}

/**
 * @brief 멤버변수를 설정 파일로 저장한다.
 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSetup::Put()
{
	PutString( ST_SIP_SERVER_IP, m_strSipServerIp.c_str() );
	PutInt( ST_SIP_SERVER_PORT, m_iSipServerPort );
	PutString( ST_SIP_DOMAIN, m_strSipDomain.c_str() );
	PutString( ST_CALLER_ID, m_strCallerId.c_str() );
	PutString( ST_CALLER_PW, m_strCallerPassWord.c_str() );
	PutString( ST_CALLEE_ID, m_strCalleeId.c_str() );
	PutString( ST_CALLEE_PW, m_strCalleePassWord.c_str() );
	PutString( ST_CALLEE2_ID, m_strCalleeId2.c_str() );
	PutString( ST_CALLEE2_PW, m_strCalleePassWord2.c_str() );
	PutBool( ST_USE_TWO_MEDIA, m_bUseTwoMedia );

	bool bRes = PutFile();

	m_clsMap.clear();

	return bRes;
}

/**
 * @brief 설정 파일을 읽어서 키/값 자료구조에 저장한다.
 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSetup::GetFile()
{
	FILE	* fd = fopen( SETUP_FILENAME, "r" );
	if( fd == NULL ) return false;

	char	szBuf[1024];
	int		iLen;
	std::string	strKey, strValue;

	memset( szBuf, 0, sizeof(szBuf) );
	while( fgets( szBuf, sizeof(szBuf), fd ) )
	{
		iLen = (int)strlen( szBuf );
		if( iLen >= 2 && szBuf[iLen-2] == '\r' )
		{
			szBuf[iLen-2] = '\0';
			iLen -= 2;
		}
		else if( iLen >= 1 && szBuf[iLen-1] == '\n' )
		{
			szBuf[iLen-1] = '\0';
			iLen -= 1;
		}

		strKey.clear();
		strValue.clear();

		for( int i = 0; i < iLen; ++i )
		{
			if( szBuf[i] == '=' )
			{
				strKey.append( szBuf, i );
				strValue.append( szBuf + i + 1 );

				SETUP_MAP::iterator	it;

				it = m_clsMap.find( strKey );
				if( it == m_clsMap.end() )
				{
					m_clsMap.insert( SETUP_MAP::value_type( strKey, strValue ) );
				}

				break;
			}
		}

		memset( szBuf, 0, sizeof(szBuf) );
	}

	fclose( fd );

	return true;
}

/**
 * @brief 키/값 자료구조를 설정 파일에 저장한다.
 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSetup::PutFile()
{
	FILE	* fd = fopen( SETUP_FILENAME, "w" );
	if( fd == NULL ) return false;

	SETUP_MAP::iterator	it;

	for( it = m_clsMap.begin(); it != m_clsMap.end(); ++it )
	{
		fprintf( fd, "%s=%s\r\n", it->first.c_str(), it->second.c_str() );
	}

	fclose( fd );

	return true;
}

int CSetup::GetInt( const char * pszName, int iIndex, int iDefaultValue )
{
	int	iInt;
	char	szIndex[11];
	std::string	strKey, strValue;

	_snprintf( szIndex, sizeof(szIndex), "%d", iIndex );

	strKey = pszName;
	strKey.append( "_" );
	strKey.append( szIndex );

	if( GetString( strKey.c_str(), strValue ) == false ) return iDefaultValue;

	iInt = atoi( strValue.c_str() );

	return iInt;
}

int CSetup::GetInt( const char * pszName, int iDefaultValue )
{
	int	iInt;
	std::string	strValue;

	if( GetString( pszName, strValue ) == false ) return iDefaultValue;

	iInt = atoi( strValue.c_str() );

	return iInt;
}

bool CSetup::GetString( const char * pszName, std::string & strValue )
{
	SETUP_MAP::iterator	it;

	it = m_clsMap.find( pszName );
	if( it != m_clsMap.end() )
	{
		strValue = it->second;
		return true;
	}

	return false;
}

bool CSetup::GetBool( const char * pszName, bool bDefaultValue )
{
	std::string	strValue;

	if( GetString( pszName, strValue ) == false ) return bDefaultValue;

	if( !strcmp( strValue.c_str(), "true" ) ) return true;

	return false;
}

bool CSetup::PutInt( const char * pszName, int iIndex, int iValue )
{
	char	szIndex[11];
	std::string	strKey;

	_snprintf( szIndex, sizeof(szIndex), "%d", iIndex );

	strKey = pszName;
	strKey.append( "_" );
	strKey.append( szIndex );

	_snprintf( szIndex, sizeof(szIndex), "%d", iValue );

	return PutString( strKey.c_str(), szIndex );
}

bool CSetup::PutInt( const char * pszName, int iValue )
{
	char	szValue[11];

	_snprintf( szValue, sizeof(szValue), "%d", iValue );

	return PutString( pszName, szValue );
}

bool CSetup::PutString( const char * pszName, const char * pszValue )
{
	SETUP_MAP::iterator	it;

	it = m_clsMap.find( pszName );
	if( it != m_clsMap.end() )
	{
		it->second = pszValue;
	}
	else
	{
		m_clsMap.insert( SETUP_MAP::value_type( pszName, pszValue ) );
	}

	return true;
}

bool CSetup::PutBool( const char * pszName, bool bValue )
{
	char	szValue[11];

	_snprintf( szValue, sizeof(szValue), "%s", ( bValue ? "true" : "false" ) );

	return PutString( pszName, szValue );
}

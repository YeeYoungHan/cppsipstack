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

CSetup::CSetup(void)
{
}

CSetup::~CSetup(void)
{
}

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

bool CSetup::PutFile()
{
	FILE	* fd = fopen( SETUP_FILENAME, "wb" );
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

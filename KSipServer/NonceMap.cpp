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

#ifndef WIN32
#include <sys/time.h>
#endif

#include <time.h>
#include "NonceMap.h"
#include "Md5.h"
#include "Base64.h"

CNonceMap	gclsNonceMap;

CNonceMap::CNonceMap()
{

}

/** 최신 nonce 값을 가져온다. */
bool CNonceMap::GetNewValue( char * pszNonce, int iNonceSize )
{
	char	szNonce[60], szMd5[33];
	int		i;
	
	if( iNonceSize < 33 ) return false;

#ifdef WIN32
	DWORD	iTime;

	iTime = GetTickCount();
#else
	struct timeval	sttTime;

	gettimeofday( &sttTime, NULL );
#endif
	for( i = 0; ; ++i )
	{
#ifdef WIN32
		snprintf( szNonce, sizeof(szNonce), "%d::%u::%s", i, iTime, PRIVATE_KEY );
#else
		snprintf( szNonce, sizeof(szNonce), "%d::%u.%06u::%s", i
			, (unsigned int)sttTime.tv_sec
			, (unsigned int)sttTime.tv_usec, PRIVATE_KEY );
#endif
		Md5String( szNonce, szMd5 );
		snprintf( szNonce, sizeof(szNonce), "%s", szMd5 );
	
		m_clsMutex.acquire();
		NONCE_MAP::const_iterator it = m_clsMap.find( szNonce );
		if( it == m_clsMap.end() )
		{
			CNonceInfo	clsNonceInfo;

			time(&clsNonceInfo.m_iTime);
			m_clsMap.insert( NONCE_MAP::value_type( szNonce, clsNonceInfo ) );
			m_clsMutex.release();
			break;
		}
		m_clsMutex.release();
	}
	
	snprintf( pszNonce, iNonceSize, "%s", szNonce );
	
	return true;
}

/** nonce 값이 존재하는지 검사한다. */
bool CNonceMap::Select( const char * pszNonce, bool bIsRemove )
{
	bool	bIsFound = false;
	
	m_clsMutex.acquire();
	NONCE_MAP::iterator it = m_clsMap.find( pszNonce );
	if( it != m_clsMap.end() )
	{
		bIsFound = true;
		if( bIsRemove ) m_clsMap.erase( it );
	}
	m_clsMutex.release();
	
	return bIsFound;
}

/** 입력한 시간 이전에 입력된 nonce 값을 모두 삭제한다. */
bool CNonceMap::DeleteTimeout( int iSecond )
{
	NONCE_MAP::iterator it, itNext;
	time_t	iTime;

	time( &iTime );
	m_clsMutex.acquire();
	for( it = m_clsMap.begin(); it != m_clsMap.end(); ++it )
	{
LOOP_START:
		if( iTime > ( it->second.m_iTime + iSecond ) )
		{
			itNext = it;
			++itNext;
			m_clsMap.erase( it );
			if( itNext == m_clsMap.end() )
			{
				break;
			}
			else
			{
				it = itNext;
				goto LOOP_START;
			}
		}
	}
	m_clsMutex.release();
	
	return true;
}

int CNonceMap::GetCount()
{
	int	iCount;

	m_clsMutex.acquire();
	iCount = (int)m_clsMap.size();
	m_clsMutex.release();

	return iCount;
}

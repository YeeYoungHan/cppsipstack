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
#include "SipMd5.h"

CNonceMap	gclsNonceMap;

CNonceMap::CNonceMap()
{

}

/**
 * @ingroup KSipServer
 * @brief 최신 nonce 값을 가져온다.
 * @param pszNonce		nonce 값을 저장할 변수
 * @param iNonceSize	pszNonce 변수의 크기
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
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
		SipMd5String( szNonce, szMd5 );
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

/**
 * @ingroup KSipServer
 * @brief nonce 값이 존재하는지 검색한다.
 * @param pszNonce	nonce 문자열
 * @param bIsRemove 검색된 nonce 를 자료구조에서 삭제하면 true 를 입력하고 그렇지 않으면 false 를 입력한다.
 * @returns nonce 값이 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
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

/**
 * @ingroup KSipServer
 * @brief 입력한 시간 이전에 입력된 nonce 값을 모두 삭제한다.
 * @param iSecond		timeout 시간 (초단위)
 */
void CNonceMap::DeleteTimeout( int iSecond )
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
}

/**
 * @ingroup KSipServer
 * @brief nonce 개수를 리턴한다.
 * @returns nonce 개수를 리턴한다.
 */
int CNonceMap::GetCount()
{
	int	iCount;

	m_clsMutex.acquire();
	iCount = (int)m_clsMap.size();
	m_clsMutex.release();

	return iCount;
}

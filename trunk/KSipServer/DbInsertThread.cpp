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

#include <list>
#include "ServerUtility.h"
#include "DbMySQL.h"
#include "SipServerSetup.h"
#include "SipStackDefine.h"

typedef std::list< std::string > SQL_LIST;
static bool gbDbThreadRun = false;

#ifdef USE_MYSQL
static CSipMutexSignal gclsMutex;
static SQL_LIST gclsSqlList;
static size_t		giMaxSqlCount = 99;
#endif

/**
 * @brief DB INSERT/UPDATE/DELETE SQL 문장을 추가한다.
 * @param strSQL SQL 문장
 */
void DbInsertString( std::string & strSQL )
{
#ifdef USE_MYSQL
	gclsMutex.acquire();
	gclsSqlList.push_back( strSQL );
	if( gclsSqlList.size() > giMaxSqlCount )
	{
		giMaxSqlCount = gclsSqlList.size();
		CLog::Print( LOG_DEBUG, "DbInsertString count(%d)", giMaxSqlCount );
	}
	if( gclsSqlList.size() == 1 ) gclsMutex.signal();
	gclsMutex.release();
#endif
}

/**
 * @brief DB INSERT/UPDATE/DELETE SQL 문장을 추가한다.
 * @param pszSQL SQL 문장
 */
void DbInsert( const char * pszSQL )
{
#ifdef USE_MYSQL
	std::string strSQL = pszSQL;

	DbInsertString( strSQL );
#endif
}

/**
 * @brief SQL 문장 버퍼가 0 인 경우 DB 입력 쓰레드에 신호를 발생한다.
 */
void DbSignal( )
{
#ifdef USE_MYSQL
	gclsMutex.acquire();
	if( gclsSqlList.size() == 0 ) gclsMutex.signal();
	gclsMutex.release();
#endif
}

#ifdef USE_MYSQL
/**
 * @brief DB INSERT/UPDATE/DELETE 쓰레드
 *				주기적으로 SQL 큐를 읽어서 이를 실행한다.
 * @param lpParameter 의미없음
 * @returns 0 을 리턴한다.
 */
#ifdef WIN32
DWORD WINAPI DbInsertThread( LPVOID lpParameter )
#else
void * DbInsertThread( void * lpParameter )
#endif
{
	std::string	strSQL;
	bool	bFound;

	gbDbThreadRun = true;
	CLog::Print( LOG_INFO, "DbInsertThread is started" );

	while( 1 )
	{
		bFound = false;
		gclsMutex.acquire();
		if( gclsSqlList.size() == 0 ) gclsMutex.wait();

		if( gclsSqlList.size() > 0 )
		{
			strSQL = gclsSqlList.front();
			gclsSqlList.pop_front();
			bFound = true;
		}
		gclsMutex.release();

		if( bFound )
		{
			gclsWriteDB.Execute( strSQL.c_str() );
		}
	}

	// 다른 쓰레드가 종료할 때까지 2초 대기한다.
	sleep(2);

	// 큐에 존재하는 모든 SQL 을 실행한다.
	gclsMutex.acquire();
	if( gclsSqlList.size() > 0 )
	{
		SQL_LIST::iterator	it;

		for( it = gclsSqlList.begin(); it != gclsSqlList.end(); ++it )
		{
			gclsWriteDB.Execute( it->c_str() );
		}
	}
	gclsMutex.release();

	gbDbThreadRun = false;
	CLog::Print( LOG_INFO, "DbInsertThread is terminated" );

	return 0;
}
#endif

/**
 * @brief DB 입력 쓰레드를 시작한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool StartDbInsertThread()
{
#ifdef USE_MYSQL
	if( StartThread( "DbInsertThread", DbInsertThread, NULL ) == false ) return false;

	return true;
#else
	return false;
#endif
}

/**
 * @brief DB 입력 쓰레드 실행 유무를 검사한다.
 * @returns DB 입력 쓰레드가 실행 중이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsDbInsertThreadRun()
{
	return gbDbThreadRun;
}

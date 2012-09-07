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

#include "DbMySQL.h"

#ifdef USE_MYSQL

#include "errmsg.h"

CMySQL gclsReadDB;
CMySQL gclsWriteDB;

CMySQL::CMySQL(void) : m_psttRes(NULL), m_bConnected(false), m_iPort(0), m_eLogLevel(LOG_SQL)
{
}

CMySQL::~CMySQL(void)
{
	Close();
}

/**
 * @brief MySQL 데이터베이스에 연결한다.
 * @param pszHost			MySQL 호스트 이름 or IP 주소
 * @param pszUserId		MySQL 접속 아이디
 * @param pszPassWord MySQL 접속 아이디의 비밀번호
 * @param pszDbName		MySQL 접속 데이터베이스 이름
 * @param iPort				MySQL 접속 포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CMySQL::Connect( const char * pszHost, const char * pszUserId, const char * pszPassWord, const char * pszDbName, int iPort )
{
	if( m_bConnected ) return true;

	bool bRes = false;

	m_clsMutex.acquire();
	if( mysql_init( &m_sttMySQL ) == NULL )
	{
		CLog::Print( LOG_ERROR, "mysql_init error" );
	}
	else if( mysql_real_connect( &m_sttMySQL, pszHost, pszUserId, pszPassWord, pszDbName, iPort, NULL, 0 ) == NULL )
	{
		CLog::Print( LOG_ERROR, "mysql_real_connect error - %s", mysql_error( &m_sttMySQL ) );
		mysql_close( &m_sttMySQL );
	}
	else
	{
		CLog::Print( LOG_INFO, "mysql_real_connect success" );
		m_bConnected = true;

		m_strDbHost = pszHost;
		m_strDbUserId = pszUserId;
		m_strDbPassWord = pszPassWord;
		m_strDbName = pszDbName;
		m_iPort = iPort;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief MySQL 데이터베이스 연결을 종료한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CMySQL::Close( bool bLock )
{
	if( m_bConnected == false ) return true;

	if( bLock ) m_clsMutex.acquire();
	mysql_close( &m_sttMySQL );
	m_bConnected = false;
	if( bLock ) m_clsMutex.release();

	return false;
}

/**
 * @brief INSERT, UPDATE, DELETE SQL 문장을 실행한다.
 * @param pszSQL SQL 문장
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CMySQL::Execute( const char * pszSQL )
{
	if( m_bConnected == false ) return false;

	char cTryCount = 0;
	bool bRes = false;

	m_clsMutex.acquire();
QUERY_START:
	if( mysql_query( &m_sttMySQL, pszSQL ) != 0 )
	{
		++cTryCount;
		unsigned int iErrorNo = mysql_errno( &m_sttMySQL );

		if( cTryCount == 1 && ( iErrorNo == CR_SERVER_GONE_ERROR || iErrorNo == CR_SERVER_LOST ) )
		{
			Close( false );
			if( Connect( ) == false )
			{
				CLog::Print( LOG_ERROR, "%s - db connection is closed and db connection error - %s", pszSQL, mysql_error( &m_sttMySQL ) );
			}
			else
			{
				goto QUERY_START;
			}
		}
		else
		{
			CLog::Print( LOG_ERROR, "%s - %s", pszSQL, mysql_error( &m_sttMySQL ) );
		}
	}
	else
	{
		CLog::Print( m_eLogLevel, "%s", pszSQL );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief INSERT SQL 문장을 실행하고 AUTO_INCREMENT 필드의 값을 저장한다.
 * @param pszSQL SQL 문장
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CMySQL::Insert( const char * pszSQL, uint64_t * piId )
{
	if( m_bConnected == false ) return false;

	char cTryCount = 0;
	bool bRes = false;

	m_clsMutex.acquire();
QUERY_START:
	if( mysql_query( &m_sttMySQL, pszSQL ) != 0 )
	{
		++cTryCount;
		unsigned int iErrorNo = mysql_errno( &m_sttMySQL );

		if( cTryCount == 1 && ( iErrorNo == CR_SERVER_GONE_ERROR || iErrorNo == CR_SERVER_LOST ) )
		{
			Close( false );
			if( Connect( ) == false )
			{
				CLog::Print( LOG_ERROR, "%s - db connection is closed and db connection error - %s", pszSQL, mysql_error( &m_sttMySQL ) );
			}
			else
			{
				goto QUERY_START;
			}
		}
		else
		{
			CLog::Print( LOG_ERROR, "%s - %s", pszSQL, mysql_error( &m_sttMySQL ) );
		}
	}
	else
	{
		if( piId )
		{
			*piId = mysql_insert_id( &m_sttMySQL );
		}
		CLog::Print( m_eLogLevel, "%s", pszSQL );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief SELECT SQL 문장을 실행한다.
 * @param pszSQL SQL 문장
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CMySQL::Query( const char * pszSQL )
{
	if( m_bConnected == false ) return false;
	if( m_psttRes ) return false;

	char cTryCount = 0;
	bool bRes = false;

	m_clsMutex.acquire();
QUERY_START:
	if( mysql_query( &m_sttMySQL, pszSQL ) != 0 )
	{
		++cTryCount;
		unsigned int iErrorNo = mysql_errno( &m_sttMySQL );

		if( cTryCount == 1 && ( iErrorNo == CR_SERVER_GONE_ERROR || iErrorNo == CR_SERVER_LOST ) )
		{
			Close( false );
			if( Connect( ) == false )
			{
				CLog::Print( LOG_ERROR, "%s - db connection is closed and db connection error - %s", pszSQL, mysql_error( &m_sttMySQL ) );
			}
			else
			{
				goto QUERY_START;
			}
		}
		else
		{
			CLog::Print( LOG_ERROR, "%s - %s", pszSQL, mysql_error( &m_sttMySQL ) );
		}
	}
	else
	{
		m_psttRes = mysql_use_result( &m_sttMySQL );
		if( m_psttRes == NULL )
		{
			CLog::Print( LOG_ERROR, "mysql_use_result - %s - %s", pszSQL, mysql_error( &m_sttMySQL ) );
		}
		else
		{
			CLog::Print( m_eLogLevel, "%s", pszSQL );
		}
		bRes = true;
	}
	m_clsMutex.release();
	
	return bRes;
}

bool CMySQL::Query( const char * pszSQL, void * pclsData, bool (*FetchRow)( void *, MYSQL_ROW & sttRow ) )
{
	if( m_bConnected == false ) return false;
	if( m_psttRes ) return false;

	char cTryCount = 0;
	bool bRes = false;

	m_clsMutex.acquire();
QUERY_START:
	if( mysql_query( &m_sttMySQL, pszSQL ) != 0 )
	{
		++cTryCount;
		unsigned int iErrorNo = mysql_errno( &m_sttMySQL );

		if( cTryCount == 1 && ( iErrorNo == CR_SERVER_GONE_ERROR || iErrorNo == CR_SERVER_LOST ) )
		{
			Close( false );
			if( Connect( ) == false )
			{
				CLog::Print( LOG_ERROR, "%s - db connection is closed and db connection error - %s", pszSQL, mysql_error( &m_sttMySQL ) );
			}
			else
			{
				goto QUERY_START;
			}
		}
		else
		{
			CLog::Print( LOG_ERROR, "%s - %s", pszSQL, mysql_error( &m_sttMySQL ) );
		}
	}
	else
	{
		MYSQL_RES * psttRes = mysql_use_result( &m_sttMySQL );
		if( psttRes == NULL )
		{
			CLog::Print( LOG_ERROR, "mysql_use_result - %s - %s", pszSQL, mysql_error( &m_sttMySQL ) );
		}
		else
		{
			CLog::Print( m_eLogLevel, "%s", pszSQL );

			MYSQL_ROW sttRow = mysql_fetch_row( psttRes );
			if( sttRow != NULL )
			{
				bRes = FetchRow( pclsData, sttRow );
			}

			mysql_free_result( psttRes );
		}
	}
	m_clsMutex.release();
	
	return bRes;
}

/**
 * @brief SELECT SQL 문 질의 결과에서 하나의 Row 를 가져온다.
 * @param sttRow 하나의 Row 를 저장하는 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CMySQL::FetchRow( MYSQL_ROW & sttRow )
{
	if( m_psttRes == NULL ) return false;

	sttRow = mysql_fetch_row( m_psttRes );
	if( sttRow == NULL )
	{
		FreeResult();
		return false;
	}

	return true;
}

/**
 * @brief SELECT SQL 문 질의 결과를 삭제한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CMySQL::FreeResult( )
{
	if( m_psttRes == NULL ) return true;

	mysql_free_result( m_psttRes );
	m_psttRes = NULL;

	return true;
}

void CMySQL::SetLogLevel( EnumLogLevel eLogLevel )
{
	m_eLogLevel = eLogLevel;
}

bool CMySQL::IsConnected()
{
	return m_bConnected;
}

/**
 * @brief MySQL 데이터베이스에 연결한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CMySQL::Connect( )
{
	if( m_strDbHost.empty() ) return false;

	bool bRes = false;

	if( mysql_init( &m_sttMySQL ) == NULL )
	{
		CLog::Print( LOG_ERROR, "mysql_init error" );
	}
	else if( mysql_real_connect( &m_sttMySQL, m_strDbHost.c_str(), m_strDbUserId.c_str(), m_strDbPassWord.c_str(), m_strDbName.c_str(), 0, NULL, 0 ) == NULL )
	{
		CLog::Print( LOG_ERROR, "mysql_real_connect error - %s", mysql_error( &m_sttMySQL ) );
		mysql_close( &m_sttMySQL );
	}
	else
	{
		CLog::Print( LOG_INFO, "mysql_real_connect success" );
		m_bConnected = true;
		bRes = true;
	}

	return bRes;
}

#endif

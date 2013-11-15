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

#ifndef _MYSQL_CLASS_H_
#define _MYSQL_CLASS_H_

#include "KSipServer.h"

#ifdef USE_MYSQL

#include "SipParserDefine.h"
#include "SipUdp.h"
#include "SipMutex.h"
#include "mysql.h"
#include "Log.h"
#include <string>

void FetchRow( void * pclsData, MYSQL_ROW & sttRow );

/**
 * @ingroup KSipServer
 * @brief MySQL 데이터베이스 연동 클래스
 */
class CMySQL
{
public:
	CMySQL();
	~CMySQL();

	bool Connect( const char * pszHost, const char * pszUserId, const char * pszPassWord, const char * pszDbName, int iPort );
	bool Close( bool bLock = true );
	bool Execute( const char * pszSQL );
	bool Insert( const char * pszSQL, uint64_t * piId );
	bool Query( const char * pszSQL );
	bool Query( const char * pszSQL, void * pclsData, bool (*FetchRow)( void *, MYSQL_ROW & sttRow ) );
	bool FetchRow( MYSQL_ROW & sttRow );
	bool FreeResult( );

	void SetLogLevel( EnumLogLevel eLogLevel );
	bool IsConnected();

private:
	MYSQL	m_sttMySQL;
	MYSQL_RES * m_psttRes;
	bool	m_bConnected;
	CSipMutex	m_clsMutex;

	std::string	m_strDbHost;
	std::string	m_strDbUserId;
	std::string	m_strDbPassWord;
	std::string	m_strDbName;
	int		m_iPort;

	EnumLogLevel m_eLogLevel;

	bool Connect( );
};

extern CMySQL gclsReadDB;
extern CMySQL gclsWriteDB;

#endif

#endif

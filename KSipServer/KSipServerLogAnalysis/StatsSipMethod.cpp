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

#include "StatsSipMethod.h"
#include "LogAnalysisSetup.h"
#include "Directory.h"

CStatsSipMethod gclsStatsSipMethod;

CStatsSipMethod::CStatsSipMethod()
{
}

CStatsSipMethod::~CStatsSipMethod()
{
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP 메소드별 통계를 저장한다.
 * @param pclsMessage SIP 메시지
 */
void CStatsSipMethod::AddSipMessage( CSipMessage * pclsMessage )
{
	STATS_SIP_METHOD_MAP::iterator	itMap;

	itMap = m_clsMap.find( pclsMessage->m_strSipMethod );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( STATS_SIP_METHOD_MAP::value_type( pclsMessage->m_strSipMethod, 1 ) );
	}
	else
	{
		++itMap->second;
	}
}

void CStatsSipMethod::AddSipMethod( const char * pszMethod )
{
	STATS_SIP_METHOD_MAP::iterator	itMap;

	itMap = m_clsMap.find( pszMethod );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( STATS_SIP_METHOD_MAP::value_type( pszMethod, 1 ) );
	}
	else
	{
		++itMap->second;
	}
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP 메소드별 통계를 파일에 저장한다.
 * @param pszDate 통계 생성 날짜
 */
void CStatsSipMethod::SaveFile( const char * pszDate )
{
	STATS_SIP_METHOD_MAP::iterator	itMap;
	FILE * fd;
	std::string strFileName = gclsSetup.m_strResultFolder;
	CDirectory::AppendName( strFileName, "method_" );
	strFileName.append( pszDate );
	strFileName.append( ".csv" );

	fd = fopen( strFileName.c_str(), "w" );
	if( fd == NULL ) return;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		fprintf( fd, "%s,%s," UNSIGNED_LONG_LONG_FORMAT "\n", pszDate, itMap->first.c_str(), itMap->second );
	}

	fclose( fd );
}

STATS_SIP_METHOD_MAP * CStatsSipMethod::GetMap()
{
	return &m_clsMap;
}

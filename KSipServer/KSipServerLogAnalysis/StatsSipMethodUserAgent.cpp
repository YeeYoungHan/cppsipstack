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

#include "StatsSipMethodUserAgent.h"
#include "LogAnalysisSetup.h"
#include "Directory.h"

CStatsSipMethodUserAgent gclsStatsSipMethodUserAgent;

CStatsSipMethodUserAgent::CStatsSipMethodUserAgent()
{
}

CStatsSipMethodUserAgent::~CStatsSipMethodUserAgent()
{
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP 메시지별 SIP UserAgent 를 저장한다.
 * @param pclsMessage SIP 메시지
 */
void CStatsSipMethodUserAgent::AddSipMessage( CSipMessage * pclsMessage )
{
	STATS_SIP_METHOD_USER_AGENT_MAP::iterator	itMap;
	std::string	strKey;

	strKey = pclsMessage->m_strSipMethod;
	strKey.append( "_" );
	strKey.append( pclsMessage->m_strUserAgent );

	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CUserAgent clsUserAgent;

		clsUserAgent.m_strMethod = pclsMessage->m_strSipMethod;
		clsUserAgent.m_strUserAgent = pclsMessage->m_strUserAgent;
		clsUserAgent.m_iCount = 1;

		m_clsMap.insert( STATS_SIP_METHOD_USER_AGENT_MAP::value_type( strKey, clsUserAgent ) );
	}
	else
	{
		++itMap->second.m_iCount;
	}
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP 메시지별 SIP UserAgent 를 파일에 저장한다.
 * @param pszDate 통계 생성 날짜
 */
void CStatsSipMethodUserAgent::SaveFile( const char * pszDate )
{
	STATS_SIP_METHOD_USER_AGENT_MAP::iterator	itMap;
	FILE * fd;
	std::string strFileName = gclsSetup.m_strResultFolder;
	CDirectory::AppendName( strFileName, "method_user_agent_" );
	strFileName.append( pszDate );
	strFileName.append( ".csv" );

	fd = fopen( strFileName.c_str(), "w" );
	if( fd == NULL ) return;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		fprintf( fd, "%s,%s,%s," UNSIGNED_LONG_LONG_FORMAT "\n", pszDate, itMap->second.m_strMethod.c_str(), itMap->second.m_strUserAgent.c_str(), itMap->second.m_iCount );
	}

	fclose( fd );
}

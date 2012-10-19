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

#include "StatsSipMethodIp.h"
#include "DbMySQL.h"

CStatsSipMethodIp gclsStatsSipMethodIp;

CStatsSipMethodIp::CStatsSipMethodIp()
{
}

CStatsSipMethodIp::~CStatsSipMethodIp()
{
}

void CStatsSipMethodIp::AddSipMessage( CSipMessage * pclsMessage, const char * pszIp )
{
	STATS_SIP_METHOD_MAP_IP::iterator	itMap;

	std::string	strKey;

	strKey = pclsMessage->m_strSipMethod;
	strKey.append( "_" );
	strKey.append( pszIp );

	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CIp clsIp;

		clsIp.m_strMethod = pclsMessage->m_strSipMethod;
		clsIp.m_strIp = pszIp;
		clsIp.m_iCount = 1;

		m_clsMap.insert( STATS_SIP_METHOD_MAP_IP::value_type( strKey, clsIp ) );
	}
	else
	{
		++itMap->second.m_iCount;
	}
}

void CStatsSipMethodIp::SaveDB( const char * pszDate )
{
	STATS_SIP_METHOD_MAP_IP::iterator	itMap;
	char	szSQL[255];

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		snprintf( szSQL, sizeof(szSQL), "INSERT INTO StatsMethodIp( Date, Method, Ip, Count ) VALUES( '%s', '%s', '%s', %llu )"
			, pszDate, itMap->second.m_strMethod.c_str(), itMap->second.m_strIp.c_str(), itMap->second.m_iCount );
		gclsWriteDB.Execute( szSQL );
	}
}

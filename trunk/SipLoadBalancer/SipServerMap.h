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

#ifndef _SIP_SERVER_MAP_H_
#define _SIP_SERVER_MAP_H_

#include "SipMutex.h"
#include <vector>

class CSipServerInfo
{
public:
	CSipServerInfo();
	bool Equal( const char * pszIp, int iPort );

	std::string m_strIp;
	int					m_iPort;
	bool				m_bUse;
	bool				m_bDelete;
};

typedef std::vector< CSipServerInfo > SIP_SERVER_LIST;

class CSipServerMap
{
public:
	CSipServerMap();
	~CSipServerMap();

	bool Insert( const char * pszIp, int iPort, bool bUse );
	bool SelectNext( CSipServerInfo & clsInfo );
	bool Select( const char * pszIp, int iPort );
	void SetDeleteAll( );
	void DeleteIfSet( );

private:
	SIP_SERVER_LIST m_clsList;
	CSipMutex m_clsMutex;
	int				m_iIndex;
};

extern CSipServerMap gclsSipServerMap;

#endif

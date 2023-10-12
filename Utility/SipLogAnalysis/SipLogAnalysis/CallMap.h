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

#ifndef _CALL_MAP_H_
#define _CALL_MAP_H_

#include <string>
#include <map>

class CRtpInfo
{
public:
	CRtpInfo() : m_iPort(0)
	{}

	std::string m_strIp;
	int					m_iPort;
};

class CCallInfo
{
public:
	CCallInfo() : m_bFaxError(false)
	{}

	void Print();

	std::string m_strFrom;
	std::string m_strTo;
	std::string m_strCallId;

	CRtpInfo m_clsFromRtp;
	CRtpInfo m_clsToRtp;

	std::string m_strFaxErrorMessage;
	bool		m_bFaxError;

	bool		m_bSend;
};

// key = Call-ID
typedef std::map< std::string, CCallInfo > CALL_MAP;

class CCallMap
{
public:
	bool Insert( CCallInfo & clsInfo );
	bool Update( CCallInfo & clsInfo );
	bool Select( std::string & strCallId, CCallInfo & clsInfo );
	bool Delete( std::string & strCallId );

	bool UpdateWithRtpPort( int iRtpPort, bool bFaxError, std::string & strErrorMessage );

private:
	CALL_MAP m_clsMap;
};

extern CCallMap gclsCallMap;

#endif

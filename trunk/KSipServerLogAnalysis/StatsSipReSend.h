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

#ifndef _STATS_SIP_RESEND_H_
#define _STATS_SIP_RESEND_H_

#include "SipMessage.h"
#include "TimeUtility.h"
#include "LogFile.h"
#include <map>
#include <list>

class CSipSendInfo
{
public:
	struct timeval	m_sttTime;
};

class CSipReSendInfo
{
public:
	std::string			m_strKey;
	struct timeval	m_sttTime;
};

// key = {req/res}_{sip call-id}_{cseq}
typedef std::map< std::string, CSipSendInfo > SIP_SEND_MAP;

typedef std::list< CSipReSendInfo > SIP_RESEND_LIST;

class CStatsSipReSend
{
public:
	CStatsSipReSend();
	~CStatsSipReSend();

	void AddSipMessage( CLogHeader * pclsLogHeader, CSipMessage * pclsMessage );
	void Clear();
	void SaveFile( const char * pszDate );
	void SaveReSendInfoFile( const char * pszLogFileName );

private:
	SIP_SEND_MAP		m_clsMap;
	SIP_RESEND_LIST	m_clsReSendList;

	void GetKey( CSipMessage * pclsMessage, std::string & strKey );
};

extern CStatsSipReSend gclsStatsSipReSend;

#endif

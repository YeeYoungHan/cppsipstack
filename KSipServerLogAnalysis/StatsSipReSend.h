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
#include <map>

class CSipSendInfo
{
public:
	struct timeval	m_sttTime;
};

// key = {req/res}_{sip call-id}_{cseq}
typedef std::map< std::string, CSipSendInfo > SIP_SEND_MAP;

class CStatsSipReSend
{
public:
	CStatsSipReSend();
	~CStatsSipReSend();

	void AddSipMessage( CSipMessage * pclsMessage );
	void Clear();

private:
	SIP_SEND_MAP	m_clsMap;

	void GetKey( CSipMessage * pclsMessage, std::string & strKey );
};

extern CStatsSipReSend gclsStatsSipReSend;

#endif

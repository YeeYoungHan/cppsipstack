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

#ifndef _STATS_SIP_METHOD_USER_AGENT_H_
#define _STATS_SIP_METHOD_USER_AGENT_H_

#include "SipParserDefine.h"
#include "SipMessage.h"
#include <map>

/**
 * @ingroup KSipServerLogAnalysis
 * @brief UserAgent 통계 저장 클래스
 */
class CUserAgent
{
public:
	std::string	m_strMethod;
	std::string	m_strUserAgent;
	uint64_t		m_iCount;
};

typedef std::map< std::string, CUserAgent > STATS_SIP_METHOD_USER_AGENT_MAP;

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP 메소드별 UserAgent 통계 저장 클래스
 */
class CStatsSipMethodUserAgent
{
public:
	CStatsSipMethodUserAgent();
	~CStatsSipMethodUserAgent();

	void AddSipMessage( CSipMessage * pclsMessage );
	void SaveFile( const char * pszDate );

private:
	STATS_SIP_METHOD_USER_AGENT_MAP m_clsMap;
};

extern CStatsSipMethodUserAgent gclsStatsSipMethodUserAgent;

#endif

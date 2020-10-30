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

#ifndef _SIP_CDR_H_
#define _SIP_CDR_H_

#include <string>
#include "SipUdp.h"

/**
 * @ingroup SipUserAgent
 * @brief SIP CDR 정보를 저장하는 클래스
 */
class CSipCdr
{
public:
	CSipCdr();
	~CSipCdr();

	/** SIP From 헤더에 저장되는 사용자 아이디 */
	std::string	m_strFromId;

	/** SIP To 헤더에 저장되는 사용자 아이디 */
	std::string	m_strToId;

	/** SIP Call-ID */
	std::string m_strCallId;

	/** INVITE 전송/수신 시간 */
	struct timeval m_sttInviteTime;

	/** 통화 시작 시간 */
	struct timeval m_sttStartTime;

	/** 통화 종료 시간 */
	struct timeval m_sttEndTime;
};

#endif

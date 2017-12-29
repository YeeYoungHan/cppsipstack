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

#ifndef _RTP_THREAD_H_
#define _RTP_THREAD_H_

#include "SipUdp.h"

class CRtpThreadArg
{
public:
	CRtpThreadArg();
	~CRtpThreadArg();

	bool CreateSocket();
	void Close();

	Socket	m_hWebRtcUdp;
	Socket	m_hPbxUdp;

	int			m_iWebRtcUdpPort;
	int			m_iPbxUdpPort;

	bool		m_bStop;

	std::string	m_strUserId;
	std::string m_strToId;
	std::string m_strSdp;
};

void InitDtls();
bool StartRtpThread( CRtpThreadArg * pclsArg );

#endif

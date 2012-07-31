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

#ifndef _SIP_DIALOG_H_
#define _SIP_DIALOG_H_

#include <string>
#include "SipUdp.h"
#include "SipMessage.h"

class CSipDialog
{
public:
	CSipDialog();
	~CSipDialog();

	std::string	m_strFromId;
	std::string	m_strFromTag;
	std::string	m_strToId;
	std::string	m_strToTag;
	std::string m_strCallId;
	std::string m_strViaBranch;
	int					m_iSeq;

	std::string	m_strContactIp;
	int					m_iContactPort;

	std::string	m_strLocalRtpIp;
	int					m_iLocalRtpPort;

	std::string	m_strRemoteRtpIp;
	int					m_iRemoteRtpPort;

	int					m_iCodec;

	struct timeval m_sttInviteTime;
	struct timeval m_sttCancelTime;
	struct timeval m_sttStartTime;
	struct timeval m_sttEndTime;

	CSipMessage * CreateInvite( );
	CSipMessage * CreateAck( );
	CSipMessage * CreateCancel( );
	CSipMessage * CreateBye( );
	bool AddSdp( CSipMessage * pclsMessage );

private:
	CSipMessage * CreateMessage( const char * pszSipMethod );
};

#endif

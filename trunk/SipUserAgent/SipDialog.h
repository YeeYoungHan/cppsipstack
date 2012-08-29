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

enum ERtpDirection;
class CSipCallRtp;

/**
 * @ingroup SipUserAgent
 * @brief SIP Dialog ������ �����ϴ� Ŭ����
 */
class CSipDialog
{
public:
	CSipDialog();
	~CSipDialog();

	/** SIP From ����� ����Ǵ� ����� ���̵� */
	std::string	m_strFromId;

	/** SIP From ����� ����Ǵ� tag */
	std::string	m_strFromTag;

	/** SIP To ����� ����Ǵ� ����� ���̵� */
	std::string	m_strToId;

	/** SIP To ����� ����Ǵ� tag */
	std::string	m_strToTag;

	/** SIP Call-ID */
	std::string m_strCallId;

	/** SIP Top Via ����� branch */
	std::string m_strViaBranch;

	/** SIP CSeq ����� ��ȣ */
	int					m_iSeq;

	/** SIP ��û �޽����� ������ IP �ּ� */
	std::string	m_strContactIp;

	/** SIP ��û �޽����� ������ ��Ʈ ��ȣ */
	int					m_iContactPort;

	/** local RTP IP �ּ� */
	std::string	m_strLocalRtpIp;

	/** local RTP ��Ʈ ��ȣ */
	int					m_iLocalRtpPort;

	/** local RTP direction ( sendrecv, sendonly, recvonly, inactive ) */
	ERtpDirection	m_eLocalDirection;

	/** remote RTP IP �ּ� */
	std::string	m_strRemoteRtpIp;

	/** remote RTP ��Ʈ ��ȣ */
	int					m_iRemoteRtpPort;

	/** remote RTP direction ( sendrecv, sendonly, recvonly, inactive ) */
	ERtpDirection	m_eRemoteDirection;

	/** �ڵ� */
	int					m_iCodec;

	/** SIP ��û �޽����� ����� Request Uri */
	std::string	m_strContactUri;

	/** INVITE ����/���� �ð� */
	struct timeval m_sttInviteTime;

	/** CANCEL ���� �ð� */
	struct timeval m_sttCancelTime;

	/** ��ȭ ���� �ð� */
	struct timeval m_sttStartTime;

	/** ��ȭ ���� �ð� */
	struct timeval m_sttEndTime;

	/** ���ŵ� INVITE �޽��� */
	CSipMessage * m_pclsInvite;

	CSipMessage * CreateInvite( );
	CSipMessage * CreateAck( );
	CSipMessage * CreateCancel( );
	CSipMessage * CreateBye( );
	CSipMessage * CreateNotify( );
	bool AddSdp( CSipMessage * pclsMessage );
	bool SetLocalRtp( CSipCallRtp * pclsRtp );
	bool SetRemoteRtp( CSipCallRtp * pclsRtp );

private:
	CSipMessage * CreateMessage( const char * pszSipMethod );
};

#endif

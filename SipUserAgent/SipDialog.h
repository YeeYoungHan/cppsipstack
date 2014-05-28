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

#include "SipCdr.h"
#include "SipMessage.h"
#include "SipUserAgentCallBack.h"

/**
 * @ingroup SipUserAgent
 * @brief SIP Dialog 정보를 저장하는 클래스
 */
class CSipDialog
{
public:
	CSipDialog( CSipStack * pclsSipStack );
	~CSipDialog();

	/** SIP From 헤더에 저장되는 사용자 아이디 */
	std::string	m_strFromId;

	/** SIP From 헤더에 저장되는 tag */
	std::string	m_strFromTag;

	/** SIP To 헤더에 저장되는 사용자 아이디 */
	std::string	m_strToId;

	/** SIP To 헤더에 저장되는 tag */
	std::string	m_strToTag;

	/** SIP Call-ID */
	std::string m_strCallId;

	/** SIP Top Via 헤더의 branch */
	std::string m_strViaBranch;

	/** SIP CSeq 헤더의 번호 */
	int					m_iSeq;

	/** SIP 요청 메시지를 전송할 IP 주소 */
	std::string	m_strContactIp;

	/** SIP 요청 메시지를 전송할 포트 번호 */
	int					m_iContactPort;

	/** SIP 요청 메시지를 전송할 transport */
	ESipTransport	m_eTransport;

	/** local RTP IP 주소 */
	std::string	m_strLocalRtpIp;

	/** local RTP 포트 번호 */
	int					m_iLocalRtpPort;

	/** local RTP direction ( sendrecv, sendonly, recvonly, inactive ) */
	ERtpDirection	m_eLocalDirection;

#ifdef USE_MEDIA_LIST
	/** local media list */
	SDP_MEDIA_LIST	m_clsLocalMediaList;
#endif

	/** remote RTP IP 주소 */
	std::string	m_strRemoteRtpIp;

	/** remote RTP 포트 번호 */
	int					m_iRemoteRtpPort;

	/** remote RTP direction ( sendrecv, sendonly, recvonly, inactive ) */
	ERtpDirection	m_eRemoteDirection;

#ifdef USE_MEDIA_LIST
	/** remote media list */
	SDP_MEDIA_LIST	m_clsRemoteMediaList;
#endif

	/** 코덱 */
	int					m_iCodec;

	CODEC_LIST	m_clsCodecList;

	/** SIP 요청 메시지에 저장될 Request Uri */
	std::string	m_strContactUri;

	/** INVITE 전송/수신 시간 */
	struct timeval m_sttInviteTime;

	/** CANCEL 전송 시간 */
	struct timeval m_sttCancelTime;

	/** 통화 시작 시간 */
	struct timeval m_sttStartTime;

	/** 통화 종료 시간 */
	struct timeval m_sttEndTime;

	/** 수신된 INVITE 메시지 */
	CSipMessage * m_pclsInvite;

	CSipStack		* m_pclsSipStack;

	CSipMessage * CreateInvite( );
	CSipMessage * CreateAck( );
	CSipMessage * CreateCancel( );
	CSipMessage * CreateBye( );
	CSipMessage * CreateNotify( );
	CSipMessage * CreateRefer( );

	bool AddSdp( CSipMessage * pclsMessage );
	bool SetLocalRtp( CSipCallRtp * pclsRtp );
	bool SetRemoteRtp( CSipCallRtp * pclsRtp );
	bool SelectRemoteRtp( CSipCallRtp * pclsRtp );
	void GetCdr( CSipCdr * pclsCdr );
	bool IsConnected( );

private:
	CSipMessage * CreateMessage( const char * pszSipMethod );
};

#endif

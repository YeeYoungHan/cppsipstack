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
 * @brief SIP Dialog ������ �����ϴ� Ŭ����
 */
class CSipDialog
{
public:
	CSipDialog( CSipStack * pclsSipStack );
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

	/** SIP CSeq ����� ���� ��ȣ ( PRACK ������ ������ �޽����� ���ؼ� ���� ) */
	int					m_iNextSeq;

	/** SIP ��û �޽����� ������ IP �ּ� */
	std::string	m_strContactIp;

	/** SIP ��û �޽����� ������ ��Ʈ ��ȣ */
	int					m_iContactPort;

	/** SIP ��û �޽����� ������ transport */
	ESipTransport	m_eTransport;

	/** local RTP IP �ּ� */
	std::string	m_strLocalRtpIp;

	/** local RTP ��Ʈ ��ȣ */
	int					m_iLocalRtpPort;

	/** local RTP direction ( sendrecv, sendonly, recvonly, inactive ) */
	ERtpDirection	m_eLocalDirection;

#ifdef USE_MEDIA_LIST
	/** local media list */
	SDP_MEDIA_LIST	m_clsLocalMediaList;
#endif

	/** remote RTP IP �ּ� */
	std::string	m_strRemoteRtpIp;

	/** remote RTP ��Ʈ ��ȣ */
	int					m_iRemoteRtpPort;

	/** remote RTP direction ( sendrecv, sendonly, recvonly, inactive ) */
	ERtpDirection	m_eRemoteDirection;

#ifdef USE_MEDIA_LIST
	/** remote media list */
	SDP_MEDIA_LIST	m_clsRemoteMediaList;
#endif

	/** �ڵ� */
	int					m_iCodec;

	CODEC_LIST	m_clsCodecList;

	/** SIP ��û �޽����� ����� Request Uri */
	std::string	m_strContactUri;

	/** RSeq */
	int					m_iRSeq;

	/** 100rel */
	bool				m_b100rel;

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

	/** ���ŵ� INVITE �޽����� ����� Record-Route ����Ʈ�� ������ Route ����Ʈ */
	SIP_FROM_LIST	m_clsRouteList;

	CSipStack		* m_pclsSipStack;

	/** �߽� ��ȭ�ΰ�? */
	bool m_bSendCall;

	CSipMessage * CreateInvite( );
	CSipMessage * CreateAck( );
	CSipMessage * CreateCancel( );
	CSipMessage * CreateBye( );
	CSipMessage * CreateNotify( );
	CSipMessage * CreateRefer( );
	CSipMessage * CreatePrack( );

	bool AddSdp( CSipMessage * pclsMessage );
	
	bool SetLocalRtp( CSipCallRtp * pclsRtp );
	bool SetRemoteRtp( CSipCallRtp * pclsRtp );

	bool SelectLocalRtp( CSipCallRtp * pclsRtp );
	bool SelectRemoteRtp( CSipCallRtp * pclsRtp );

	void GetCdr( CSipCdr * pclsCdr );
	bool IsConnected( );

private:
	CSipMessage * CreateMessage( const char * pszSipMethod );
};

#endif

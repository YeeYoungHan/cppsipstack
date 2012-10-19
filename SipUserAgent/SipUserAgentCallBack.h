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

#ifndef _SIP_USER_AGENT_CALLBACK_H_
#define _SIP_USER_AGENT_CALLBACK_H_

#include "SipStackDefine.h"
#include "SipServerInfo.h"

/**
 * @ingroup SipUserAgent
 * @brief RTP 전송/수신
 */
enum ERtpDirection
{
	E_RTP_SEND_RECV = 0,
	E_RTP_SEND,
	E_RTP_RECV,
	E_RTP_INACTIVE
};

typedef std::list< int > CODEC_LIST;

/**
 * @ingroup SipUserAgent
 * @brief RTP 정보 저장 클래스
 */
class CSipCallRtp
{
public:
	CSipCallRtp() : m_iPort(-1), m_iCodec(-1), m_eDirection( E_RTP_SEND_RECV )
	{}

	std::string	m_strIp;
	int					m_iPort;
	int					m_iCodec;
	ERtpDirection	m_eDirection;
	CODEC_LIST	m_clsCodecList;
};

/**
 * @ingroup SipUserAgent
 * @brief CSipUserAgent 의 이벤트를 응용 프로그램으로 전달하는 callback 인터페이스
 */
class ISipUserAgentCallBack
{
public:
	virtual ~ISipUserAgentCallBack(){};

	/**
   * @ingroup SipUserAgent
	 * @brief SIP REGISTER 응답 메시지 수신 이벤트 핸들러
	 * @param pclsInfo	SIP REGISTER 응답 메시지를 전송한 IP-PBX 정보 저장 객체
	 * @param iStatus		SIP REGISTER 응답 코드
	 */
	virtual void EventRegister( CSipServerInfo * pclsInfo, int iStatus ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP 통화 요청 수신에 대한 인증 확인 이벤트 핸들러
	 * @param pclsMessage	SIP INVITE 요청 메시지
	 * @return 인증에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool EventIncomingRequestAuth( CSipMessage * pclsMessage ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP 통화 요청 수신 이벤트 핸들러
	 * @param	pszCallId	SIP Call-ID
	 * @param pszFrom		SIP From 사용자 아이디
	 * @param pszTo			SIP To 사용자 아이디
	 * @param pclsRtp		RTP 정보 저장 객체
	 */
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP Ring / Session Progress 수신 이벤트 핸들러
	 * @param	pszCallId		SIP Call-ID
	 * @param iSipStatus	SIP 응답 코드
	 * @param pclsRtp			RTP 정보 저장 객체
	 */
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP 통화 연결 이벤트 핸들러
	 * @param	pszCallId	SIP Call-ID
	 * @param pclsRtp		RTP 정보 저장 객체
	 */
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP 통화 종료 이벤트 핸들러
	 * @param	pszCallId		SIP Call-ID
	 * @param iSipStatus	SIP 응답 코드. INVITE 에 대한 오류 응답으로 전화가 종료된 경우, INVITE 의 응답 코드를 저장한다.
	 */
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP ReINVITE 수신 이벤트 핸들러
	 * @param	pszCallId	SIP Call-ID
	 * @param pclsRtp		RTP 정보 저장 객체
	 */
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief Screened / Unscreened Transfer 요청 수신 이벤트 핸들러
	 * @param pszCallId					SIP Call-ID
	 * @param pszReferToCallId	전화가 전달될 SIP Call-ID
	 * @param bScreenedTransfer Screened Transfer 이면 true 가 입력되고 Unscreened Transfer 이면 false 가 입력된다.
	 * @returns 요청을 수락하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief Blind Transfer 요청 수신 이벤트 핸들러
	 * @param pszCallId			SIP Call-ID
	 * @param pszReferToId	전화가 전달될 사용자 아이디
	 * @returns 요청을 수락하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool EventBlindTransfer( const char * pszCallId, const char * pszReferToId ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP MESSAGE 수신 이벤트 핸들러
	 * @param pszFrom		SIP From 사용자 아이디
	 * @param pszTo			SIP To 사용자 아이디
	 * @param pclsMessage	SIP 메시지
	 */
	virtual bool EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief 1초 단위로 실행되는 타이머 이벤트 핸들러
	 */
	virtual void EventTimer( ) = 0;
};

#endif

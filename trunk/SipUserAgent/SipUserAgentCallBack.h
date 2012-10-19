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
 * @brief RTP ����/����
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
 * @brief RTP ���� ���� Ŭ����
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
 * @brief CSipUserAgent �� �̺�Ʈ�� ���� ���α׷����� �����ϴ� callback �������̽�
 */
class ISipUserAgentCallBack
{
public:
	virtual ~ISipUserAgentCallBack(){};

	/**
   * @ingroup SipUserAgent
	 * @brief SIP REGISTER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
	 * @param pclsInfo	SIP REGISTER ���� �޽����� ������ IP-PBX ���� ���� ��ü
	 * @param iStatus		SIP REGISTER ���� �ڵ�
	 */
	virtual void EventRegister( CSipServerInfo * pclsInfo, int iStatus ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP ��ȭ ��û ���ſ� ���� ���� Ȯ�� �̺�Ʈ �ڵ鷯
	 * @param pclsMessage	SIP INVITE ��û �޽���
	 * @return ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
	 */
	virtual bool EventIncomingRequestAuth( CSipMessage * pclsMessage ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP ��ȭ ��û ���� �̺�Ʈ �ڵ鷯
	 * @param	pszCallId	SIP Call-ID
	 * @param pszFrom		SIP From ����� ���̵�
	 * @param pszTo			SIP To ����� ���̵�
	 * @param pclsRtp		RTP ���� ���� ��ü
	 */
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP Ring / Session Progress ���� �̺�Ʈ �ڵ鷯
	 * @param	pszCallId		SIP Call-ID
	 * @param iSipStatus	SIP ���� �ڵ�
	 * @param pclsRtp			RTP ���� ���� ��ü
	 */
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
	 * @param	pszCallId	SIP Call-ID
	 * @param pclsRtp		RTP ���� ���� ��ü
	 */
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
	 * @param	pszCallId		SIP Call-ID
	 * @param iSipStatus	SIP ���� �ڵ�. INVITE �� ���� ���� �������� ��ȭ�� ����� ���, INVITE �� ���� �ڵ带 �����Ѵ�.
	 */
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP ReINVITE ���� �̺�Ʈ �ڵ鷯
	 * @param	pszCallId	SIP Call-ID
	 * @param pclsRtp		RTP ���� ���� ��ü
	 */
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief Screened / Unscreened Transfer ��û ���� �̺�Ʈ �ڵ鷯
	 * @param pszCallId					SIP Call-ID
	 * @param pszReferToCallId	��ȭ�� ���޵� SIP Call-ID
	 * @param bScreenedTransfer Screened Transfer �̸� true �� �Էµǰ� Unscreened Transfer �̸� false �� �Էµȴ�.
	 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
	 */
	virtual bool EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief Blind Transfer ��û ���� �̺�Ʈ �ڵ鷯
	 * @param pszCallId			SIP Call-ID
	 * @param pszReferToId	��ȭ�� ���޵� ����� ���̵�
	 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
	 */
	virtual bool EventBlindTransfer( const char * pszCallId, const char * pszReferToId ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief SIP MESSAGE ���� �̺�Ʈ �ڵ鷯
	 * @param pszFrom		SIP From ����� ���̵�
	 * @param pszTo			SIP To ����� ���̵�
	 * @param pclsMessage	SIP �޽���
	 */
	virtual bool EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage ) = 0;

	/**
   * @ingroup SipUserAgent
	 * @brief 1�� ������ ����Ǵ� Ÿ�̸� �̺�Ʈ �ڵ鷯
	 */
	virtual void EventTimer( ) = 0;
};

#endif

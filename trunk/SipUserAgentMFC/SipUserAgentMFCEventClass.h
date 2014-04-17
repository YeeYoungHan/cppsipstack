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

#ifndef _SIP_USER_AGENT_MFC_EVENT_CLASS_H_
#define _SIP_USER_AGENT_MFC_EVENT_CLASS_H_

/**
 * @ingroup SipUserAgentMFC
 * @brief EventRegister 정보 전달 클래스
 */
class CEventRegister
{
public:
	CEventRegister( CSipServerInfo * pclsInfo, int iStatus )
	{
		m_clsInfo = *pclsInfo;
		m_iStatus = iStatus;
	}

	CSipServerInfo m_clsInfo;
	int		m_iStatus;
};

/**
 * @ingroup SipUserAgentMFC
 * @brief EventIncomingCall 정보 전달 클래스
 */
class CEventIncomingCall
{
public:
	CEventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
	{
		m_pszCallId = pszCallId;
		m_pszFrom = pszFrom;
		m_pszTo = pszTo;
		m_pclsRtp = pclsRtp;
	}

	const char * m_pszCallId;
	const char * m_pszFrom;
	const char * m_pszTo;
	CSipCallRtp * m_pclsRtp;
};

/**
 * @ingroup SipUserAgentMFC
 * @brief EventCallRing 정보 전달 클래스
 */
class CEventCallRing
{
public:
	CEventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
	{
		m_pszCallId = pszCallId;
		m_iSipStatus = iSipStatus;
		m_pclsRtp = pclsRtp;
	}

	const char * m_pszCallId;
	int m_iSipStatus;
	CSipCallRtp * m_pclsRtp;
};

/**
 * @ingroup SipUserAgentMFC
 * @brief EventCallStart 정보 전달 클래스
 */
class CEventCallStart
{
public:
	CEventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
	{
		m_pszCallId = pszCallId;
		m_pclsRtp = pclsRtp;
	}

	const char * m_pszCallId;
	CSipCallRtp * m_pclsRtp;
};

/**
 * @ingroup SipUserAgentMFC
 * @brief EventCallEnd 정보 전달 클래스
 */
class CEventCallEnd
{
public:
	CEventCallEnd( const char * pszCallId, int iSipStatus )
	{
		m_pszCallId = pszCallId;
		m_iSipStatus = iSipStatus;
	}

	const char * m_pszCallId;
	int m_iSipStatus;
};

/**
 * @ingroup SipUserAgentMFC
 * @brief EventTransfer 정보 전달 클래스
 */
class CEventTransfer
{
public:
	CEventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
	{
		m_pszCallId = pszCallId;
		m_pszReferToCallId = pszReferToCallId;
		m_bScreenedTransfer = bScreenedTransfer;
	}

	const char * m_pszCallId;
	const char * m_pszReferToCallId;
	bool m_bScreenedTransfer;
};

/**
 * @ingroup SipUserAgentMFC
 * @brief EventBlindTransfer 정보 전달 클래스
 */
class CEventBlindTransfer
{
public:
	CEventBlindTransfer( const char * pszCallId, const char * pszReferToId )
	{
		m_pszCallId = pszCallId;
		m_pszReferToId = pszReferToId;
	}

	const char * m_pszCallId;
	const char * m_pszReferToId;
};

/**
 * @ingroup SipUserAgentMFC
 * @brief SIP 메시지 수신 정보 전달 클래스
 */
class CEventSipMessage
{
public:
	CEventSipMessage( CSipMessage * pclsSipMessage )
	{
		m_pclsSipMessage = pclsSipMessage;
	}

	CSipMessage * m_pclsSipMessage;
};

#endif

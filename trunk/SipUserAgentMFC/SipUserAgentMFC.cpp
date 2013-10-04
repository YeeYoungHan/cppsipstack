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

#include "StdAfx.h"
#include "SipUserAgentMFC.h"
#include "SipUserAgentMFCDefine.h"
#include "SipUserAgentMFCEventClass.h"

CSipUserAgentMFC::CSipUserAgentMFC() : m_hWnd(0), m_pclsCallBack(NULL)
{
}

CSipUserAgentMFC::~CSipUserAgentMFC()
{
}

/**
 * @ingroup SipClientMFC
 * @brief SipUserAgent �� ������ �̺�Ʈ�� ������ ������ �ڵ��� ����Ѵ�.
 * @param hWnd ������ �ڵ�
 */
void CSipUserAgentMFC::SetWindowHandle( HWND hWnd )
{
	m_hWnd = hWnd;
}

/**
 * @ingroup SipClientMFC
 * @brief ������ ������ ����� SipUserAgent callback �������̽��� ����Ѵ�.
 * @param pclsCallBack SipUserAgent callback �������̽�
 */
void CSipUserAgentMFC::SetCallBack( ISipUserAgentCallBack * pclsCallBack )
{
	m_pclsCallBack = pclsCallBack;
}

/**
 * @ingroup SipClientMFC
 * @brief ������ �޽��� ���� callback �޼ҵ�
 * @param wParam 
 * @param lParam 
 * @returns 0 �� �����Ѵ�.
 */
LRESULT CSipUserAgentMFC::OnSipMessage( WPARAM wParam, LPARAM lParam )
{
	if( m_pclsCallBack )
	{
		switch( wParam )
		{
		case SMC_REGISER:
			{
				CEventRegister * pclsParam = (CEventRegister *)lParam;
				m_pclsCallBack->EventRegister( &pclsParam->m_clsInfo, pclsParam->m_iStatus );
				delete pclsParam;
			}
			break;
		case SMC_INCOMING_CALL:
			{
				CEventIncomingCall * pclsParam = (CEventIncomingCall *)lParam;
				m_pclsCallBack->EventIncomingCall( pclsParam->m_pszCallId, pclsParam->m_pszFrom, pclsParam->m_pszTo, pclsParam->m_pclsRtp );
			}
			break;
		case SMC_CALL_RINGING:
			{
				CEventCallRing * pclsParam = (CEventCallRing *)lParam;
				m_pclsCallBack->EventCallRing( pclsParam->m_pszCallId, pclsParam->m_iSipStatus, pclsParam->m_pclsRtp );
			}
			break;
		case SMC_CALL_START:
			{
				CEventCallStart * pclsParam = (CEventCallStart *)lParam;
				m_pclsCallBack->EventCallStart( pclsParam->m_pszCallId, pclsParam->m_pclsRtp );
			}
			break;
		case SMC_CALL_END:
			{
				CEventCallEnd * pclsParam = (CEventCallEnd *)lParam;
				m_pclsCallBack->EventCallEnd( pclsParam->m_pszCallId, pclsParam->m_iSipStatus );
			}
			break;
		}
	}

	return WMR_TRUE;
}

/**
 * @ingroup SipClientMFC
 * @brief SIP REGISTER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param pclsInfo	SIP REGISTER ���� �޽����� ������ IP-PBX ���� ���� ��ü
 * @param iStatus		SIP REGISTER ���� �ڵ�
 */
void CSipUserAgentMFC::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	CEventRegister * pclsParam = new CEventRegister( pclsInfo, iStatus );
	if( pclsParam == NULL ) return;

	_SendMessage( SMC_REGISER, (LPARAM)pclsParam );
}

/**
 * @ingroup SipClientMFC
 * @brief SIP ��ȭ ��û ���ſ� ���� ���� Ȯ�� �̺�Ʈ �ڵ鷯
 * @param pclsMessage	SIP INVITE ��û �޽���
 * @return ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipUserAgentMFC::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	return true;
}

/**
 * @ingroup SipClientMFC
 * @brief SIP ��ȭ ��û ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From ����� ���̵�
 * @param pszTo			SIP To ����� ���̵�
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipUserAgentMFC::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	CEventIncomingCall clsParam( pszCallId, pszFrom, pszTo, pclsRtp );

	_SendMessage( SMC_INCOMING_CALL, (LPARAM)&clsParam );
}

/**
 * @ingroup SipClientMFC
 * @brief SIP Ring / Session Progress ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�
 * @param pclsRtp			RTP ���� ���� ��ü
 */
void CSipUserAgentMFC::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	CEventCallRing clsParam( pszCallId, iSipStatus, pclsRtp );

	_SendMessage( SMC_CALL_RINGING, (LPARAM)&clsParam );
}

/**
 * @ingroup SipClientMFC
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipUserAgentMFC::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	CEventCallStart clsParam( pszCallId, pclsRtp );

	_SendMessage( SMC_CALL_START, (LPARAM)&clsParam );
}

/**
 * @ingroup SipClientMFC
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�. INVITE �� ���� ���� �������� ��ȭ�� ����� ���, INVITE �� ���� �ڵ带 �����Ѵ�.
 */
void CSipUserAgentMFC::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	CEventCallEnd clsParam( pszCallId, iSipStatus );

	_SendMessage( SMC_CALL_END, (LPARAM)&clsParam );
}

/**
 * @ingroup SipClientMFC
 * @brief SIP ReINVITE ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipUserAgentMFC::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

/**
 * @ingroup SipClientMFC
 * @brief Screened / Unscreened Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId					SIP Call-ID
 * @param pszReferToCallId	��ȭ�� ���޵� SIP Call-ID
 * @param bScreenedTransfer Screened Transfer �̸� true �� �Էµǰ� Unscreened Transfer �̸� false �� �Էµȴ�.
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipUserAgentMFC::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	return false;
}

/**
 * @ingroup SipClientMFC
 * @brief Blind Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId			SIP Call-ID
 * @param pszReferToId	��ȭ�� ���޵� ����� ���̵�
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipUserAgentMFC::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	return false;
}

/**
 * @ingroup SipClientMFC
 * @brief SIP MESSAGE ���� �̺�Ʈ �ڵ鷯
 * @param pszFrom			SIP �޽��� ���� ���̵�
 * @param pszTo				SIP �޽��� ���� ���̵�
 * @param pclsMessage SIP �޽���
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipUserAgentMFC::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	return false;
}

/**
 * @ingroup SipClientMFC
 * @brief SIP �޽��� ���� �����尡 ������� �˷��ִ� �̺�Ʈ �ڵ鷯
 * @param iThreadId UDP ������ ��ȣ
 */
void CSipUserAgentMFC::EventCallBackThreadEnd( int iThreadId )
{
}

/**
 * @ingroup SipClientMFC
 * @brief ������ �޽����� SIP �̺�Ʈ�� �����Ѵ�.
 * @param wParam SIP �̺�Ʈ ��� �ڵ�
 * @param lParam SIP �̺�Ʈ ������
 * @returns �����ϸ� WMR_TRUE �� �����ϰ� �����ϸ� WMR_FALSE �� �����Ѵ�.
 */
LRESULT CSipUserAgentMFC::_SendMessage( WPARAM wParam, LPARAM lParam )
{
	if( m_hWnd == 0 ) return WMR_FALSE;

	if( wParam == SMC_REGISER )
	{
		PostMessage( m_hWnd, SIP_MESSAGE_ID, wParam, lParam );
		return WMR_TRUE;
	}

	return SendMessage( m_hWnd, SIP_MESSAGE_ID, wParam, lParam );
}

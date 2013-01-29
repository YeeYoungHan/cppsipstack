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

#include "SipMessage.h"

/**
 * @ingroup McuControlServer
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

	/** SIP ��û �޽����� ����� Request Uri */
	std::string	m_strContactUri;

	/** ���ŵ� INVITE �޽��� */
	CSipMessage * m_pclsInvite;

	std::string	m_strLocalSdp;
	std::string m_strRemoteSdp;

	CSipMessage * CreateInvite( );
	CSipMessage * CreateAck( );
	CSipMessage * CreateCancel( );
	CSipMessage * CreateBye( );
	CSipMessage * CreateNotify( );
	bool AddSdp( CSipMessage * pclsMessage );
	bool SetLocalSdp( const char * pszSdp );
	bool SetRemoteSdp( const char * pszSdp );

private:
	CSipMessage * CreateMessage( const char * pszSipMethod );
};

#endif

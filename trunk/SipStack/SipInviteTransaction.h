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

#ifndef _SIP_INVITE_TRANSACTION_H_
#define _SIP_INVITE_TRANSACTION_H_

#include "SipStackDefine.h"

/**
 * @ingroup SipStack
 * @brief SIP INVITE TRANSACTION Ŭ����
 */
class CSipInviteTransaction
{
public:
	CSipInviteTransaction();
	~CSipInviteTransaction();

public:
	/** SIP ��û �޽��� */
	CSipMessage * m_pclsRequest;

	/** SIP ���� �޽��� */
	CSipMessage * m_pclsResponse;

	/** ACK �޽��� */
	CSipMessage * m_pclsAck;

	/** SIP ��û �޽��� ����/���� �ð� */
	struct timeval m_sttStartTime;

	/** SIP transaction ���� �ð� */
	struct timeval m_sttStopTime;

	/** 180/183 ���� �޽��� ���� �ð� */
	struct timeval m_sttRingTime;

	/** ������ ���� */
	int	m_iReSendCount;

	/** ���� SIP ���� �ڵ� */
	int m_iStatusCode;
};

typedef MAP< std::string, CSipInviteTransaction * > INVITE_TRANSACTION_MAP;

#endif

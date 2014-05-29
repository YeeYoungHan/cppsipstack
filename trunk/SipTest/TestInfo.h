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

#ifndef _TEST_INFO_H_
#define _TEST_INFO_H_

#include "SipUdp.h"
#include "SipUserAgentCallBack.h"

enum ETestType
{
	E_TEST_NULL = 0,
	E_TEST_ESTABLISHED,
	E_TEST_CANCEL,
	E_TEST_DECLINE,
	E_TEST_TRANSFER,
	E_TEST_TRANSFER_CALL,
	E_TEST_SCREENED_TRANSFER_CALL
};

enum ETransferResult
{
	E_TR_NULL = 0,
	E_TR_SUCCESS,
	E_TR_ERROR
};

/**
 * @ingroup SipTest
 * @brief �׽�Ʈ�� RTP ���� ���� Ŭ����
 */
class CTestRtpInfo
{
public:
	CTestRtpInfo();

	int	m_iPort;
	Socket	m_hSocket;
	int m_iRecvCount;

	bool Create( int iPort );
	void Close( );
	void Clear( );
};

/**
 * @ingroup SipTest
 * @brief �׽�Ʈ ���� ���� Ŭ����
 */
class CTestInfo
{
public:
	CTestInfo();

	std::string		m_strCallerCallId;
	std::string		m_strCalleeCallId;

	CTestRtpInfo	m_clsCallerRtp;
	CTestRtpInfo	m_clsCalleeRtp;

	CTestRtpInfo	m_clsCallerVideoRtp;
	CTestRtpInfo	m_clsCalleeVideoRtp;

	CSipCallRtp		m_clsCallerPeerRtp;
	CSipCallRtp		m_clsCalleePeerRtp;

	ETestType			m_eTestType;
	bool					m_bResult;
	ETransferResult	m_eTransferResult;
	bool          m_bRtpThreadEnd;

	bool CreateRtp( );
	void CloseRtp( );
	void ClearRtp( );
};

extern CTestInfo gclsTestInfo;

#endif

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

#ifndef _MONITOR_CALLBACK_H_
#define _MONITOR_CALLBACK_H_

#include "MonitorString.h"

/**
 * @brief ���� ����͸� ��û �̺�Ʈ callback �������̽�
 */
class IMonitorCallBack
{
public:
	IMonitorCallBack() : m_iMonitorPort(0), m_bIpv6(false)
	{
	}

	virtual ~IMonitorCallBack(){};

	/**
	 * @brief ���� ����͸� ��û ��� ���� �̺�Ʈ �ڵ鷯
	 * @param pszRequest	��û ��� ���ڿ�
	 * @param strResponse ���� ���ڿ� ���� ����
	 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	virtual bool RecvRequest( const char * pszRequest, CMonitorString & strResponse ) = 0;

	/**
	 * @brief ���� Ŭ���̾�Ʈ�ΰ�?
	 * @param pszIp Ŭ���̾�Ʈ IP �ּ�
	 * @returns ���� Ŭ���̾�Ʈ�̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
	 */
	virtual bool IsMonitorIp( const char * pszIp ) = 0;

	int		m_iMonitorPort;

	bool	m_bIpv6;
};

#endif

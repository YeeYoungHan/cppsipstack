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

#ifndef _SIP_DELETE_QUEUE_H_
#define _SIP_DELETE_QUEUE_H_

#include "SipStackDefine.h"
#include "SipMutex.h"
#include <list>

#ifdef _DEBUG
#define SIP_DELETE_TIME	1
#else
#define SIP_DELETE_TIME	30
#endif

/**
 * @ingroup SipStack
 * @brief 만료된 SIP 메시지를 저장하는 클래스
 */
class CSipDeleteInfo
{
public:
	/** 만료된 SIP 메시지 */
	CSipMessage  * m_pclsMessage;

	/** 삭제전 대기 시간 */
	time_t				 m_iDeleteTime;
};

typedef std::list< CSipDeleteInfo > SIP_DELETE_QUEUE;

/**
 * @ingroup SipStack
 * @brief 만료된 SIP 메시지를 저장한 후, 일정 시간 후에 삭제하는 클래스
 */
class CSipDeleteQueue
{
public:
	CSipDeleteQueue();
	~CSipDeleteQueue();

	void Insert( CSipMessage * pclsMessage );
	void DeleteTimeout( );
	int GetSize();

private:
	SIP_DELETE_QUEUE	m_clsList;
	CSipMutex					m_clsMutex;
};

extern CSipDeleteQueue gclsSipDeleteQueue;

#endif

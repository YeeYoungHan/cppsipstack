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

#define SIP_DELETE_TIME	30

/**
 * @ingroup SipStack
 * @brief 
 */
class CSipDeleteInfo
{
public:
	CSipMessage  * m_pclsMessage;
	time_t				 m_iDeleteTime;
};

typedef std::list< CSipDeleteInfo > SIP_DELETE_QUEUE;

/**
 * @ingroup SipStack
 * @brief 
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

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

#ifndef _SIP_TRANSACTION_LIST_H_
#define _SIP_TRANSACTION_LIST_H_

#include "SipStackDefine.h"
#include "SipMutex.h"

#define MAX_ICT_RESEND_COUNT	11
#define VIA_PREFIX						"z9hG4bK"
#define VIA_PREFIX_LEN				7

class CSipStack;

/**
 * @brief SIP transaction List Å¬·¡½º
 */
class CSipTransactionList
{
public:
	CSipTransactionList();
	virtual ~CSipTransactionList();
	void SetSipStack( CSipStack * pclsSipStack );

	static int m_arrICTReSendTime[MAX_ICT_RESEND_COUNT];
	static bool GetKey( CSipMessage * pclsMessage, std::string & strKey );
	static bool GetKey( CSipMessage * pclsMessage, const char * pszMethod, std::string & strKey );

	virtual bool Insert( CSipMessage * pclsMessage ) = 0;
	virtual void Execute( struct timeval * psttTime ) = 0;
	virtual void DeleteAll( ) = 0;
	virtual int GetSize( ) = 0;
	virtual void GetString( std::string & strBuf ) = 0;

	CSipStack * m_pclsSipStack;
	CSipMutex		m_clsMutex;
};

typedef std::list< CSipMessage * > OSIP_MESSAGE_LIST;

#endif

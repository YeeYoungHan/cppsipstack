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

#ifndef _SIP_IST_LIST_H_
#define _SIP_IST_LIST_H_

#include "SipInviteTransaction.h"
#include "SipTransactionList.h"

/**
 * @ingroup SipStack
 * @brief INVITE SERVER TRANSACTION List Ŭ����
 */
class CSipISTList : public CSipTransactionList
{
public:
	CSipISTList();
	virtual ~CSipISTList();

	virtual bool Insert( CSipMessage * pclsMessage );
	virtual void Execute( struct timeval * psttTime );
	virtual void DeleteAll( );
	virtual int GetSize( );
	virtual void GetString( CMonitorString & strBuf );

private:
	INVITE_TRANSACTION_MAP	m_clsMap;
};

#endif

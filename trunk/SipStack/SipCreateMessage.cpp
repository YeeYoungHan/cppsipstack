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

#include "SipStackDefine.h"
#include "SipCreateMessage.h"
#include "SipMutex.h"
#include "SipTransactionList.h"

static CSipMutex gclsMutex;
static int	giTag;
static int	giBranch;

CSipMessage * SipCreateResponse( CSipStackSetup * pclsSipStackSetup, CSipMessage * pclsRequest, int iStatus )
{
	if( pclsRequest == NULL ) return NULL;

	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse == NULL ) return NULL;

	pclsResponse->m_iStatusCode = iStatus;
	pclsResponse->m_clsTo = pclsRequest->m_clsTo;
	pclsResponse->m_clsFrom = pclsRequest->m_clsFrom;
	pclsResponse->m_clsViaList = pclsRequest->m_clsViaList;
	pclsResponse->m_clsCallId = pclsRequest->m_clsCallId;
	pclsResponse->m_clsCSeq = pclsRequest->m_clsCSeq;

	CSipFrom clsContact;

	clsContact.m_clsUri.m_strProtocol = "sip";
	clsContact.m_clsUri.m_strUser = pclsRequest->m_clsTo.m_clsUri.m_strUser;
	clsContact.m_clsUri.m_strHost = pclsSipStackSetup->m_strLocalIp;
	clsContact.m_clsUri.m_iPort = pclsSipStackSetup->m_iLocalUdpPort;

	pclsResponse->m_clsContactList.push_back( clsContact );

	return pclsResponse;
}

void SipMakeTag( char * pszTag, int iTagSize )
{
	int		iTag;

	gclsMutex.acquire();
	if( giTag <= 0 || giTag > 2000000000 )
	{
		giTag = 1;
	}
	else
	{
		++giTag;
	}

	iTag = giTag;
	gclsMutex.release();

	snprintf( pszTag, iTagSize, "%d", iTag );
}

void SipMakeBranch( char * pszBranch, int iBranchSize )
{
	int		iBranch;

	gclsMutex.acquire();
	if( giBranch <= 0 || giBranch > 2000000000 )
	{
		giBranch = 1;
	}
	else
	{
		++giBranch;
	}

	iBranch = giBranch;
	gclsMutex.release();

	snprintf( pszBranch, iBranchSize, "%s-WCSS-%d", VIA_PREFIX, iBranch );
}

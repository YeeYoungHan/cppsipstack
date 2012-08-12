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

#include "SipParserDefine.h"
#include "SipUtility.h"
#include "SipMutex.h"
#include <string>

static CSipMutex gclsMutex;
static int	giTag;
static int	giBranch;
static int	giCallId;

#ifdef WIN32
#include <sys/timeb.h>

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  struct _timeb timebuffer;

  _ftime( &timebuffer );
  tv->tv_sec = (long) timebuffer.time;
  tv->tv_usec = timebuffer.millitm * 1000;

  return 0;
}

#endif

/**
 * @brief SIP From/To tag �� ����� ���ڿ��� �ۼ��Ѵ�.
 * @param pszTag		SIP From/To tag �� ����� ���ڿ��� ������ ����
 * @param iTagSize	pszTag ������ ũ��
 */
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

/**
 * @brief SIP Via branch �� ����� ���ڿ��� �ۼ��Ѵ�.
 * @param pszBranch		SIP Via branch �� ����� ���ڿ��� ������ ����
 * @param iBranchSize	pszBranch ������ ũ��
 */
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

/**
 * @brief SIP Call-ID �� ����� name ���ڿ��� �ۼ��Ѵ�.
 * @param pszCallId		SIP Call-ID �� ����� name ���ڿ��� ������ ����
 * @param iCallIdSize	pszCallId ������ ũ��
 */
void SipMakeCallIdName( char * pszCallId, int iCallIdSize )
{
	int		iCallId;
	struct timeval sttTime;

	gettimeofday( &sttTime, NULL );

	gclsMutex.acquire();
	if( giCallId <= 0 || giCallId > 2000000000 )
	{
		giCallId = 1;
	}
	else
	{
		++giCallId;
	}

	iCallId = giCallId;
	gclsMutex.release();

	snprintf( pszCallId, iCallIdSize, "WCSS-%d-%d-%d", iCallId, (int)sttTime.tv_sec, (int)sttTime.tv_usec );
}

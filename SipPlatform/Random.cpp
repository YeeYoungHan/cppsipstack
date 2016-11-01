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

#include "Random.h"

#define _CRT_RAND_S
#include <stdlib.h>

CRandom::CRandom()
{
#ifndef WIN32
	srandom( time(NULL) );
#endif
}

CRandom::~CRandom()
{
}

/**
 * @ingroup SipPlatform
 * @brief random 정수를 리턴한다.
 * @returns random 정수를 리턴한다.
 */
int CRandom::Get()
{
#ifdef WIN32
	unsigned int iRand;

	rand_s( &iRand );
#else
	long int iRand;

	iRand = random();
#endif

	if( iRand > 2000000000 )
	{
		iRand = iRand % 2000000000;
	}

	return iRand;
}

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
#include "Log.h"
#include <signal.h>
#include <string>

bool gbStop = false;

/**
 * @ingroup ServerPlatform
 * @brief signal function
 * @param sig 신호 번호
 */
void LastMethod( int sig )
{
	char	szText[21];

	szText[0] = '\0';
	switch( sig )
	{
	case SIGINT:
		snprintf( szText, sizeof(szText), "SIGINT" );
		break;
	case SIGSEGV:
		snprintf( szText, sizeof(szText), "SIGSEGV" );
		break;
	case SIGTERM:
		snprintf( szText, sizeof(szText), "SIGTERM" );
		break;
#ifndef WIN32
	case SIGQUIT:
		snprintf( szText, sizeof(szText), "SIGQUIT" );
		break;
#endif
	case SIGABRT:
		snprintf( szText, sizeof(szText), "SIGABRT" );
		break;
	}

	CLog::Print( LOG_ERROR, "signal%s%s(%d) is received. terminated", strlen(szText) > 0 ? "-" : "", szText, sig );

	gbStop = true;
}

/**
 * @ingroup ServerPlatform
 * @brief 서버를 위한 signal 함수를 호출한다.
 */
void ServerSignal()
{
	signal( SIGINT, LastMethod );
	signal( SIGTERM, LastMethod );
	signal( SIGABRT, LastMethod );
#ifndef WIN32
	signal( SIGKILL, LastMethod );
	signal( SIGQUIT, LastMethod );
	signal( SIGPIPE, SIG_IGN );
#endif
}
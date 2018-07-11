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

#include "SipPlatformDefine.h"
#include <stdio.h>
#include <string.h>

static char * garrFunc[] = {
"asctime(",
"basename(",
"catgets(",
"crypt(",
"ctime(",
"dbm_clearerr(",
"dbm_close(",
"dbm_delete(",
"dbm_error(",
"dbm_fetch(",
"dbm_firstkey(",
"dbm_nextkey(",
"dbm_open(",
"dbm_store(",
"dirname(",
"dlerror(",
"drand48(",
"encrypt(",
"endgrent(",
"endpwent(",
"endutxent(",
"ftw(",
"getdate(",
"getenv(",
"getgrent(",
"getgrgid(",
"getgrnam(",
"gethostent(",
"getlogin(",
"getnetbyaddr(",
"getnetbyname(",
"getnetent(",
"getopt(",
"getprotobyname(",
"getprotobynumber(",
"getprotoent(",
"getpwent(",
"getpwnam(",
"getpwuid(",
"getservbyname(",
"getservbyport(",
"getservent(",
"getutxent(",
"getutxid(",
"getutxline(",
"gmtime(",
"hcreate(",
"hdestroy(",
"hsearch(",
"inet_ntoa(",
"l64a(",
"lgamma(",
"lgammaf(",
"lgammal(",
"localeconv(",
"localtime(",
"lrand48(",
"mblen(",
"mbtowc(",
"mrand48(",
"nftw(",
"nl_langinfo(",
"ptsname(",
"putenv(",
"pututxline(",
"rand(",
"readdir(",
"setenv(",
"setgrent(",
"setkey(",
"setlocale(",
"setpwent(",
"setutxent(",
"strerror(",
"strsignal(",
"strtok(",
"system(",
"ttyname(",
"unsetenv(",
"wctomb(",
 NULL
};

bool CheckLine( const char * pszLine )
{
	char szFunc[255];

	for( int i = 0; garrFunc[i]; ++i )
	{
		snprintf( szFunc, sizeof(szFunc), " %s", garrFunc[i] );

		if( strstr( pszLine, szFunc ) ) return false;

		snprintf( szFunc, sizeof(szFunc), "\t%s", garrFunc[i] );

		if( strstr( pszLine, szFunc ) ) return false;
	}

	return true;
}

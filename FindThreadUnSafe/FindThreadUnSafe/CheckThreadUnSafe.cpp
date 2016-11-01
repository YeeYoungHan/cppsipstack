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

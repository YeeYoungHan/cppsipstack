#include "SipParserDefine.h"
#include "Log.h"
#include <signal.h>
#include <string>

bool gbStop = false;

/**
 * @ingroup KSipServer
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

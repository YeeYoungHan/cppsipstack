#include "SipRegisterThread.h"
#include <time.h>

#ifdef WIN32
DWORD WINAPI SipRegisterThread( LPVOID lpParameter )
#else
void * SipRegisterThread( void * lpParameter )
#endif
{
	CSipUserAgent * pclsSipUserAgent = (CSipUserAgent *)lpParameter;
	SIP_SERVER_INFO_LIST::iterator itList;
	time_t	iTime;

	while( pclsSipUserAgent->m_clsSipStack.m_bStopEvent == false )
	{
		time( &iTime );

		for( itList = pclsSipUserAgent->m_clsRegisterList.begin(); itList != pclsSipUserAgent->m_clsRegisterList.end(); ++itList )
		{
			if( itList->m_bLogin == false )
			{

			}
			else
			{
				if( ( iTime - itList->m_iLoginTime ) > ( itList->m_iLoginTimeout / 2 ) )
				{
					
				}
			}
		}

		sleep(1);
	}

	return 0;
}

bool StartSipRegisterThread( CSipUserAgent * pclsSipUserAgent )
{
	return StartThread( "SipRegisterThread", SipRegisterThread, pclsSipUserAgent );
}

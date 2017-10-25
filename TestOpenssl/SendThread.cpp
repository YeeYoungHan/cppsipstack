#include "TestOpenssl.h"

THREAD_API SendThread( LPVOID lpParameter )
{
	SSL * psttSsl = (SSL *)lpParameter;
	char szPacket[8192];

	memset( szPacket, 0, sizeof(szPacket) );

	while( 1 )
	{
		SSLSend( psttSsl, szPacket, sizeof(szPacket) );
	}

	return 0;
}

bool StartSendThread( SSL * psttSsl )
{
	return StartThread( "SendThread", SendThread, psttSsl );
}

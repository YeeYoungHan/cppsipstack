#include "TestServerPlatform.h"
#include "ServerService.h"

void ErrorFunction()
{
	char * pszTemp;

	snprintf( pszTemp, 1024, "ABCDEFGHIJKLMNOPQRSTU" );
}

bool TestServerSignal()
{
	ServerSignal();

	ErrorFunction();

	return true;
}

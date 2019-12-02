#include "TestServerPlatform.h"
#include "ServerService.h"

void ErrorFunction()
{
	char * pszTemp = NULL;

	snprintf( pszTemp, 1024, "ABCDEFGHIJKLMNOPQRSTU" );
}

bool TestServerSignal()
{
	ServerSignal();

	ErrorFunction();

	return true;
}

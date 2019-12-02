#include "ServerService.h"

void ErrorFunction()
{
	int n = 0;

	printf( "%d", 100 / n );
}

bool TestServerSignal()
{
	ServerSignal();

	ErrorFunction();

	return true;
}

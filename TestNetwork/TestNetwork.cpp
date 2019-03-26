#include "TestNetwork.h"

int main( int argc, char * argv[] )
{
	InitNetwork();

	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK );

	TestTcpSendBlocking();

	return 0;
}

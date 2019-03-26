#include "TestNetwork.h"

int main( int argc, char * argv[] )
{
	InitNetwork();

	TestTcpSendBlocking();

	return 0;
}

#ifndef _TEST_NETWORK_H_
#define _TEST_NETWORK_H_

#include "SipPlatformDefine.h"
#include "SipTcp.h"
#include "ServerUtility.h"
#include "Log.h"

#ifndef WIN32
#include <sys/select.h>
#endif

bool TestTcpSendBlocking( );

#endif

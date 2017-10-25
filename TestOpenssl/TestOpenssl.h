#ifndef _TEST_OPENSSL_H_
#define _TEST_OPENSSL_H_

#define USE_TLS

#include "SipPlatformDefine.h"
#include "ServerUtility.h"
#include "TlsFunction.h"

bool StartClientThread();
bool StartSendThread( SSL * psttSsl );

#endif

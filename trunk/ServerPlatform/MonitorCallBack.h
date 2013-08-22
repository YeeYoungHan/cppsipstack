#ifndef _MONITOR_CALLBACK_H_
#define _MONITOR_CALLBACK_H_

#include <string>

class IMonitorCallBack
{
public:
	virtual ~IMonitorCallBack(){};

	virtual bool RecvRequest( const char * pszRequest, std::string & strResponse ) = 0;

	virtual bool IsMonitorIp( const char * pszIp ) = 0;

	int m_iMonitorPort;
};

#endif

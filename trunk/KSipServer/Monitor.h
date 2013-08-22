#pragma once

#include "MonitorCallBack.h"

class CMonitor : public IMonitorCallBack
{
public:
	CMonitor();
	~CMonitor();

	virtual bool RecvRequest( const char * pszRequest, std::string & strResponse );

	virtual bool IsMonitorIp( const char * pszIp );
};

extern CMonitor gclsMonitor;

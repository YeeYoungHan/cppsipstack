#ifndef _STATS_SIP_METHOD_H_
#define _STATS_SIP_METHOD_H_

#include "SipParserDefine.h"
#include "SipMessage.h"
#include <map>

typedef std::map< std::string, uint64_t > STATS_SIP_METHOD_MAP;

class CStatsSipMethod
{
public:
	CStatsSipMethod();
	~CStatsSipMethod();

	void AddSipMessage( CSipMessage * pclsMessage );

private:
	STATS_SIP_METHOD_MAP m_clsMap;
};

extern CStatsSipMethod gclsStatsSipMethod;

#endif

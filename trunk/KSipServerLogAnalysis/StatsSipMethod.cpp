#include "StatsSipMethod.h"

CStatsSipMethod gclsStatsSipMethod;

CStatsSipMethod::CStatsSipMethod()
{
}

CStatsSipMethod::~CStatsSipMethod()
{
}

void CStatsSipMethod::AddSipMessage( CSipMessage * pclsMessage )
{
	STATS_SIP_METHOD_MAP::iterator	itMap;

	itMap = m_clsMap.find( pclsMessage->m_strSipMethod );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( STATS_SIP_METHOD_MAP::value_type( pclsMessage->m_strSipMethod, 1 ) );
	}
	else
	{
		++itMap->second;
	}
}

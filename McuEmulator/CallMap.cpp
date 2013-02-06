#include "CallMap.h"

CCallMap::CCallMap()
{
}

CCallMap::~CCallMap()
{
}

bool CCallMap::Insert( CSipMessage * pclsMessage )
{
	std::string	strKey;
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	GetKey( pclsMessage, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallMap::Select( CSipMessage * pclsMessage, CCallInfo & clsCallInfo )
{
	std::string	strKey;
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	GetKey( pclsMessage, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		clsCallInfo = itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallMap::Delete( CSipMessage * pclsMessage )
{
	std::string	strKey;
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	GetKey( pclsMessage, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

void CCallMap::GetKey( CSipMessage * pclsMessage, std::string & strKey )
{
	pclsMessage->m_clsCallId.ToString( strKey );
}

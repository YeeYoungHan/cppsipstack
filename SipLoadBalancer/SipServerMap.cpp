#include "SipServerMap.h"

CSipServerMap gclsSipServerMap;

CSipServerMap::CSipServerMap() : m_iIndex(0)
{
}

CSipServerMap::~CSipServerMap()
{
}

bool CSipServerMap::Insert( const char * pszIp )
{
	CSipServerInfo clsInfo;

	clsInfo.m_strIp = pszIp;
	
	m_clsMutex.acquire();
	m_clsList.push_back( clsInfo );
	m_clsMutex.release();

	return true;
}

bool CSipServerMap::SelectNext( CSipServerInfo & clsInfo )
{
	bool bRes = false;

	m_clsMutex.acquire();
	int iCount = m_clsList.size();
	if( iCount > 0 )
	{
		if( m_iIndex >= iCount )
		{
			m_iIndex = 0;
		}

		clsInfo = m_clsList[m_iIndex];
		++m_iIndex;

		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

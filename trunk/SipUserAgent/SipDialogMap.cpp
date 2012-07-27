#include "SipParserDefine.h"
#include "SipDialogMap.h"
#include "SipUtility.h"

CSipDialogMap::CSipDialogMap() : m_iLastKey(0)
{
}

CSipDialogMap::~CSipDialogMap()
{
}

bool CSipDialogMap::Insert( CSipDialog & clsDialog, int & iKey )
{
	if( clsDialog.m_strFromId.empty() || clsDialog.m_strToId.empty() ) return false;
	
	SIP_DIALOG_KEY_MAP::iterator	itKey;
	char	szTag[SIP_TAG_MAX_SIZE], szBranch[SIP_BRANCH_MAX_SIZE], szCallIdName[SIP_CALL_ID_NAME_MAX_SIZE];

	SipMakeTag( szTag, sizeof(szTag) );
	SipMakeBranch( szBranch, sizeof(szBranch) );
	SipMakeCallIdName( szCallIdName, sizeof(szCallIdName) );

	clsDialog.m_strFromTag = szTag;
	clsDialog.m_strViaBranch = szBranch;

	m_clsMutex.acquire();
	while( 1 )
	{
		++m_iLastKey;
		if( m_iLastKey > 2000000000 ) m_iLastKey = 1;

		itKey = m_clsKeyMap.find( m_iLastKey );
		if( itKey == m_clsKeyMap.end() )
		{
			break;
		}
	}

	m_clsMutex.release();

	return true;
}

bool CSipDialogMap::Delete( int iKey )
{
	m_clsMutex.acquire();

	m_clsMutex.release();

	return true;
}

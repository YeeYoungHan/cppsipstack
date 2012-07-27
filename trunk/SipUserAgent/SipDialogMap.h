#ifndef _SIP_DIALOG_MAP_H_
#define _SIP_DIALOG_MAP_H_

#include "SipDialog.h"
#include "SipMutex.h"
#include <map>

typedef std::map< std::string, CSipDialog > SIP_DIALOG_MAP;
typedef std::map< int, std::string > SIP_DIALOG_KEY_MAP;

class CSipDialogMap
{
public:
	CSipDialogMap();
	~CSipDialogMap();

	bool Insert( CSipDialog & clsDialog, int & iKey );
	bool Delete( int iKey );

private:
	int		m_iLastKey;
	SIP_DIALOG_MAP			m_clsMap;
	SIP_DIALOG_KEY_MAP	m_clsKeyMap;
	CSipMutex						m_clsMutex;
};

#endif

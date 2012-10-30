

#ifndef _SIP_BUFFER_H_
#define _SIP_BUFFER_H_

#define SIP_MAX_BUF_SIZE	8192

#include "SipUdp.h"

/** SIP TCP ���۸� �����ϴ� Ŭ���� */
class CSipBuffer
{
public:
	CSipBuffer();
	~CSipBuffer();

	bool AddBuf( const char * pszBuf, int iLen );
	bool GetSipMessage( char ** ppszBuf, int * piLen );
	bool ShiftBuf( int iLen );
	void Clear();

private:
	char		* m_pszBuf;
	int			m_iBufLen;
};

#endif

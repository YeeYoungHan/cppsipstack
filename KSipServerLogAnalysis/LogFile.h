/*----------------------------------------------------------------------------
 * LogFile.h
 *----------------------------------------------------------------------------
 * programmer : Yee Young Han ( websearch@naver.com )
 *            : http://blog.naver.com/websearch/
 *----------------------------------------------------------------------------
 * start date : 2011/11/14
 *----------------------------------------------------------------------------
 * comment    : SIP 로그 파일 읽기 클래스
 *----------------------------------------------------------------------------
 */

#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include <string>

/**
 * @brief SIP 로그의 발신/수신 정보를 저장하는 클래스
 */
class CLogHeader
{
public:
	struct timeval	m_sttTime;
	char						m_szIp[16];
	int							m_iPort;
	bool						m_bSend;
	std::string			m_strCallId;
	int							m_iStatusCode;
	bool						m_bBye;

	void Clear();
};

/**
 * @brief SIP 로그 파일 읽기 클래스
 */
class CLogFile
{
public:
	CLogFile(void);
	~CLogFile(void);

	bool Open( const char * pszFileName );
	bool ReadSip( CLogHeader * psttLogHeader, char * pszBuf, int iBufSize );
	void Close( );

private:
	FILE	* m_sttFd;

	bool SaveLogHeader( const char * pszTemp, CLogHeader * psttLogHeader, char * pszBuf, int iBufSize, int & iPos );
	void CheckLineEnd( char * pszTemp );
};

#endif

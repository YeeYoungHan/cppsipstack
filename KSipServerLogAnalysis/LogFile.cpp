/*----------------------------------------------------------------------------
 * LogFile.cpp
 *----------------------------------------------------------------------------
 * programmer : Yee Young Han ( websearch@naver.com )
 *            : http://blog.naver.com/websearch/
 *----------------------------------------------------------------------------
 * start date : 2011/11/14
 *----------------------------------------------------------------------------
 * comment    : SIP 로그 파일 읽기 클래스
 *----------------------------------------------------------------------------
 */

#include "SipParserDefine.h"
#include "LogFile.h"

void CLogHeader::Clear()
{
	memset( this, 0, sizeof(CLogHeader) );
}

CLogFile::CLogFile(void) : m_sttFd(NULL)
{
}

CLogFile::~CLogFile(void)
{
	Close();
}

bool CLogFile::Open( const char * pszFileName )
{
	if( m_sttFd ) return false;

	m_sttFd = fopen( pszFileName, "r" );
	if( m_sttFd == NULL ) return false;

	return true;
}

bool CLogFile::ReadSip( CLogHeader * psttLogHeader, char * pszBuf, int iBufSize )
{
	if( m_sttFd == NULL ) return false;

	char	szTemp[2048];
	bool	bStart = false;
	int		iPos = 0, iLen;

	memset( szTemp, 0, sizeof(szTemp) );
	while( fgets( szTemp, sizeof(szTemp), m_sttFd ) )
	{
		if( bStart )
		{
			if( szTemp[0] == ']' )
			{
				return true;
			}
			else
			{
				CheckLineEnd( szTemp );
				iPos += snprintf( pszBuf + iPos, iBufSize - iPos, "%s", szTemp );
				if( iPos >= iBufSize ) return false;

				if( !strncmp( szTemp, "Call-ID: ", 9 ) )
				{
					iLen = strlen( szTemp + 9 );
					if( iLen > 2 )
					{
						psttLogHeader->m_strCallId.append( szTemp + 9, iLen - 2 );
					}
				}
				else if( !strncmp( szTemp, "CSeq: ", 6 ) )
				{
					std::string strTemp;

					strTemp.append( szTemp + 6, iLen - 2 );
					sscanf( strTemp.c_str(), "%*d %s", szTemp );
					if( !strcmp( szTemp, "BYE" ) )
					{
						psttLogHeader->m_bBye = true;
					}
				}
			}
		}
		else if( strstr( szTemp, "UdpSend" ) )
		{
			psttLogHeader->Clear();
			psttLogHeader->m_bSend = true;
			psttLogHeader->m_strCallId.clear();

			CheckLineEnd( szTemp );
			if( SaveLogHeader( szTemp, psttLogHeader, pszBuf, iBufSize, iPos ) == false ) return false;
			bStart = true;
		}
		else if( strstr( szTemp, "UdpRecv" ) )
		{
			psttLogHeader->Clear();
			psttLogHeader->m_bSend = false;
			psttLogHeader->m_strCallId.clear();

			CheckLineEnd( szTemp );
			if( SaveLogHeader( szTemp, psttLogHeader, pszBuf, iBufSize, iPos ) == false ) return false;
			bStart = true;
		}
		
		memset( szTemp, 0, sizeof(szTemp) );
	}

	return false;
}

void CLogFile::Close( )
{
	if( m_sttFd )
	{
		fclose( m_sttFd );
		m_sttFd = NULL;
	}
}

bool CLogFile::SaveLogHeader( const char * pszTemp, CLogHeader * psttLogHeader, char * pszBuf, int iBufSize, int & iPos )
{
	char	cType = 0;
	char	szData[255];
	int		iDataPos = 0;

	memset( szData, 0, sizeof(szData) );

	for( int i = 0; pszTemp[i]; ++i )
	{
		if( pszTemp[i] == '[' || pszTemp[i] == '(' )
		{
			++cType;

			if( cType == 5 )
			{
				iPos = snprintf( pszBuf, iBufSize, "%s", pszTemp + i + 1 );
				if( !strncmp( pszBuf, "SIP/2.0 ", 8 ) )
				{
					psttLogHeader->m_iStatusCode = atoi( pszBuf + 8 );
				}
				return true;
			}
		}
		else if( pszTemp[i] == ']' || pszTemp[i] == ')' )
		{
			if( cType == 1 )
			{
				int iHour, iMin, iSec, iMSec;

				if( sscanf( szData, "%d:%d:%d.%d", &iHour, &iMin, &iSec, &iMSec ) != 4 ) return false;

				psttLogHeader->m_sttTime.tv_sec = iHour * 3600 + iMin * 60 + iSec;
				psttLogHeader->m_sttTime.tv_usec = iMSec;
			}
			else if( cType == 4 )
			{
				int iIp1, iIp2, iIp3, iIp4, iPort;

				if( sscanf( szData, "%d.%d.%d.%d:%d", &iIp1, &iIp2, &iIp3, &iIp4, &iPort ) != 5 ) return false;

				snprintf( psttLogHeader->m_szIp, sizeof(psttLogHeader->m_szIp), "%d.%d.%d.%d", iIp1, iIp2, iIp3, iIp4 );
				psttLogHeader->m_iPort = iPort;
			}

			memset( szData, 0, sizeof(szData) );
			iDataPos = 0;
		}
		else if( cType == 1 || cType == 4 )
		{
			szData[iDataPos++] = pszTemp[i];
		}
	}

	return true;
}

void CLogFile::CheckLineEnd( char * pszTemp )
{
	int iLen = strlen( pszTemp );

	if( iLen >= 2 )
	{
		if( pszTemp[iLen-2] == '\r' )
		{
			if( pszTemp[iLen-1] != '\n' )
			{
				pszTemp[iLen-1] = '\n';
			}

			return;
		}
	}

	if( iLen >= 1 )
	{
		if( pszTemp[iLen-1] == '\n' )
		{
			pszTemp[iLen-1] = '\r';
			pszTemp[iLen] = '\n';
		}
	}
}

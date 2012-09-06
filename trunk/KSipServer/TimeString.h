/*----------------------------------------------------------------------------
 * TimeString.h
 *----------------------------------------------------------------------------
 * programmer : Yee Young Han ( websearch@naver.com )
 *            : http://blog.naver.com/websearch/
 *----------------------------------------------------------------------------
 * start date : 2011/3/29
 *----------------------------------------------------------------------------
 * comment    : 시간 관련 문자열 생성 메소드 선언
 *----------------------------------------------------------------------------
 */

#ifndef _TIME_STRING_H_
#define _TIME_STRING_H_

#include <time.h>

void LocalTime( time_t iTime, struct tm & sttTm );

void GetDateTimeString( time_t iTime, char * pszTime, int iTimeSize );
void GetDateTimeString( char * pszTime, int iTimeSize );

void GetDateString( time_t iTime, char * pszDate, int iDateSize );
void GetDateString( char * pszDate, int iDateSize );

void GetTimeString( time_t iTime, char * pszTime, int iTimeSize );
void GetTimeString( char * pszTime, int iTimeSize );

#endif

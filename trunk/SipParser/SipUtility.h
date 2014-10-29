/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef _SIP_UTILITY_H_
#define _SIP_UTILITY_H_

#include <string>

void SipSetSystemId( const char * pszId );
void SipMakeTag( char * pszTag, int iTagSize );
void SipMakeBranch( char * pszBranch, int iBranchSize );
void SipMakeCallIdName( char * pszCallId, int iCallIdSize );
bool SipMakePrintString( const unsigned char * pszInput, int iInputSize, char * pszOutput, int iOutputSize );
void SipMd5String21( char * string, char result[22] );

void SipIpv6Parse( std::string & strHost );
int SipIpv6Print( std::string & strHost, char * pszText, int iTextSize, int iLen );

#endif

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

#ifndef _BASE64_H_
#define _BASE64_H_

#include <string>

int GetBase64EncodeLength( int iLength );
int GetBase64DecodeLength( int iLength );
int Base64Encode( const char * pszInput, int iInputLength, char * pszOutput, int iOutputLength );
int Base64Decode( const char * pszInput, int iInputLength, char * pszOutput, int iOutputLength );

bool Base64Encode( const char * pszInput, int iInputLength, std::string & strOutput );

#endif

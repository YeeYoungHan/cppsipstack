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

#include "StringUtility.h"
#include <string.h>
#include <stdio.h>

static bool TestLeftTrim( const char * pszInput, const char * pszOutput )
{
	std::string strText = pszInput;

	LeftTrimString( strText );
	
	if( strcmp( pszOutput, strText.c_str() ) )
	{
		printf( "%s input(%s) output(%s) => result(%s)", __FUNCTION__, pszInput, pszOutput, strText.c_str() );
		return false;
	}

	return true;
}

static bool TestRightTrim( const char * pszInput, const char * pszOutput )
{
	std::string strText = pszInput;

	RightTrimString( strText );
	
	if( strcmp( pszOutput, strText.c_str() ) )
	{
		printf( "%s input(%s) output(%s) => result(%s)", __FUNCTION__, pszInput, pszOutput, strText.c_str() );
		return false;
	}

	return true;
}

static bool TestTrim( const char * pszInput, const char * pszOutput )
{
	std::string strText = pszInput;

	TrimString( strText );
	
	if( strcmp( pszOutput, strText.c_str() ) )
	{
		printf( "%s input(%s) output(%s) => result(%s)", __FUNCTION__, pszInput, pszOutput, strText.c_str() );
		return false;
	}

	return true;
}

bool TestStringUtility()
{
	if( TestLeftTrim( "abcd", "abcd" ) == false ) return false;
	if( TestLeftTrim( " abcd", "abcd" ) == false ) return false;
	if( TestLeftTrim( "  abcd", "abcd" ) == false ) return false;
	if( TestLeftTrim( " \t abcd", "abcd" ) == false ) return false;
	if( TestLeftTrim( " abcd  ", "abcd  " ) == false ) return false;
	if( TestLeftTrim( " ab  cd", "ab  cd" ) == false ) return false;
	if( TestLeftTrim( "    ", "" ) == false ) return false;
	if( TestLeftTrim( "", "" ) == false ) return false;

	if( TestRightTrim( "abcd", "abcd" ) == false ) return false;
	if( TestRightTrim( "abcd ", "abcd" ) == false ) return false;
	if( TestRightTrim( "abcd  ", "abcd" ) == false ) return false;
	if( TestRightTrim( "abcd \t ", "abcd" ) == false ) return false;
	if( TestRightTrim( "  abcd  ", "  abcd" ) == false ) return false;
	if( TestRightTrim( "  ab  cd  ", "  ab  cd" ) == false ) return false;
	if( TestRightTrim( "   ", "" ) == false ) return false;
	if( TestRightTrim( "", "" ) == false ) return false;

	if( TestTrim( "abcd", "abcd" ) == false ) return false;
	if( TestTrim( " abcd", "abcd" ) == false ) return false;
	if( TestTrim( "  abcd", "abcd" ) == false ) return false;
	if( TestTrim( " \t abcd", "abcd" ) == false ) return false;
	if( TestTrim( "abcd ", "abcd" ) == false ) return false;
	if( TestTrim( "abcd  ", "abcd" ) == false ) return false;
	if( TestTrim( "abcd \t ", "abcd" ) == false ) return false;
	if( TestTrim( " abcd ", "abcd" ) == false ) return false;
	if( TestTrim( "  abcd  ", "abcd" ) == false ) return false;
	if( TestTrim( "  ", "" ) == false ) return false;
	if( TestTrim( "", "" ) == false ) return false;

	STRING_LIST clsList;
	STRING_LIST::iterator	itList;

	SplitString( "a/b", clsList, '/' );

	if( clsList.size() != 2 ) return false;
	itList = clsList.begin();
	if( strcmp( itList->c_str(), "a" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "b" ) ) return false;

	SplitString( "/a/b", clsList, '/' );

	if( clsList.size() != 2 ) return false;
	itList = clsList.begin();
	if( strcmp( itList->c_str(), "a" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "b" ) ) return false;

	SplitString( "a/b/", clsList, '/' );

	if( clsList.size() != 2 ) return false;
	itList = clsList.begin();
	if( strcmp( itList->c_str(), "a" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "b" ) ) return false;

	SplitString( "a//b", clsList, '/' );

	if( clsList.size() != 2 ) return false;
	itList = clsList.begin();
	if( strcmp( itList->c_str(), "a" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "b" ) ) return false;

	SplitString( "abc/d/ef/ghij/klmnop/qr/s/tu", clsList, '/' );

	if( clsList.size() != 8 ) return false;
	itList = clsList.begin();
	if( strcmp( itList->c_str(), "abc" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "d" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "ef" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "ghij" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "klmnop" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "qr" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "s" ) ) return false;
	++itList;
	if( strcmp( itList->c_str(), "tu" ) ) return false;

	return true;
}

#include "XmlElement.h"
#include "Directory.h"
#include "FileUtility.h"
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

bool CheckUnicodeProject( const char * pszFileName )
{
	CXmlElement clsXml;

	if( clsXml.ParseFile( pszFileName ) == false ) 
	{
		printf( "clsXml.ParseFile( %s ) error\n", pszFileName );
		return false;
	}

	CXmlElement * pclsConfigures = clsXml.SelectElement( "Configurations" );
	if( pclsConfigures == NULL )
	{
		printf( "file(%s) - Configurations is not found\n", pszFileName );
		return false;
	}

	XML_ELEMENT_LIST clsConfigureList;
	XML_ELEMENT_LIST::iterator itCL;

	pclsConfigures->SelectElementList( "Configuration", clsConfigureList );

	for( itCL = clsConfigureList.begin(); itCL != clsConfigureList.end(); ++itCL )
	{
		int iValue = 0;

		itCL->SelectAttribute( "CharacterSet", iValue );

		if( iValue == 1 )
		{
			printf( "file(%s) is unicode project\n", pszFileName );
			return false;
		}
	}

	return true;
}

void SearchProjectList( const char * pszFolder, STRING_LIST & clsProjectList )
{
	FILE_LIST clsFileList;
	FILE_LIST::iterator itFL;

	CDirectory::List( pszFolder, clsFileList );

	for( itFL = clsFileList.begin(); itFL != clsFileList.end(); ++itFL )
	{
		std::string strFileName = pszFolder;
		CDirectory::AppendName( strFileName, itFL->c_str() );

		if( CDirectory::IsDirectory( strFileName.c_str() ) )
		{
			SearchProjectList( strFileName.c_str(), clsProjectList );
		}
		else
		{
			std::string strExt;

			GetFileExt( itFL->c_str(), strExt );

			if( !strcmp( strExt.c_str(), "vcproj" ) )
			{
				clsProjectList.push_back( strFileName );
			}
		}
	}
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {directory full path}\n", argv[0] );
		return 0;
	}

	STRING_LIST clsProjectList;
	STRING_LIST::iterator itPL;

	SearchProjectList( argv[1], clsProjectList );

	for( itPL = clsProjectList.begin(); itPL != clsProjectList.end(); ++itPL )
	{
		CheckUnicodeProject( itPL->c_str() );
	}

	return 0;
}

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

#include "ServerServicePrivate.h"
#include "ServerUtility.h"
#include "SipUdp.h"

static std::string gstrConfigFileName;
CServerService gclsService;
ServerFunc gpServerFunc;

/**
 * @ingroup ServerPlatform
 * @brief 서버 서비스 main function
 * @param argc 
 * @param argv 
 * @returns 정상 종료하면 0 을 리턴하고 오류가 발생하면 -1 를 리턴한다.
 */
int ServerMain( int argc, char * argv[], CServerService & clsService, ServerFunc pFunc )
{
	gclsService = clsService;
	gpServerFunc = pFunc;

#ifdef WIN32
	if( argc == 1 )
	{
		gstrConfigFileName = GetConfigFileName();
		if( IsExistFile( gstrConfigFileName.c_str() ) == false )
		{
			printf( "setup file(%s) is not exist", gstrConfigFileName.c_str() );
			return -1;
		}

		ServiceStart();
		return 0;
	}
#endif
	 
	if( argc != 2 )
	{
		printf( "[Usage] %s {config filename}\n", argv[0] );
		return -1;
	}

	gstrConfigFileName = argv[1];
	if( !strcmp( gstrConfigFileName.c_str(), "-h" ) || !strcmp( gstrConfigFileName.c_str(), "-v" ) )
	{
		printf( "%s version-%s ( build %s %s )\n", argv[0], gclsService.m_strVersion.c_str(), __DATE__, __TIME__ );
		printf( "[Usage] %s {config filename}\n", argv[0] );
#ifdef WIN32
		printf( "        %s -i : install service\n", argv[0] );
		printf( "        %s -u : uninstall service\n", argv[0] );
#endif
		return 0;
	}
#ifdef WIN32
	if( !strcmp( gstrConfigFileName.c_str(), "-i" ) )
	{
		InstallService();
		return 0;
	}
	else if( !strcmp( gstrConfigFileName.c_str(), "-u" ) )
	{
		InitNetwork();
		UninstallService();
		return 0;
	}
#endif

	gpServerFunc( );

	return 0;
}

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
int ServerMain( int argc, char * argv[], ServerFunc pFunc )
{
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

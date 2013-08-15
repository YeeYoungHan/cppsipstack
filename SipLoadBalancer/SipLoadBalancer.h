#ifndef _SIP_LOAD_BALANCER_H_
#define _SIP_LOAD_BALANCER_H_

// MS 윈도우 서비스 선언
#define SERVICE_NAME								"KSipServer"		// 서비스 이름
#define SERVICE_DISPLAY_NAME				"KSipServer"
#define SERVICE_DESCRIPTION_STRING	"SIP Server - made by Yee Young Han"

#define CONFIG_FILENAME						"SipServer.xml"

extern bool gbStop;

#ifdef WIN32
// Service.cpp
void ServiceStart();
char * GetConfigFileName();

// InstallService.cpp
bool InstallService( );
bool UninstallService( );
#endif

#endif

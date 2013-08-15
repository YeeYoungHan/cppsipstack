#ifndef _SIP_LOAD_BALANCER_H_
#define _SIP_LOAD_BALANCER_H_

// MS ������ ���� ����
#define SERVICE_NAME								"KSipServer"		// ���� �̸�
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

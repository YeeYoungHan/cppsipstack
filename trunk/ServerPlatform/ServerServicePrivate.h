#ifndef _SERVER_SERVICE_PRIVATE_H_
#define _SERVER_SERVICE_PRIVATE_H_

#include "ServerService.h"

// InstallService.h
bool InstallService( );
bool UninstallService( );

// ServerService.h
void ServiceStart();
char * GetConfigFileName();

// ServerSignal.h
extern bool gbStop;

void LastMethod( int sig );

// ServerMain.cpp
extern CServerService gclsService;
extern ServerFunc gpServerFunc;

#endif

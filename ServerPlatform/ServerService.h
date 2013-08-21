#ifndef _SERVER_SERVICE_H_
#define _SERVER_SERVICE_H_

#include <string>

class CServerService
{
public:
	std::string m_strName;
	std::string m_strDisplayName;
	std::string m_strDescription;
	std::string m_strVersion;
	std::string m_strConfigFileName;
};

typedef void (*ServerFunc)();

int ServerMain( int argc, char * argv[], ServerFunc pFunc );

#endif

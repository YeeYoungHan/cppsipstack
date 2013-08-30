#include "StdAfx.h"
#include "SipUserAgentMFC.h"

CSipUserAgentMFC::CSipUserAgentMFC()
{
}

CSipUserAgentMFC::~CSipUserAgentMFC()
{
}

void CSipUserAgentMFC::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
}

bool CSipUserAgentMFC::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	return true;
}

void CSipUserAgentMFC::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
}

void CSipUserAgentMFC::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
}

void CSipUserAgentMFC::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

void CSipUserAgentMFC::EventCallEnd( const char * pszCallId, int iSipStatus )
{
}

void CSipUserAgentMFC::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

bool CSipUserAgentMFC::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	return false;
}

bool CSipUserAgentMFC::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	return false;
}

bool CSipUserAgentMFC::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	return false;
}

void CSipUserAgentMFC::EventCallBackThreadEnd( int iThreadId )
{
}
